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
- 📡 **Αποστολή δεδομένων μέσω MQTT** κάθε ~5 δευτερόλεπτα (π.χ. προς Node-RED, Home Assistant, Grafana)
- 🔁 **Αυτόματη επανασύνδεση** στον MQTT broker αν χαθεί η σύνδεση
- 🖥️ **Serial monitor** με όλες τις μετρήσεις για debugging

---

## 🧰 Υλικά (Hardware)

| Εξάρτημα | Περιγραφή |
|---|---|
| ESP32-S3-DevKitC-1 | Ο μικροελεγκτής |
| DHT11 | Αισθητήρας θερμοκρασίας & υγρασίας αέρα |
| Αισθητήρας υγρασίας χώματος | Αναλογικός (capacitive ή resistive) |
| Relay module | Για τον έλεγχο της αντλίας |
| Μικρή αντλία νερού (5V/12V) | Με το κατάλληλο τροφοδοτικό |
| Καλώδια, breadboard | |

---

## 🔌 Συνδεσμολογία

| Εξάρτημα | Pin ESP32-S3 |
|---|---|
| DHT11 – Data | `GPIO 11` |
| Αισθητήρας χώματος – Analog out | `GPIO 1` (ADC) |
| Relay – IN | `GPIO 14` |
| VCC / GND | 3.3V (ή 5V ανάλογα με το module) / GND |

> ⚠️ Η αντλία πρέπει να τροφοδοτείται από **ξεχωριστή πηγή** μέσω του relay, όχι απευθείας από το ESP32.

---

## 📡 MQTT Topics

| Topic | Κατεύθυνση | Περιεχόμενο |
|---|---|---|
| `temp` | ESP32 → Broker | Θερμοκρασία σε °C |
| `hum` | ESP32 → Broker | Υγρασία αέρα σε % |
| `Soil` | ESP32 → Broker | Υγρασία χώματος σε % |
| `esp/output` | Broker → ESP32 | Μηνύματα προς τη συσκευή (προς το παρόν απλώς τυπώνονται στο Serial) |

Παράδειγμα για να δεις τα δεδομένα από υπολογιστή με Mosquitto:

```bash
mosquitto_sub -h <IP_του_broker> -t "#" -v
```

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
int dry_valeu = 1500;  // ένδειξη ADC όταν το χώμα είναι στεγνό
int wet_valeu = 4095;  // ένδειξη ADC όταν το χώμα είναι πολύ υγρό
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
```

---

## 🛠️ Πιθανά προβλήματα

| Πρόβλημα | Λύση |
|---|---|
| `Error data reading` στο Serial | Έλεγξε τη σύνδεση του DHT11 στο GPIO 11 και ότι καλείται `dht.begin()` στο `setup()` |
| `failed, rc= -2` | Ο MQTT broker δεν είναι προσβάσιμος: έλεγξε IP, θύρα και ότι είστε στο ίδιο δίκτυο |
| Κολλάει στο `Connecting to WiFi ..` | Λάθος SSID/κωδικός ή δίκτυο 5 GHz (το ESP32 υποστηρίζει μόνο 2.4 GHz) |
| Η υγρασία χώματος δείχνει πάντα 0% ή 100% | Χρειάζεται βαθμονόμηση (δες παραπάνω) |

---

## 🗺️ Μελλοντικές βελτιώσεις

- [ ] Χειροκίνητος έλεγχος αντλίας μέσω του topic `esp/output`
- [ ] Dashboard σε Node-RED / Grafana
- [ ] Αποθήκευση credentials σε ξεχωριστό αρχείο εκτός Git
- [ ] Deep sleep για εξοικονόμηση ενέργειας
- [ ] Ειδοποιήσεις όταν τελειώνει το νερό

---

## 📄 Άδεια

Πρόσθεσε εδώ την άδεια χρήσης του project (π.χ. MIT).
