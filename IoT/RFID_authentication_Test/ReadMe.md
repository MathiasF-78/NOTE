# Arduino RFID + OLED + RGB LED + LED Matrix

This project implements a prototype access control system with **API simulation**, using:
- RFID reader (MFRC522)
- OLED display (128x64)
- RGB LED
- Push button
- Arduino LED Matrix (12x8)

The system simulates communication with **IFS Cloud** on the first button press, then toggles between "Access Granted" and "Access Denied" with visual and LED feedback.  
If access is denied, the LED matrix displays a middle finger icon.

---

## Features

- **API simulation flow** on first button press:
  1. Connecting to IFS Cloud...
  2. API Communication...
  3. Retrieving Data...
  4. Access Denied + Matrix animation
- **Button-controlled toggling** between Access Granted / Denied (after first simulation).
- **OLED display** shows messages.
- **RGB LED** indicates state:
  - Blue = Standby
  - Green = Access Granted
  - Red = Access Denied
- **LED Matrix** displays a middle finger when access is denied.

---

## Hardware Requirements

- Arduino board (e.g., Uno, Mega)
- MFRC522 RFID reader (not actively used in this revision, but ready for extension)
- OLED display (128x64, I2C)
- RGB LED (common cathode)
- Push button
- Arduino LED Matrix (12x8)
- Wires and resistors

### Pin Configuration

| Component       | Pin        |
|-----------------|-----------|
| RED LED         | 8         |
| GREEN LED       | 10        |
| BLUE LED        | 11        |
| BUTTON          | 7         |
| RFID SS         | 9         |
| RFID RST        | 6         |
| OLED I2C        | SDA/SCL   |
| LED Matrix      | Arduino Nano 33 BLE / compatible board |

---

## Libraries Required

- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [MFRC522](https://github.com/miguelbalboa/rfid)
- [Arduino_LED_Matrix](https://github.com/arduino-libraries/Arduino_LED_Matrix)
- SPI (built-in Arduino)
- Wire (built-in Arduino)

---

## Usage

1. Connect all components according to the pin configuration.
2. Install required libraries in Arduino IDE.
3. Upload the sketch to your Arduino.
4. Open Serial Monitor at **115200 baud** for debug output.
5. **System start:**
   - OLED → "System Ready"
   - RGB LED → Blue
6. **First button press:**
   - Simulates API communication with IFS Cloud.
   - Ends with Access Denied (Red LED + Matrix middle finger).
7. **Subsequent presses:**
   - Toggles between "Access Granted" (Green LED) and "Access Denied" (Red LED + Matrix).

---

## Functions

- `displayMessage(String msg, bool clear=true)`  
  Displays a message on the OLED.
- `setRGBColor(int r, int g, int b)`  
  Sets the RGB LED color (0–255 per channel).
- `showMiddleFinger()`  
  Renders the middle finger bitmap on the LED Matrix.

---

## Notes

- The RFID module is initialized but not yet used in this revision.  
  It can be integrated to trigger API simulation instead of button presses.  
- Button uses **INPUT_PULLUP** – connect to GND when pressed.
- LED Matrix requires an Arduino board that supports the **Arduino_LED_Matrix library**.

---

## License

This project is licensed under the MIT License.
