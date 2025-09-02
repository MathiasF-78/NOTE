# Arduino-knapp → Flask-API med WiFi‑uppkoppling från EEPROM

> **Syfte (TL;DR):** När en fysisk knapp trycks på Arduino skickas ett HTTP POST till ett lokalt Flask‑API. WiFi‑SSID/lösenord läses från EEPROM via ett litet hjälpbibliotek (WiFiEEPROM) så att inloggningen inte hårdkodas i sketchen.

---

## Innehåll

1. [Översikt](#översikt)
2. [Varför denna design](#varför-denna-design)
3. [Arkitektur](#arkitektur)
4. [Kodstruktur](#kodstruktur)
5. [Flöde vid uppstart och knapptryck](#flöde-vid-uppstart-och-knapptryck)
6. [EEPROM‑hantering (WiFiEEPROM.\*)](#eeprom-hantering-wifieeprom)
7. [HTTP‑klient (Arduino) och Flask‑server](#http-klient-arduino-och-flask-server)
8. [Bygg & kör](#bygg--kör)
9. [Första initiering av EEPROM (provisionering)](#första-initiering-av-eeprom-provisionering)
10. [Felsökning](#felsökning)
11. [Säkerhet](#säkerhet)
12. [Robusthet & förbättringar](#robusthet--förbättringar)
13. [Kända begränsningar](#kända-begränsningar)
14. [Ändringslogg](#ändringslogg)

> **Terminologi & riktighet:** Jag korrigerar felaktiga begrepp där det behövs och noterar vad som är **verifierat** i koden vs. vad som är **antagande/att kontrollera** för din specifika hårdvara.

---

## Översikt

* **Arduino‑sketch** läser SSID/lösenord från EEPROM (via `WiFiEEPROM.h/.cpp`), ansluter till WiFi (bibliotek **WiFiS3**) och väntar på knapptryck (ingång med `INPUT_PULLUP`).
* Vid **fallande flank** (HIGH→LOW) skickas ett **HTTP POST** med JSON‑payload `{"status":"press"}` till en **Flask‑server** på din dator (`/api/button`).
* **Flask‑appen** tar emot POST, skriver ut payload och svarar med `{"message":"Mottaget!"}`.

```text
[Button] --(LOW på tryck)--> Arduino (WiFiS3)
      ^                        |
      |                        v
EEPROM (SSID/PW)         HTTP POST → 192.168.1.100:5000/api/button
                               |
                               v
                         Flask server (Python)
```

---

## Varför denna design

* **Konfig separat från kod:** SSID/lösenord i EEPROM gör att du kan byta nätverk utan ny flashning. (Verifierat: biblioteket läser/skriv­er bytes till fasta offset.)
* **Enkel, transparent HTTP:** Rå `WiFiClient` + manuella HTTP‑headers ger full kontroll och minimala beroenden.
* **Edge‑detektering på knapp:** Mindre brus än att polla kontinuerligt; enkelt att bygga vidare med debounce.
* **Flask för snabb prototyp:** Minimal server för att testa integrationskedjan från maskin → API.

---

## Arkitektur

* **Hårdvara**: Arduino‑kort med stöd för **WiFiS3** (t.ex. *UNO R4 WiFi / GIGA R1 / Portenta C33*). **Antagande**: kontrollera att ditt kort verkligen använder `WiFiS3`; annars krävs rätt bibliotek (t.ex. `WiFiNINA` för MKR WiFi 1010).
* **Nätverk**: Datorn kör Flask på **port 5000** och är nåbar från Arduinons nät. IP hårdkodas i sketchen (`192.168.1.100`).
* **Lagring**: SSID/lösenord sparas i **EEPROM** på fasta adresser.

---

## Kodstruktur

```
.
├─ Ino sketch (huvudsketchen)
├─ WiFiEEPROM.h / WiFiEEPROM.cpp  (hjälpbibliotek för SSID/PW i EEPROM)
└─ python script (Flask-servern)
```

* **Sketch**: Initierar seriell, knapp‑pin med `INPUT_PULLUP`, kallar `EEPROM.begin()` (*antagande*: på vissa plattformar krävs detta), läser SSID/PW via `getSSID()/getPassword()`, ansluter till WiFi och lyssnar på knapp.
* **WiFiEEPROM**: Tillhandahåller `readWiFiCredentials()`, `writeWiFiCredentials()`, `setupWiFi()`, `getSSID()`, `getPassword()`.
* **Flask**: En enkel endpoint `/api/button` som tar emot JSON över POST.

---

## Flöde vid uppstart och knapptryck

1. **Uppstart**

   * `EEPROM.begin()` (om plattformen kräver det). *(Se [EEPROM‑hantering](#eeprom-hantering-wifieeprom))*
   * `readWiFiCredentials()` läser SSID och lösenord till globala buffertar.
   * `WiFi.begin(ssid, password)` → blockar tills `WL_CONNECTED` (loop med `delay(500)`).
2. **Knapptryck**

   * Knappen är internt pull‑up (viloläge `HIGH`), tryck ger `LOW`.
   * På **fallande flank** (`buttonState == LOW && lastButtonState == HIGH`) skapas `WiFiClient` och ett **HTTP POST** skickas till Flask.

---

## EEPROM‑hantering (WiFiEEPROM)

**Verifierat i koden:**

* **Layout:**

  * SSID: adresser `0 .. SSID_MAX_LEN-1` (32 bytes)
  * Password: adresser `SSID_MAX_LEN .. SSID_MAX_LEN + PASSWORD_MAX_LEN - 1` (64 bytes)
* **Nollterminering:** Sista byte i respektive buffert sätts till `\0` vid läsning.
* **Skrivning:** `writeWiFiCredentials()` fyller ut resterande bytes med `0` om data är kortare.

**Att kontrollera per plattform:**

* På vissa kort (t.ex. ESP‑familjen, UNO R4) krävs **`EEPROM.begin(size)`** innan läs/skriv och **`EEPROM.commit()`** efter skrivning. Din sketch anropar `EEPROM.begin()` men **`EEPROM.commit()` saknas** i `writeWiFiCredentials()`; lägg till commit om ditt kort kräver det.
* Nuvarande lösning saknar **"magic"/version** för att veta om EEPROM är initierad. Överväg att reservera t.ex. 4 bytes (`'W','I','F','I'`) som header.

---

## HTTP‑klient (Arduino) och Flask‑server

### Arduino → HTTP POST

**Verifierat i koden:**

* Öppnar TCP mot `192.168.1.100:5000` och skickar en **HTTP/1.1 POST** med `Content-Type: application/json`.
* Payload: `{"status":"press"}`.

**Rättelse/förbättring:**

* `Content-Length` **måste** vara antalet **bytes** i payload. För `{"status":"press"}` är längden **18**, inte 17.
* Använd `client.print(payload)` (ej `println`) för själva payload‑raden för att undvika extra `\r\n`.
* Läs gärna svaret från servern för att tömma socketen och få enkel kvittens.

**Exempel (suggestion):**

```cpp
const char* host = "192.168.1.100";
const int   port = 5000;
const char* path = "/api/button";
const char* payload = "{\"status\":\"press\"}"; // 18 bytes

if (client.connect(host, port)) {
  client.print("POST "); client.print(path); client.println(" HTTP/1.1");
  client.print("Host: "); client.println(host);
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: "); client.println(strlen(payload));
  client.println(); // tom rad före kropp
  client.print(payload); // exakt payload, ingen extra rad

  // (Valfritt) Läs svar
  unsigned long t0 = millis();
  while (client.connected() && millis() - t0 < 2000) {
    while (client.available()) Serial.write(client.read());
  }
  client.stop();
} else {
  Serial.println("Kunde inte ansluta till server");
}
```

### Flask‑servern

* Endpoint: `POST /api/button`
* Läser `request.json` och skriver ut i konsolen.
* Returnerar 200 med JSON `{ "message": "Mottaget!" }`.

**Tips:** Kör i virtuell miljö, se [Bygg & kör](#bygg--kör). För test från datorn:

```bash
curl -i -X POST http://127.0.0.1:5000/api/button \
  -H "Content-Type: application/json" \
  -d '{"status":"press"}'
```

---

## Bygg & kör

### 1) Arduino‑sidan

* **Bibliotek:** `WiFiS3`, `EEPROM` (ingår ofta i kärnan), samt dina `WiFiEEPROM.*`.
* **Pinout:** `buttonPin = 2` med `INPUT_PULLUP`. Koppla knappen mellan **D2** och **GND**.
* **Nät:** Uppdatera IP (`192.168.1.100`) och port vid behov.
* **Timeout:** Koden väntar oändligt på WiFi; överväg timeout (se förbättringar).

### 2) Python/Flask‑sidan

```bash
python -m venv .venv
source .venv/bin/activate   # Windows: .venv\Scripts\activate
pip install flask
python app.py  # startar på 0.0.0.0:5000
```

Se till att brandvägg tillåter inkommande på port 5000 och att datorns IP matchar i sketchen.

---

## Första initiering av EEPROM (provisionering)

Om EEPROM inte innehåller giltiga uppgifter måste de skrivas in **en gång**.

**Alternativ A – tillfällig provisioning‑sketch**

```cpp
#include <EEPROM.h>
#include "WiFiEEPROM.h"

void setup(){
  Serial.begin(9600);
  EEPROM.begin();              // OBS: på vissa plattformar krävs storlek, t.ex. EEPROM.begin(256)
  writeWiFiCredentials("DittSSID", "DittLösenord");
  // EEPROM.commit();          // Lägg till om ditt kort kräver commit
  Serial.println("Credentials skrivna.");
}
void loop(){}
```

Flasha, öppna Serial Monitor för bekräftelse, flasha sedan **huvudsketchen**.

**Alternativ B – via seriella kommandon** (framtida förbättring): Lägg till ett enkelt seriellt meny‑läge som låter dig skriva in SSID/lösenord utan att ändra koden.

---

## Felsökning

* **Fastnar på anslutning (bara punkter):**

  * Kontrollera SSID/lösenord i EEPROM (skriv om dem).
  * Kontrollera att kortet verkligen använder `WiFiS3`‑stacken. *(Antagande: byt bibliotek om annat kort.)*
  * Routerns 2.4 GHz kontra 5 GHz (många Arduino‑kort stöder bara 2.4 GHz).
* **"Kunde inte ansluta till server":**

  * Testa `ping 192.168.1.100` från annan enhet.
  * Kör `curl` lokalt mot Flask (se ovan). Om OK, testa från annan värddator i samma nät.
  * Brandvägg/port 5000.
* **Ingen payload i Flask:**

  * Säkerställ `Content-Length` korrekt och att payload skickas med `client.print()`.
  * Logga `request.data` i Flask för att se rå bytes.
* **Flera POST vid ett tryck:**

  * Lägg till **debounce** (se förbättringar) eller hårdvaru‑RC.

---

## Säkerhet

* **Klartext i EEPROM:** SSID/lösenord lagras i klartext. OK för labbnät, men inte för känsliga miljöer.
* **Oautentiserat API:** Alla på LAN kan POST\:a. Lägg till t.ex. hemlig **API‑nyckel** i HTTP‑header (`X-API-Key`) eller HMAC‑signatur av payload.
* **Ingen TLS:** Flask körs över HTTP. För produktion: sätt Flask bakom en omvänd proxy (nginx/Traefik) med TLS, eller använd `WiFiSSLClient` om ditt kort och servern stödjer HTTPS.

---

## Robusthet & förbättringar

1. **Content-Length & läs svar** *(hygien)* – enligt koden i [HTTP‑klient](#http-klient-arduino-och-flask-server).
2. **Debounce** – enkel mjukvarudebounce:

   ```cpp
   const unsigned long DEBOUNCE_MS = 30;
   static unsigned long lastChange = 0;
   if (buttonState != lastButtonState && millis() - lastChange > DEBOUNCE_MS) {
     lastChange = millis();
     if (buttonState == LOW) {
       // skicka POST
     }
   }
   lastButtonState = buttonState;
   ```
3. **Timeouts & återanslutning** – bryt WiFi‑väntan efter t.ex. 20 s; försök igen periodiskt.
4. **Init‑kontroll i EEPROM** – skriv en **magic** (t.ex. `0x57,0x49,0x46,0x49` = "WIFI") + version vid offset 0 och börja SSID efter headern.
5. **Konfig via seriell** – meny för att läsa/skriva credentials och server‑IP.
6. **JSON‑utökning** – skicka t.ex. tidsstämpel, knapp‑id, batterinivå.
7. **Återkoppling** – tänd LED när POST lyckas (kvittens efter att svaret lästs).

---

## Kända begränsningar

* Blockerande `while (WiFi.status() != WL_CONNECTED)` kan låsa uppstart om nätet saknas.
* Hårdkodad IP; ingen mDNS/namnuppslagning.
* Ingen felhantering för EEPROM‑korruption.

---

## Ändringslogg

* **v1.0** – Första versionen av README. Innehåller rättning av `Content-Length`, råd om `EEPROM.commit()` och förbättrad HTTP‑sändning.

---

### Snabb checklista

* [ ] Skriv in SSID/lösenord i EEPROM (provisionering).
* [ ] Starta Flask på rätt IP/port och öppna brandvägg.
* [ ] Uppdatera IP i sketchen.
* [ ] Testa `curl` → testa Arduino.
* [ ] Lägg till debounce och (ev.) `EEPROM.commit()` beroende på pl
