//version 1#include <WiFiS3.h>   // Arduino Uno R4 WiFi:
/*
#include <EEPROM.h>

#define SSID_MAX_LEN     32
#define PASSWORD_MAX_LEN 64

char ssid[SSID_MAX_LEN + 1];
char password[PASSWORD_MAX_LEN + 1];

void readWiFiCredentials() {
  for (int i = 0; i < SSID_MAX_LEN; i++) ssid[i] = EEPROM.read(i);
  ssid[SSID_MAX_LEN] = 0;

  for (int i = 0; i < PASSWORD_MAX_LEN; i++) password[i] = EEPROM.read(SSID_MAX_LEN + i);
  password[PASSWORD_MAX_LEN] = 0;
}

String getMacAddress() {
  uint8_t mac[6];
  WiFi.macAddress(mac); // fyller arrayen
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

void printWiFiError(uint8_t status) {
  Serial.println("---- WiFi Felsökning ----");
  Serial.print("Fel-ID: "); Serial.println(status);

  switch(status) {
    case WL_CONNECTED:
      Serial.println("Beskrivning: Anslutning lyckades.");
      Serial.println("Rotorsak: Ingen, allt fungerar.");
      Serial.println("Åtgärd: Ingen åtgärd behövs.");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("Beskrivning: Nätverket finns inte.");
      Serial.println("Rotorsak: Fel SSID, 5GHz-nätverk, eller utom räckhåll.");
      Serial.println("Åtgärd: Kontrollera att SSID är korrekt, och att nätverket är 2.4GHz och inom räckhåll.");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("Beskrivning: Anslutning misslyckades.");
      Serial.println("Rotorsak: Fel lösenord eller nätverket blockerar enheten (IT-policy).");
      Serial.println("Åtgärd: Kontrollera lösenordet, registrera enheten i nätverket om företagsnätverk.");
      break;
    case WL_CONNECTION_LOST:
      Serial.println("Beskrivning: Anslutning förlorad.");
      Serial.println("Rotorsak: Tillfällig störning eller signalproblem.");
      Serial.println("Åtgärd: Kontrollera signalstyrka och att enheten är inom räckhåll.");
      break;
    case WL_DISCONNECTED:
      Serial.println("Beskrivning: Enheten är frånkopplad.");
      Serial.println("Rotorsak: WiFi.begin() har inte lyckats eller enheten har kopplats ner.");
      Serial.println("Åtgärd: Starta om anslutning.");
      break;
    default:
      Serial.println("Beskrivning: Okänt fel.");
      Serial.println("Rotorsak: Kan vara en bugg eller okänt statusvärde.");
      Serial.println("Åtgärd: Kontrollera kod och biblioteksversioner.");
      break;
  }
  Serial.println("--------------------------");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  readWiFiCredentials();

  Serial.println("---- EEPROM innehåll ----");
  Serial.print("SSID: "); Serial.println(ssid);
  Serial.print("Lösenord: "); Serial.println(password);
  Serial.print("MAC-adress: "); Serial.println(getMacAddress());
  Serial.println("------------------------");
}

void loop() {
  static uint32_t lastCheck = 0;
  uint32_t now = millis();

  // Kontrollera WiFi-status var 5:e sekund
  if(now - lastCheck >= 5000) {
    lastCheck = now;

    uint8_t status = WiFi.status();

    // Om ej ansluten, försök koppla upp
    if(status != WL_CONNECTED) {
      Serial.print("Försöker ansluta till SSID: ");
      Serial.println(ssid);
      WiFi.begin(ssid, password);

      // Vänta kort stund för anslutning
      uint8_t attempt = 0;
      while (WiFi.status() != WL_CONNECTED && attempt < 20) { // max 10 sekunder
        delay(500);
        Serial.print(".");
        attempt++;
      }
      Serial.println();
      status = WiFi.status();
    }

    printWiFiError(status);

    if(status == WL_CONNECTED) {
      Serial.print("IP-adress: "); Serial.println(WiFi.localIP());
    }
  }
}
*/

#include <WiFiS3.h>
#include <EEPROM.h>

#define SSID_MAX_LEN     32
#define PASSWORD_MAX_LEN 64

char ssid[SSID_MAX_LEN + 1];
char password[PASSWORD_MAX_LEN + 1];

// Läs WiFi credentials från EEPROM
void readWiFiCredentials() {
  for (int i = 0; i < SSID_MAX_LEN; i++) ssid[i] = EEPROM.read(i);
  ssid[SSID_MAX_LEN] = 0;

  for (int i = 0; i < PASSWORD_MAX_LEN; i++) password[i] = EEPROM.read(SSID_MAX_LEN + i);
  password[PASSWORD_MAX_LEN] = 0;
}

// Läs MAC-adress som sträng
String getMacAddress() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

// Skriv ut felsökningsinfo baserat på WiFi.status()
void printWiFiError(uint8_t status) {
  Serial.println("---- WiFi Felsökning ----");
  Serial.print("Fel-ID: "); Serial.println(status);

  switch(status) {
    case WL_CONNECTED:
      Serial.println("Beskrivning: Anslutning lyckades.");
      Serial.println("Rotorsak: Ingen, allt fungerar.");
      Serial.println("Åtgärd: Ingen åtgärd behövs.");
      break;
    case WL_NO_SSID_AVAIL:
      Serial.println("Beskrivning: Nätverket finns inte.");
      Serial.println("Rotorsak: Fel SSID, 5GHz-nätverk, eller utom räckhåll.");
      Serial.println("Åtgärd: Kontrollera att SSID är korrekt, att nätverket är 2.4GHz och inom räckhåll.");
      break;
    case WL_CONNECT_FAILED:
      Serial.println("Beskrivning: Anslutning misslyckades.");
      Serial.println("Rotorsak: Fel lösenord eller nätverket blockerar enheten (IT-policy).");
      Serial.println("Åtgärd: Kontrollera lösenordet, registrera enheten i nätverket om företagsnätverk.");
      break;
    case WL_CONNECTION_LOST:
      Serial.println("Beskrivning: Anslutning förlorad.");
      Serial.println("Rotorsak: Tillfällig störning eller signalproblem.");
      Serial.println("Åtgärd: Kontrollera signalstyrka och att enheten är inom räckhåll.");
      break;
    case WL_DISCONNECTED:
      Serial.println("Beskrivning: Enheten är frånkopplad.");
      Serial.println("Rotorsak: WiFi.begin() har inte lyckats eller enheten har kopplats ner.");
      Serial.println("Åtgärd: Starta om anslutning.");
      break;
    default:
      Serial.println("Beskrivning: Okänt fel.");
      Serial.println("Rotorsak: Möjliga orsaker:");
      Serial.println("- Fel SSID eller lösenord i EEPROM");
      Serial.println("- Enheten är inte godkänd på nätverket (MAC-blockerad)");
      Serial.println("- Signalproblem / avstånd till accesspunkt");
      Serial.println("- Felaktigt bibliotek eller inkompatibel hårdvara");
      Serial.println("Åtgärd: Kontrollera credentials, MAC-registrering, närhet till AP och biblioteksversion.");
      break;
  }
  Serial.println("--------------------------");
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // Läs credentials från EEPROM
  readWiFiCredentials();

  // Visa info
  Serial.println("---- EEPROM innehåll ----");
  Serial.print("SSID: "); Serial.println(ssid);
  Serial.print("Lösenord: "); Serial.println(password);
  Serial.print("MAC-adress: "); Serial.println(getMacAddress());
  Serial.println("------------------------");

  // Försök ansluta
  Serial.print("Försöker ansluta till SSID: "); Serial.println(ssid);
  WiFi.begin(ssid, password);

  uint8_t attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20) { // max 10 sekunder
    delay(500);
    Serial.print(".");
    attempt++;
  }
  Serial.println();

  uint8_t status = WiFi.status();
  printWiFiError(status);

  if(status == WL_CONNECTED) {
    Serial.print("IP-adress: "); Serial.println(WiFi.localIP());
  }
}

void loop() {
  // Kan repetera anslutningstest om du vill
}


