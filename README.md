# allarme-camper
# Allarme Camper ESP32 + Flutter

Sistema di allarme per camper basato su ESP32, accelerometro ADXL335 e comando Bluetooth da app Flutter.

## Funzionalità
- Rilevazione movimento tramite accelerometro
- Attivazione clacson tramite relè
- Stato allarme segnalato con LED
- Gestione da app Flutter via Bluetooth

## Componenti hardware
- ESP32 NodeMCU USB-C
- ADXL335 (GY-61)
- Modulo relè 5V con optoisolatore
- LED rosso/verde + resistenze
- Clacson camper

## Software
- [main.dart](lib/main.dart) — App Flutter
- [allarme_camper.ino](arduino/allarme_camper.ino) — Firmware ESP32
- [schema_collegamenti.md](schema_collegamenti.md) — Schema collegamenti

## Collegamenti e schema elettrico
Vedi [schema_collegamenti.md](schema_collegamenti.md)

## Come usare
1. Carica il firmware su ESP32
2. Collega i componenti come da schema
3. Avvia l’app Flutter, accoppia via Bluetooth
4. Controlla l’allarme dal telefono

## Licenza
MIT
