# AX Multipicking Companion

## 🧠 Vad är detta?

Här har jag byggt ett Python-program med grafiskt gränssnitt (GUI) i Tkinter som jag kallar **AX Multipicking Companion**. Syftet är att hjälpa mig och mina kollegor att hämta och hantera plocklistor från arbetsorder i AX-systemet på ett smidigt sätt.

Jag skapade detta projekt som en del av min egen lärresa i Python – för att förstå hur man bygger riktiga applikationer med databasanslutning, GUI, export till Excel och utskrift. Det har varit ett sätt för mig att kombinera nytta med lärande.

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

1. Jag matar in arbetsorder i fälten.
2. Trycker på **Fetch Data** – då körs en SQL-fråga mot AX-databasen.
3. Resultatet visas i en tabell.
4. Jag kan exportera till Excel eller skriva ut direkt.
5. Jag kan ändra Company Code via menyn **Inställningar**.

---

## 🧪 Teknisk översikt

- **GUI:** Tkinter
- **Databas:** SQL Server via SQLAlchemy
- **Excel-export:** openpyxl
- **Utskrift:** `os.startfile()` (Windows)
- **Trådning:** `threading` för att inte frysa GUI
- **Kommentarer i Excel:** `openpyxl.comments.Comment`

---

## 🚀 Hur kör jag programmet?

1. Se till att du har **Python 3.10+** installerat.
2. Installera beroenden:

```bash
pip install pandas sqlalchemy pyodbc openpyxl
