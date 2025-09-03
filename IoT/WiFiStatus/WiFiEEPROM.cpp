#include <EEPROM.h>
#include <WiFiS3.h>   // nödvändigt för WiFi.begin(), WiFi.status() m.m.
#include <string.h>   // för strlen()
#include "WiFiEEPROM.h"

char ssid[SSID_MAX_LEN + 1];
char password[PASSWORD_MAX_LEN + 1];

void readWiFiCredentials() {
    for (int i = 0; i < SSID_MAX_LEN; i++) ssid[i] = EEPROM.read(i);
    ssid[SSID_MAX_LEN] = 0;
    for (int i = 0; i < PASSWORD_MAX_LEN; i++) password[i] = EEPROM.read(SSID_MAX_LEN + i);
    password[PASSWORD_MAX_LEN] = 0;
}

void writeWiFiCredentials(const char* initialSSID, const char* initialPassword) {
    for (int i = 0; i < SSID_MAX_LEN; i++) {
        if (i < strlen(initialSSID)) EEPROM.write(i, initialSSID[i]);
        else EEPROM.write(i, 0);
    }
    for (int i = 0; i < PASSWORD_MAX_LEN; i++) {
        if (i < strlen(initialPassword)) EEPROM.write(SSID_MAX_LEN + i, initialPassword[i]);
        else EEPROM.write(SSID_MAX_LEN + i, 0);
    }
}

void setupWiFi() {
    readWiFiCredentials();
    Serial.print("Connecting to SSID: "); Serial.println(ssid);
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println("\nConnected!");
    Serial.print("IP address: "); Serial.println(WiFi.localIP());
}

/* Nytt */
const char* getSSID() {
  return ssid;
}

const char* getPassword() {
  return password;
}
