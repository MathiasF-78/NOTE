// Use 220 ohm resistors for each R,G,B channel for the LED Module
// Tools → Board → Arduino Uno R4 WiFi

#include <WiFiS3.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WiFiEEPROM.h>

#define SSID_MAX_LEN     32
#define PASSWORD_MAX_LEN 64

// RGB LED
#define RED_PIN    8
#define GREEN_PIN  10
#define BLUE_PIN   12

// BUTTON
#define BUTTON_PIN 7

// OLED (I2C)
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

String getMacAddress() {
  uint8_t mac[6];
  WiFi.macAddress(mac);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

void setLEDColor(bool r, bool g, bool b) {
  analogWrite(RED_PIN, r ? 255 : 0);
  analogWrite(GREEN_PIN, g ? 255 : 0);
  analogWrite(BLUE_PIN, b ? 255 : 0);
}

void showSignalBars(long rssi) {
  int bars = 0;
  if (rssi > -50) bars = 4;
  else if (rssi > -60) bars = 3;
  else if (rssi > -70) bars = 2;
  else if (rssi > -80) bars = 1;
  else bars = 0;

  int startX = 100;  // höger hörn
  int baseY = 0;     // längst upp
  int barWidth = 4;
  int gap = 2;

  for (int i = 0; i < 4; i++) {
    int height = (i + 1) * 5;          // höjden på staplarna
    int x = startX + i * (barWidth + gap);
    int y = baseY + (20 - height);     // staplar växer uppåt

    // Rensa området först
    display.fillRect(x, baseY, barWidth, 20, BLACK); // rensa bakgrund
    if (i < bars) {
        display.fillRect(x, y, barWidth, height, WHITE);
    }
  }
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println("OLED init misslyckades");
    while (true);
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  // Läs SSID och lösenord från EEPROM
  readWiFiCredentials();

  display.setCursor(0, 0);
  display.print("Ansluter till:");
  display.setCursor(0, 10);
  display.print(ssid);
  display.display();

  // Blinkar blått två gånger
  for (int i = 0; i < 2; i++) {
    setLEDColor(false, false, true);
    delay(300);
    setLEDColor(false, false, false);
    delay(300);
  }

  Serial.print("Försöker ansluta till SSID: ");
  Serial.println(ssid);

  setLEDColor(false, false, true); // blå
  WiFi.begin(ssid, password);

  uint8_t attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20) {
    delay(500);
    attempt++;
  }

  display.clearDisplay();
  if (WiFi.status() == WL_CONNECTED) {
    display.setCursor(0, 0);
    display.print("Ansluten till:");
    display.setCursor(0, 10);
    display.print(ssid);
    display.display();

    // Vänta på IP
    IPAddress ip;
    while ((ip = WiFi.localIP()) == IPAddress(0, 0, 0, 0)) {
      delay(100);
    }

    setLEDColor(false, true, false); // grön

    // Rensa hela området för IP
    display.fillRect(0, 30, 128, 20, BLACK);  // y = 30

    // Skriv statisk text
    display.setCursor(0, 30);
    display.println("IP:");

    // Skriv IP som en sträng
    display.setCursor(0, 40);  // flytta ner en rad
    String ipStr = String(ip[0]) + "." + String(ip[1]) + "." + String(ip[2]) + "." + String(ip[3]);
    display.print(ipStr);
  } else {
    setLEDColor(true, false, false); // röd
    display.setCursor(0, 0);
    display.println("Anslutning misslyckad");
  }

  display.display();
}

void loop() {
  static bool lastButtonState = HIGH;
  bool buttonState = digitalRead(BUTTON_PIN);

  // Knappstatus
  if (buttonState == LOW && lastButtonState == HIGH) {
    display.fillRect(0, 55, 128, 10, BLACK);
    display.setCursor(0, 55);
    display.println("Knapp aktiverad");
    display.display();
  } else if (buttonState == HIGH && lastButtonState == LOW) {
    display.fillRect(0, 55, 128, 10, BLACK);
    display.display();
  }
  lastButtonState = buttonState;

  // Visa signalstyrka om WiFi är anslutet
  if (WiFi.status() == WL_CONNECTED) {
    long rssi = WiFi.RSSI();
    showSignalBars(rssi);
    display.display();
  }

  delay(500);
}
