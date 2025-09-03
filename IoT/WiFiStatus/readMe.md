# Arduino Uno R4 WiFi: WiFi Credentials från EEPROM

Detta projekt demonstrerar hur Arduino Uno R4 WiFi kan läsa WiFi-credentials (SSID & lösenord) från EEPROM och använda dem för att ansluta till ett WiFi-nätverk. Sketchen hjälper med felsökning och visar relevant anslutningsinformation via seriell monitor.

## Funktioner

- **Läser SSID och lösenord från EEPROM**
- **Försöker ansluta till WiFi och skriver dessutom ut MAC-adress & anslutningsstatus**
- **Ger utförlig felsökningsinformation via seriell monitor baserat på WiFi.status()**

## Kom igång

### Hårdvara

- Arduino Uno R4 WiFi (med ESP32-S3-modul)
- Strömkälla via USB

### Mjukvara

- Arduino IDE (eller Arduino Web Editor)
- Installerat support för Arduino Uno R4 WiFi (från Board Manager)

### Installation

1. Kopiera sketchen till Arduino IDE.
2. Kompilera och ladda upp till din Arduino Uno R4 WiFi.
3. Säkerställ att EEPROM innehåller giltiga credentials:
   - SSID sparas på adress 0–31
   - Lösenord sparas på adress 32–95
4. Öppna Serial Monitor (115200 baud), starta kortet.

### Exempel på användning

Seriell monitor visar:
```
---- EEPROM innehåll ----
SSID: MittWiFi
Lösenord: mitt_hemliga_losenord
MAC-adress: AA:BB:CC:DD:EE:FF
------------------------
Försöker ansluta till SSID: MittWiFi
...
---- WiFi Felsökning ----
Fel-ID: 3
Beskrivning: Anslutning lyckades.
Rotorsak: Ingen, allt fungerar.
Åtgärd: Ingen åtgärd behövs.
--------------------------
IP-adress: 192.168.1.10
```

Vid fel ger sketchen tydlig felbeskrivning och åtgärdsförslag (ex: fel SSID, lösenord, utom räckhåll etc).

## Funktion för felsökning

Funktionen `printWiFiError(status)` skriver ut:
- Felbeskrivning
- Rotorsak
- Åtgärdsförslag

## Anpassning

Vill du ändra var i EEPROM credentials lagras eller lägga till säkerhetsfunktioner, justera adressering och logik i `readWiFiCredentials()`.

## Licens

Projektet är fritt att använda och modifiera. Bidra gärna med förbättringar!

***

## Referenser

- [Arduino Uno R4 WiFi officiell dokumentation][4][1]
- [Exempelkod och tutorials][2][5]
- [Guide till README.md på GitHub][3]

***

Kopiera och spara denna README.md till ditt GitHub-repo för att enklare dela och dokumentera din sketch![1][2][3]

[1](https://docs.arduino.cc/tutorials/uno-r4-wifi/wifi-examples)
[2](https://github.com/PerfecXX/Arduino-UNO-R4-Tutorial)
[3](https://dev.to/mochafreddo/how-to-manage-documentation-in-a-github-repository-a-guide-for-junior-developers-pgo)
[4](https://docs.arduino.cc/hardware/uno-r4-wifi)
[5](https://docs.arduino.cc/tutorials/uno-r4-wifi/r4-wifi-getting-started)
[6](https://docs.52pi.com/md/kz-0073/arduino/p5/)
[7](https://www.youtube.com/watch?v=hiDUket7pt0)
[8](https://stackoverflow.com/questions/61440464/saving-wifi-credentials-to-eeprom-on-esp8266)
[9](https://github.com/loneconspirator/WifiCreds)
[10](https://arduino.github.io/arduino-cli/0.31/sketch-specification/)
[11](https://gist.github.com/zjor/98eebd8cbfa1e907197fea60456b0958)
