# allarme-camper
# Allarme Camper ESP32 + Flutter

Sistema di allarme per camper basato su ESP32, accelerometro ADXL335 e comando Bluetooth da app Flutter.

## Funzionalità
- Rilevazione movimento tramite accelerometro
- Attivazione clacson tramite relè
- **Display TFT con messaggi di stato**
- **LED RGB con feedback visivo**
- Gestione da app Flutter via Bluetooth

### Stati del display:
- `ALLARME CAMPER NON CONNESSO` - quando Bluetooth non è connesso
- `ALLARME CONNESSO` - quando Bluetooth è connesso ma allarme è disattivo  
- `ALLARME INSERITO` - quando l'allarme è attivo

### Stati LED RGB:
- **SPENTO** - quando Bluetooth non è connesso
- **VERDE** - quando Bluetooth è connesso
- **ROSSO lampeggiante** (ogni secondo) - quando l'allarme è inserito

## Componenti hardware
- ESP32 NodeMCU USB-C
- ADXL335 (GY-61)
- **Display TFT compatibile TFT_eSPI** (es. ILI9341, ST7735)
- **LED RGB** (sostituisce LED rosso/verde separati)
- Modulo relè 5V con optoisolatore
- Clacson camper

## Software
- [main.dart](lib/main.dart) — App Flutter
- [allarme_camper.ino](allarme_camper.ino) — Firmware ESP32 con display TFT e RGB LED
- [schema_collegamenti.md](schema_collegamenti.md) — Schema collegamenti
- [TFT_SETUP.md](TFT_SETUP.md) — Guida configurazione display TFT

## Collegamenti e schema elettrico
Vedi [schema_collegamenti.md](schema_collegamenti.md)

## Come usare
1. Carica il firmware su ESP32
2. Collega i componenti come da schema
3. Avvia l’app Flutter, accoppia via Bluetooth
4. Controlla l’allarme dal telefono

## Licenza
MIT

## Setup addizionale per display TFT
- Installa la libreria TFT_eSPI nell'IDE Arduino
- Configura il display TFT seguendo la guida [TFT_SETUP.md](TFT_SETUP.md)
- I pin del display sono configurabili nella libreria TFT_eSPI
