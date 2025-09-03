#ifndef WIFI_EEPROM_H
#define WIFI_EEPROM_H

#include <Arduino.h>
#include <EEPROM.h>


#define SSID_MAX_LEN     32
#define PASSWORD_MAX_LEN 64

// Globala variabler för WiFi credentials
extern char ssid[SSID_MAX_LEN + 1];
extern char password[PASSWORD_MAX_LEN + 1];

// Funktioner
void readWiFiCredentials();
void writeWiFiCredentials(const char* initialSSID, const char* initialPassword);
//void setupWiFi();

/*Nytt */

void setupWiFi();
const char* getSSID();
const char* getPassword();

#endif
