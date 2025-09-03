# Raspberry Pi 5 + Luxonis OAK-1 + GS1 QR Reader

### ❗Detta är inte testat då hårdvara ännu ej finns på plats. ❗
### ❗Detta är en förkompilerad kod att användas som grund för att snabbt komma igång med tester. ❗


Projektet visar hur man sätter upp en **Raspberry Pi 5 (4GB)** tillsammans med en **Luxonis OAK-1 kamera** och en **7" Raspberry Pi Touchscreen** för att:
- Visa live-bild från kameran på skärmen  
- Läsa QR-koder (inklusive GS1 DataMatrix/QR)  
- Dekryptera GS1-koder enligt Application Identifiers (AI)  
- Visualisera data direkt på skärmen  

---

## 🛠️ Hårdvara
- Raspberry Pi 5 (4GB)  
- Raspberry Pi 7" Touchscreen  
- Luxonis OAK-1 USB AI Camera  

---

## 📦 Installation

### 1. Raspberry Pi OS
1. Installera **Raspberry Pi OS (64-bit, med desktop)** med [Raspberry Pi Imager](https://www.raspberrypi.com/software/).  
2. Kör systemuppdatering:
   ```bash
   sudo apt update && sudo apt upgrade -y
   ```

### 2. DepthAI (Luxonis SDK)
   ```bash
   sudo apt install python3-pip libatlas-base-dev -y
   python3 -m pip install depthai opencv-python
   ```

Testa kameran:
  ```bash
  git clone https://github.com/luxonis/depthai-python.git
  cd depthai-python/examples
  python3 color_preview.py
  ```

### 3. QR-kod bibliotek
```bash
python3 -m pip install pyzbar pillow
```

### Exempel: QR Reader med GS1-dekodning
   ```bash
import cv2
import depthai as dai
from pyzbar.pyzbar import decode
import re

# GS1 parser
def parse_gs1(data):
    ai_patterns = {
        "01": ("GTIN", 14),
        "10": ("Batch", None),
        "17": ("BestBefore", 6),
        "21": ("Serial", None),
    }
    result = {}
    i = 0
    while i < len(data):
        ai = data[i:i+2]
        if ai in ai_patterns:
            label, fixed_len = ai_patterns[ai]
            i += 2
            if fixed_len:
                value = data[i:i+fixed_len]
                i += fixed_len
            else:
                m = re.search(r"\x1D", data[i:])
                if m:
                    value = data[i:i+m.start()]
                    i += m.start()+1
                else:
                    value = data[i:]
                    i = len(data)
            result[label] = value
        else:
            i += 1
    return result
   ```

# DepthAI pipeline
   ```bash
pipeline = dai.Pipeline()
cam = pipeline.create(dai.node.ColorCamera)
cam.setPreviewSize(640, 480)
cam.setInterleaved(False)
xout = pipeline.create(dai.node.XLinkOut)
xout.setStreamName("video")
cam.preview.link(xout.input)

with dai.Device(pipeline) as device:
    q = device.getOutputQueue(name="video", maxSize=4, blocking=False)

    while True:
        frame = q.get().getCvFrame()
        decoded = decode(frame)

        for obj in decoded:
            data = obj.data.decode("utf-8")
            pts = obj.polygon
            cv2.polylines(frame, [pts], True, (0,255,0), 2)

            parsed = parse_gs1(data)
            y = 30
            for k,v in parsed.items():
                cv2.putText(frame, f"{k}: {v}", (10, y),
                            cv2.FONT_HERSHEY_SIMPLEX, 0.7, (255,255,0), 2)
                y += 30

        cv2.imshow("QR Reader", frame)
        if cv2.waitKey(1) == ord('q'):
            break
   ```

#### Funktioner
- Live video från OAK-1 → Raspberry Pi 7" touchscreen
- QR-kod detektering med pyzbar
- GS1 parser för Application Identifiers (GTIN, batch, BBD, serienummer)
- Overlay av dekrypterad data på bildström

### Kör
   ```bash
python3 gs1_qr_reader.py
   ```
Avsluta med "Q"


📌 TODO
- Lägg till stöd för fler GS1 Application Identifiers
- Export av data till fil/databas/REST API
- Integrering mot MES/ERP (t.ex. IFS Cloud)

📜 Licens
MIT License – fritt att använda och vidareutveckla.
