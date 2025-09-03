# WiFiTroubleshooting Extended Sketch #
Denna Arduino-sketch är en utökning av originalet WiFiTroubleshooting.ino i NOTE-repot som lägger till
funktioner för HW-479 RGB LED-modul, SSD1306 OLED-display (I2C) och Momentan knappmodul.

### 🔧 Hårdvarukrav ###
#### Arduino Uno R4 WiFi ####
#### HW-479 RGB LED-modul (kopplas till digitala utgångar) ####
#### SSD1306 OLED-display (I2C: SDA=A4, SCL=A5) #### 
#### Momentan knapp (kopplas till digital pin med intern pull-up) ####

### 🧠 Funktioner ###

RGB LED-status:  
🔵 Blinkar blå när anslutning försöks  
🟢 Lyser grönt vid lyckad anslutning  
🔴 Lyser rött om anslutning misslyckas  

OLED-display:  
Visar: "Ansluter till SSID"  
Visar: "Ansluten till SSID"  
Visar: "Anslutning misslyckad"  
Visar WiFi-signalstyrka som staplar  
Visar: "Knapp aktiverad" när knapp hålls intryckt  

EEPROM:  
Läser SSID och lösenord från EEPROM  
📦 Bibliotek som används  

Installera via Library Manager eller manuellt:  
WiFiS3.h  
EEPROM.h  
Adafruit_SSD1306  
Adafruit_GFX  

📌 Pin-konfiguration  
Komponent	Pin(s)	Kommentar  
RGB LED-modul	RED_PIN, GREEN_PIN, BLUE_PIN	Digitala utgångar  
OLED-display	I2C: SDA=A4, SCL=A5	Använder Wire-biblioteket  
Momentan knapp	BUTTON_PIN	Intern pull-up aktiveras  
