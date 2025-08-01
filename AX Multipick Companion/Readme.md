# AX Multipicking Companion

## 🧠 Vad är detta?

Ett Python-program med grafiskt gränssnitt (GUI) i Tkinter. Syftet är att hämta och hantera plocklistor från arbetsorder i Microsoft Dynamics AX-systemet på ett smidigt sätt.

---

## ⚙️ Funktioner

- ✅ Inmatning av upp till 10 arbetsorder
- ✅ Automatisk hämtning av data från SQL Server via SQLAlchemy
- ✅ Visning av resultat i en tabell med färgade rader (Treeview)
- ✅ Export till formaterad Excel-fil med radbrytningar, färger och kommentarer
- ✅ Direkt utskrift av Excel-filen
- ✅ Inställningsfönster med rullista för att välja Company Code
- ✅ Statuslogg som visar vad som händer i bakgrunden

---

## 🏗️ Hur fungerar det?

1. Användare matar in arbetsorder i fälten.
2. Användare trycker på **Fetch Data** – då körs en SQL-fråga mot AX-databasen.
3. Resultatet visas i en tabell.
4. Användaren kan exportera till Excel eller skriva ut direkt.
5. Användare kan även ändra Company Code via menyn **Inställningar**, om det skulle vara önskvärt.

---

## 🧪 Teknisk översikt

- **GUI:** Tkinter
- **Databas:** SQL Server via SQLAlchemy
- **Excel-export:** openpyxl
- **Utskrift:** `os.startfile()` (Windows)
- **Trådning:** `threading` för att inte frysa GUI
- **Kommentarer i Excel:** `openpyxl.comments.Comment`

---

## 🚀 Hur körs programmet?

1. Se till att du har **Python 3.10+** installerat.
2. Installera beroenden:

```bash
pip install pandas sqlalchemy pyodbc openpyxl
```

3. Kör programmet:

```bash
python Retreive_Pickinglists_from_WO_number.pyfinal.py
```

---

## 🛠️ Bygga EXE med PyInstaller

För att skapa en körbar version (ej onefile) med ikon:

```bash
pyinstaller --name "AX Multipicking Companion" --icon=table-icon.ico --noconfirm --windowed Retreive_Pickinglists_from_WO_number.pyfinal.py
```

---

## 📚 Vad jag lärt mig

- Hur man skapar ett GUI med Tkinter
- Hur man använder SQLAlchemy för att prata med en riktig databas
- Hur man exporterar data snyggt till Excel
- Hur man hanterar trådning i GUI-appar
- Hur man skapar en användarvänlig upplevelse med menyer och inställningar
