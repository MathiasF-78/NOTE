# WiFiTroubleshooting Extended Sketch #
Denna Arduino-sketch är en utökning av originalet WiFiTroubleshooting.ino i NOTE-repot som lägger till
funktioner för HW-479 RGB LED-modul, SSD1306 OLED-display (I2C) och Momentan knappmodul.

### 🔧 Hårdvarukrav ###
#### Arduino Uno R4 WiFi ####
#### HW-479 RGB LED-modul (kopplas till digitala utgångar) ####
#### SSD1306 OLED-display (I2C: SDA=A4, SCL=A5) #### 
#### Momentan knapp (kopplas till digital pin med intern pull-up) ####

### 🧠 Funktioner ###

1. **WiFi-anslutning**
   - Laddar SSID och lösenord från EEPROM via `WiFiEEPROM`.
   - Försöker ansluta till det angivna WiFi-nätverket.
   - Visar anslutningsstatus på OLED och via RGB LED:
        🔵 Blinkar blå när anslutning försöks  
        🟢 Lyser grönt vid lyckad anslutning  
        🔴 Lyser rött om anslutning misslyckas.
     

2. **IP-adress**
   - När ansluten, visas den lokala IP-adressen på OLED-skärmen.
   - Rensar bakgrund för att undvika överlappning med andra element.

3. **WiFi-signalstyrka**
   - Visas som staplar uppe till höger på OLED, inspirerat av iPhone-stil.
   - Uppdateras kontinuerligt i `loop()` när WiFi är anslutet.
   - Staplarnas höjd representerar signalstyrka:
     - 4 staplar: mycket stark signal (> -50 dBm)
     - 3 staplar: stark signal (> -60 dBm)
     - 2 staplar: medel (> -70 dBm)
     - 1 stapel: svag (> -80 dBm)
     - 0 staplar: mycket svag

4. **RGB LED**
   - Visar anslutningsstatus enligt ovan.
   - Blinkar blått två gånger vid uppstart innan WiFi-anslutning påbörjas.

5. **Tryckknapp**
   - Tryck på knappen för att visa meddelandet `"Knapp aktiverad"` längst ner på OLED.
   - Meddelandet försvinner när knappen släpps.
   - 
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



# Arduino Uno R4 WiFi – WiFi Status Display with RGB LED and Button

Detta projekt använder en **Arduino Uno R4 WiFi** tillsammans med en **OLED 128x64-skärm**, en **RGB LED** och en **tryckknapp** för att visa WiFi-status och IP-adress.

---

## Funktioner



---

## Kopplingar

| Komponent      | Arduino-pin |
|----------------|------------|
| RGB LED R      | 8          |
| RGB LED G      | 10         |
| RGB LED B      | 12         |
| Knapp          | 7 (INPUT_PULLUP) |
| OLED SDA       | A4 / Wire  |
| OLED SCL       | A5 / Wire  |

> Obs: Använd 220 Ω motstånd på varje kanal för RGB LED.

---

## Bibliotek som används

- [WiFiS3](https://www.arduino.cc/en/Reference/WiFiS3)
- [EEPROM](https://www.arduino.cc/en/Reference/EEPROM)
- [Adafruit GFX](https://github.com/adafruit/Adafruit-GFX-Library)
- [Adafruit SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [WiFiEEPROM](https://github.com/your-library-link) – hantering av SSID/lösenord i EEPROM

---

## Installation

1. Installera alla ovanstående bibliotek via Arduino Library Manager.
2. Sätt upp Arduino Uno R4 WiFi som mål i **Tools → Board**.
3. Klistra in koden i Arduino IDE.
4. Ladda upp till kortet.
5. Kontrollera Serial Monitor (115200 baud) för diagnostik vid behov.

---

## Tips

- Placera WiFi-staplarna högst upp på OLED för att inte täcka IP-adressen.
- Om du ändrar OLED-storlek, justera koordinater för signal-staplar.
- Se till att SSID och lösenord finns korrekt i EEPROM innan uppstart.

---

## Licens

Detta projekt är open-source. Du får använda, modifiera och distribuera koden fritt.

