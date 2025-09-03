


//
//
// OLED och anslutning fungerar, inte RGB eller button funktion
//
//



// Tools → Board → Arduino Uno R4 WiFi

#include <WiFiS3.h>
#include <EEPROM.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SSID_MAX_LEN     32
#define PASSWORD_MAX_LEN 64

// RGB:
#define RED_PIN    3 // Digital with 220 - 330 ohm resistor
#define GREEN_PIN  5 // Digital with 220 - 330 ohm resistor
#define BLUE_PIN   6 // Digital with 220 - 330 ohm resistor
// GND PIN

// Momentary Button Module:
#define BUTTON_PIN 7 // Digital
// GND PIN

// OLED:
// OLED SDA (I2C) PIN A4 // Analog
// OLED SCL (I2C) PIN A5 // Analog

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

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
  WiFi.macAddress(mac);
  char macStr[18];
  sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X",
          mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
  return String(macStr);
}

void setLEDColor(bool r, bool g, bool b) {
  digitalWrite(RED_PIN, r ? HIGH : LOW);
  digitalWrite(GREEN_PIN, g ? HIGH : LOW);
  digitalWrite(BLUE_PIN, b ? HIGH : LOW);
}

void showSignalBars(long rssi) {
  int bars = 0;
  if (rssi > -50) bars = 4;
  else if (rssi > -60) bars = 3;
  else if (rssi > -70) bars = 2;
  else if (rssi > -80) bars = 1;
  else bars = 0;

  for (int i = 0; i < 4; i++) {
    display.fillRect(10 + i * 10, 50 - i * 10, 8, i * 10, i < bars ? WHITE : BLACK);
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

  readWiFiCredentials();

  display.setCursor(0, 0);
  display.print("Ansluter till ");
  display.println(ssid);
  display.display();

  Serial.print("Försöker ansluta till SSID: ");
  Serial.println(ssid);

  setLEDColor(false, false, true); // blå blink
  WiFi.begin(ssid, password);

  uint8_t attempt = 0;
  while (WiFi.status() != WL_CONNECTED && attempt < 20) {
    delay(500);
    setLEDColor(false, false, attempt % 2); // blink blå
    attempt++;
  }

  uint8_t status = WiFi.status();
  display.clearDisplay();

  if (status == WL_CONNECTED) {
    setLEDColor(false, true, false); // grön
    display.setCursor(0, 0);
    display.print("Ansluten till ");
    display.println(ssid);
    display.setCursor(0, 20);
    display.print("IP: ");
    display.println(WiFi.localIP());
    showSignalBars(WiFi.RSSI());
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

  if (buttonState == LOW && lastButtonState == HIGH) {
    display.setCursor(0, 40);
    display.println("Knapp aktiverad");
    display.display();
  } else if (buttonState == HIGH && lastButtonState == LOW) {
    display.fillRect(0, 40, 128, 10, BLACK);
    display.display();
  }

  lastButtonState = buttonState;
  delay(100);
}
