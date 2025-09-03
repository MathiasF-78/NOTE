# Arduino RFID Access Control with OLED and RGB LED
## Version 1

This project implements a simple access control system using an Arduino, RFID reader, RGB LED, push button, and OLED display. It can be used as a prototype for integrating RFID-based access with visual feedback.

## Features

- Reads RFID tags using the MFRC522 module.
- Displays status messages on a 128x64 OLED display.
- RGB LED indicates system status:
  - Blue: Standby
  - Green: Access Granted
  - Red: Access Denied
- Push button toggles access state manually.
- Simulated communication with an external system (e.g., IFS) when a tag is detected.

## Hardware Requirements

- Arduino board (e.g., Uno, Mega)
- MFRC522 RFID reader
- OLED display (128x64, I2C)
- RGB LED (common cathode)
- Push button
- Connecting wires and resistors

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

## Libraries Required

- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [MFRC522](https://github.com/miguelbalboa/rfid)
- SPI (built-in Arduino)
- Wire (built-in Arduino)
- WiFiEEPROM (optional, for saving data persistently)

## Usage

1. Connect all hardware according to the pin configuration.
2. Install required libraries in Arduino IDE.
3. Upload the code to your Arduino.
4. Open Serial Monitor at 115200 baud to see debug messages.
5. The system starts in standby (blue LED).
6. Press the button to toggle access manually.
7. Present an RFID tag to the reader to simulate system communication. Access will be denied by default.

## Functions

- `displayMessage(String msg, bool clear=true)` – Displays a message on the OLED.
- `setRGBColor(int r, int g, int b)` – Sets the RGB LED color.
- `handleButton()` – Handles push button presses to toggle access.
- `handleRFID()` – Handles RFID tag detection and simulates communication with external systems.

## Notes

- The RFID communication with IFS is simulated with a delay and a message display.
- Ensure proper wiring of the RGB LED (common cathode) and OLED display.
- The button uses `INPUT_PULLUP`, so connect one side to GND.

## License

This project is licensed under the MIT License.
