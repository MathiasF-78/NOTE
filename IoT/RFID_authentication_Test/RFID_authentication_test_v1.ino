#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>
#include "WiFiEEPROM.h"

// --- Funktionsprototyper ---
void displayMessage(String msg, bool clear=true);
void setRGBColor(int r, int g, int b);
void handleButton();
void handleRFID();

// --- OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- RGB LED (HW_479) ---
#define RED_PIN    8
#define GREEN_PIN  10
#define BLUE_PIN   11

// --- Knapp ---
#define BUTTON_PIN 7
bool accessGranted = false;
bool lastButtonState = LOW;

// --- RFID ---
#define SS_PIN 9
#define RST_PIN 6
MFRC522 rfid(SS_PIN, RST_PIN);

void setup() {
  Serial.begin(115200);
  Wire.begin();
  
  // OLED init
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.display();

  // RGB LED init
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  setRGBColor(0,0,0);

  // Knapp init
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // RFID init
  SPI.begin();
  rfid.PCD_Init();

  // Startmeddelande
  displayMessage("System Ready");
  setRGBColor(0,0,255); // Blå standby
}

void loop() {
  handleButton();
  handleRFID();
}

// --- Funktioner ---
void displayMessage(String msg, bool clear) {
  if(clear) display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 10);
  display.println(msg);
  display.display();
}

void setRGBColor(int r, int g, int b) {
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}

void handleButton() {
  bool buttonState = digitalRead(BUTTON_PIN) == LOW;
  if(buttonState && !lastButtonState) {
    accessGranted = !accessGranted;
    if(accessGranted) {
      displayMessage("Access Granted");
      setRGBColor(0,255,0);
    } else {
      displayMessage("Access Denied");
      setRGBColor(255,0,0);
    }
  }
  lastButtonState = buttonState;
}

void handleRFID() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) return;

  displayMessage("RFID Tag Detected\nCommunicating with IFS...");

  for(int i=0; i<6; i++){
    String dots = "";
    for(int j=0; j<=i; j++) dots += ".";
    displayMessage("Communicating with IFS" + dots, false);
    delay(500);
  }

  accessGranted = false;
  displayMessage("Access Denied");
  setRGBColor(255,0,0);

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
