# WiFiTroubleshooting Extended Sketch #

Denna Arduino-sketch är en utökning av originalet `WiFiTroubleshooting.ino` i NOTE-repot och lägger till stöd för  
**HW-479 RGB LED-modul**, **SSD1306 OLED-display (I2C)** och **momentan knapp**.


### 🔧 Hårdvarukrav ###
- **Arduino Uno R4 WiFi**  
- **HW-479 RGB LED-modul** (kopplas till digitala utgångar)  
- **SSD1306 OLED-display** (I2C: SDA=A4, SCL=A5)  
- **Momentan knapp** (kopplas till digital pin med intern pull-up)  

### 🧠 Funktioner ###

1. **WiFi-anslutning**  
   - Laddar SSID och lösenord från EEPROM via `WiFiEEPROM`.  
   - Försöker ansluta till det angivna WiFi-nätverket.  
   - Statusindikering via OLED och RGB LED:  
     🔵 Blinkar blå när anslutning pågår  
     🟢 Lyser grönt vid lyckad anslutning  
     🔴 Lyser rött om anslutning misslyckas  

2. **IP-adress**  
   - Visas på OLED när ansluten.  
   - RGB LED lyser grönt.  
   - Bakgrund rensas för att undvika överlappning.  

3. **WiFi-signalstyrka**  
   - Visas som staplar uppe till höger på OLED (iPhone-stil).  
   - Uppdateras kontinuerligt i `loop()`.  
   - Signalstaplar:  
     - 4: mycket stark (> -50 dBm)  
     - 3: stark (> -60 dBm)  
     - 2: medel (> -70 dBm)  
     - 1: svag (> -80 dBm)  
     - 0: mycket svag  

4. **RGB LED**  
   - Visar anslutningsstatus enligt ovan.  
   - Blinkar blått två gånger vid uppstart innan WiFi-anslutning påbörjas.  

5. **Tryckknapp**  
   - Tryck för att visa `"Knapp aktiverad"` längst ner på OLED.  
   - Meddelandet försvinner när knappen släpps.  

---

### OLED-display ###
- `"Ansluter till SSID"`  
- `"Ansluten till SSID"`  
- `"Anslutning misslyckad"`  
- WiFi-signalstyrka som staplar  
- `"Knapp aktiverad"` när knapp hålls intryckt  

---

### EEPROM ###
- Lagrar och läser SSID/lösenord.  

---

### 📦 Bibliotek ###
Installera via Library Manager eller manuellt:  
- WiFiS3  
- EEPROM  
- Adafruit_SSD1306  
- Adafruit_GFX  
- WiFiEEPROM (hantering av SSID/lösenord i EEPROM)  

---

### 📌 Pin-konfiguration ###

| Komponent      | Pin(s)           | Kommentar                        |
|----------------|-----------------|---------------------------------|
| RGB LED R/G/B  | 8 / 10 / 12      | Digitala utgångar, använd 220 Ω |
| OLED-display   | SDA=A4, SCL=A5   | I2C, Wire-biblioteket           |
| Momentan knapp | 7                | INPUT_PULLUP aktiveras          |

---

# Arduino Uno R4 WiFi – WiFi Status Display with RGB LED and Button #

Detta projekt använder **Arduino Uno R4 WiFi**, **OLED 128x64**, **RGB LED** och **tryckknapp** för att visa WiFi-status och IP-adress.

---

## Installation

1. Installera alla bibliotek via Arduino Library Manager.  
2. Sätt Arduino Uno R4 WiFi som mål i **Tools → Board**.  
3. Klistra in koden i Arduino IDE.  
4. Ladda upp till kortet.  
5. Kontrollera Serial Monitor (115200 baud) för diagnostik vid behov.  

---

## Tips

- Placera WiFi-staplar högst upp på OLED för att inte täcka IP-adressen.  
- Om OLED-storlek ändras, justera koordinater för signal-staplar.  
- Kontrollera att SSID/lösenord finns i EEPROM innan uppstart.  
