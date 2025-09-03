#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Arduino_LED_Matrix.h>

ArduinoLEDMatrix matrix;  // <--- Nu deklarerad korrekt

// Långfinger på 12x8 LED-matris
uint8_t middleFinger[8][12] = {
  {0,0,0,0,0,1,0,0,0,0,0,0},  // långfingret topp
  {0,0,0,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,0,0,0,0,0},
  {0,0,0,0,0,1,0,0,0,0,0,0},  // långfingret fortsätter
  {0,1,0,1,1,1,1,1,0,1,0,0},  // handflata + ring/lillfinger/pekfinger
  {0,1,0,1,0,0,0,1,0,1,0,0},  
  {0,1,0,1,0,0,0,1,0,1,0,0},  
  {0,0,0,0,0,0,0,0,0,0,0,0}   // tom rad
};


// --- Funktionsprototyper ---
void displayMessage(String msg, bool clear=true);
void setRGBColor(int r, int g, int b);
void showMiddleFinger();  // <-- prototyp

// --- OLED ---
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// --- RGB LED ---
#define RED_PIN    8
#define GREEN_PIN  10
#define BLUE_PIN   11

// --- BUTTON ---
#define BUTTON_PIN 7

// --- RFID ---
#define SS_PIN 9
#define RST_PIN 6
MFRC522 rfid(SS_PIN, RST_PIN);

bool accessGranted = false; // Start som denied
bool lastButtonState = LOW;
bool firstPressDone = false; // Första tryck används för API-flöde
bool apiSimulationDone = false; // Indikerar att API-simuleringen är klar
bool toggleNext = false; 

void setup() {
  matrix.begin(); //Finger Matrix

  Serial.begin(115200);
  Wire.begin();
  
// --- OLED ---
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  display.clearDisplay();
  display.display();

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  setRGBColor(0,0,0);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  SPI.begin();
  rfid.PCD_Init();

  // Visa startmeddelande
  displayMessage("System Ready");
  setRGBColor(0,0,255);
}

void loop() {

  // --- BUTTON ---
  bool buttonState = digitalRead(BUTTON_PIN) == LOW;

  // Första knapptryck → API-simulering
  if(buttonState && !lastButtonState && !firstPressDone) {
      firstPressDone = true;

      displayMessage("Connecting to\nIFS Cloud...");
      delay(2000);
      displayMessage("API Communication...");
      delay(2000);
      displayMessage("Retrieving Data...");
      delay(2000);

      accessGranted = false;
      displayMessage("Access Denied");
      setRGBColor(255,0,0);
      showMiddleFinger();

      apiSimulationDone = true; // Nu kan växling börja
  }

  /// Efter API-simulering → växla mellan Access Denied / Granted
if(buttonState && !lastButtonState && apiSimulationDone) {
    if(!toggleNext) {
        // Första knapptryck efter API: stanna på Access Denied
        toggleNext = true;
    } else {
        accessGranted = !accessGranted;
        if(accessGranted) {
            displayMessage("Access Granted");
            setRGBColor(0,255,0);
        } else {
            displayMessage("Access Denied");
            setRGBColor(255,0,0);
            showMiddleFinger();
        }
    }
}

  lastButtonState = buttonState;
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

void showMiddleFinger() {
  matrix.renderBitmap(middleFinger, 8, 12);
}
