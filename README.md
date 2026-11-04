# 🌱 IoT Smart Plant

Σύστημα αυτόματου ποτίσματος και παρακολούθησης φυτού με **ESP32-S3**. Μετράει την υγρασία του χώματος, τη θερμοκρασία και την υγρασία του αέρα, στέλνει τις τιμές μέσω **MQTT** και ενεργοποιεί αντλία νερού (μέσω relay) όταν το χώμα στεγνώσει.

![Platform](https://img.shields.io/badge/platform-ESP32--S3-blue)
![Framework](https://img.shields.io/badge/framework-Arduino-00979D)
![Build](https://img.shields.io/badge/build-PlatformIO-orange)
![Protocol](https://img.shields.io/badge/protocol-MQTT-660066)

---

## ✨ Λειτουργίες

- 💧 **Μέτρηση υγρασίας χώματος** με αναλογικό αισθητήρα, σε ποσοστό 0–100%
- 🌡️ **Θερμοκρασία & υγρασία αέρα** με αισθητήρα DHT11
- 🚿 **Αυτόματο πότισμα**: όταν η υγρασία χώματος πέσει στο 15% ή χαμηλότερα, το relay ανοίγει την αντλία για 6 δευτερόλεπτα
- 📡 **Αποστολή δεδομένων μέσω MQTT** κάθε ~5 δευτερόλεπτα προς Node-RED
- 🔁 **Αυτόματη επανασύνδεση** στον MQTT broker και στο Wi-Fi αν χαθεί η σύνδεση
- 🖥️ **Serial monitor** με όλες τις μετρήσεις για debugging

---

## 🧰 Υλικά (Hardware)

| Εξάρτημα | Περιγραφή |
|---|---|
| ESP32-S3-DevKitC-1 | Ο μικροελεγκτής |
| DHT11 | Αισθητήρας θερμοκρασίας & υγρασίας δωματίου |
| Keyestudio | Αισθητήρας υγρασίας χώματος |
| Relay module (5V)| Για τον έλεγχο της αντλίας |
| Μικρή αντλία νερού (5V) | Με το κατάλληλο τροφοδοτικό | |

---

## 🔌 Συνδεσμολογία

| Εξάρτημα | Pin ESP32-S3 |
|---|---|
| DHT11 – Data | `GPIO 11` |
| Αισθητήρας χώματος – Analog out | `GPIO 1` (ADC) |
| Relay – IN | `GPIO 14` & `NO` |
| VCC / GND | 3.3V & 5V / GND |

> ⚠️ Η αντλία πρέπει να τροφοδοτείται από **ξεχωριστή πηγή** μέσω του relay, όχι απευθείας από το ESP32.
>
> ##Fritzing
<img width="1273" height="859" alt="Screenshot_20261104_151728" src="https://github.com/user-attachments/assets/40ab1935-e065-4c25-9adf-7cce7d779e2a" />

---

## 📡 MQTT Topics

| Topic | Κατεύθυνση |
|---|---|
| `temp` | ESP32 → Broker |
| `hum` | ESP32 → Broker |
| `Soil` | ESP32 → Broker |
| `esp/output` | Broker → ESP32 |

---

## 🚀 Εγκατάσταση & Εκτέλεση

### Προαπαιτούμενα
- [VS Code](https://code.visualstudio.com/) με την επέκταση [PlatformIO IDE](https://platformio.org/install/ide?install=vscode)
- Ένας MQTT broker στο ίδιο δίκτυο (π.χ. [Mosquitto](https://mosquitto.org/))

### Βήματα

1. **Κλωνοποίηση του repository**
   ```bash
   git clone https://github.com/<username>/IoT-Smart-Plant.git
   cd IoT-Smart-Plant
   ```

2. **Ρύθμιση WiFi και MQTT** στο `src/main.cpp`:
   ```cpp
   const char* ssid        = "YOUR_WIFI_SSID";
   const char* password    = "YOUR_WIFI_PASSWORD";
   const char* mqtt_server = "192.168.x.x";   // IP του MQTT broker
   const int   mqtt_port   = 1883;
   ```

3. **Ρύθμιση θύρας upload** στο `platformio.ini` αν χρειάζεται:
   ```ini
   upload_port = /dev/ttyACM0   ; Linux
   ; upload_port = COM3         ; Windows
   ```

4. **Build & Upload**
   ```bash
   pio run --target upload
   ```

5. **Serial Monitor** (115200 baud)
   ```bash
   pio device monitor
   ```

Οι βιβλιοθήκες κατεβαίνουν αυτόματα από το PlatformIO:
- [`adafruit/DHT sensor library`](https://github.com/adafruit/DHT-sensor-library) `^1.4.6`
- [`knolleary/PubSubClient`](https://github.com/knolleary/pubsubclient) `^2.8`

---

## ⚙️ Βαθμονόμηση αισθητήρα χώματος

Οι ακραίες τιμές του αισθητήρα ορίζονται στο `loop()`:

```cpp
int dry_valeu  // ένδειξη ADC όταν το χώμα είναι στεγνό
int wet_valeu  // ένδειξη ADC όταν το χώμα είναι πολύ υγρό
```

Για να τις προσαρμόσεις στον δικό σου αισθητήρα:
1. Διάβασε την τιμή `analogRead` με τον αισθητήρα στον αέρα / σε στεγνό χώμα.
2. Διάβασε την τιμή με τον αισθητήρα σε νερό / πολύ υγρό χώμα.
3. Βάλε αυτές τις τιμές στις παραπάνω μεταβλητές.

Το όριο ποτίσματος (15%) και η διάρκεια (6 s) αλλάζουν στο μπλοκ `RELAY` του `main.cpp`.

---

## 📁 Δομή Project

```
IoT-Smart-Plant/
├── src/
│   └── main.cpp        # Κύριος κώδικας (αισθητήρες, WiFi, MQTT, relay)
├── include/            # Header files
├── lib/                # Τοπικές βιβλιοθήκες
├── test/               # Unit tests
├── platformio.ini      # Ρυθμίσεις PlatformIO (board, βιβλιοθήκες, θύρα)
└── README.md
