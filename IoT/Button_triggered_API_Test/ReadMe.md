📡 Arduino WiFi Button API Trigger
Projektbeskrivning
Detta projekt är ett enkelt men kraftfullt exempel på hur en Arduino UNO R4 WiFi kan användas för att:

Läsa WiFi-credentials från EEPROM
Ansluta till ett WiFi-nätverk
Lyssna på knapptryckningar
Skicka ett HTTP POST-anrop till ett API när knappen trycks
Projektet är tänkt som en grund för framtida IoT-lösningar, där en fysisk händelse (t.ex. en knapptryckning) triggar en digital process via nätverk.

🧩 Komponenter
Arduino UNO R4 WiFi
Knapp kopplad till pin 2
EEPROM (inbyggd)
WiFiEEPROM-bibliotek (egen implementation)
Flask-server som tar emot API-anrop
🧠 Designprinciper
1. Modulär hantering av WiFi-credentials
För att undvika hårdkodade SSID/lösenord i sketch-filen används ett eget bibliotek (WiFiEEPROM) som läser och skriver credentials till EEPROM. Detta gör det enkelt att återanvända koden i andra projekt och att uppdatera credentials utan att ändra sketch.

2. Responsiv knapphantering
Knappen är kopplad med INPUT_PULLUP och kontrolleras med en enkel edge-detektion (fallande flank) för att undvika upprepade anrop.

3. HTTP POST till lokal Flask-server
Vid knapptryckning skickas ett JSON-anrop till en lokal server som körs på t.ex. en laptop eller Raspberry Pi. Detta kan enkelt utökas till att skicka data till molnet eller en produktionsserver.

📁 Filstruktur

🔧 Funktionalitet
WiFiEEPROM.h/.cpp
readWiFiCredentials() – Läser SSID och lösenord från EEPROM
writeWiFiCredentials(ssid, password) – Skriver credentials till EEPROM
getSSID() / getPassword() – Returnerar credentials som const char*
setupWiFi() – Ansluter till WiFi och skriver ut IP-adress
sketch.ino
Läser credentials via getSSID() och getPassword()
Ansluter till WiFi
Lyssnar på knapptryckning
Skickar POST-anrop med JSON-payload {"status":"press"}
server.py
Flask-server som lyssnar på /api/button
Skriver ut mottagen payload till konsolen
🧪 Testinstruktioner
Skriv credentials till EEPROM (kan göras via separat sketch eller funktion)
Starta Flask-servern:

Ladda upp sketch.ino till Arduino
Tryck på knappen – kontrollera att servern skriver ut:
Mottagen payload: {'status': 'press'}
🛠️ Möjliga förbättringar
Debounce-logik för knappen
Visuell feedback via LED eller OLED-display
Stöd för flera API-endpoints
Kryptering av credentials i EEPROM
Automatisk fallback till konfigurationsläge om WiFi misslyckas
