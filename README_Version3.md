# Allarme Camper ESP32

Sistema di allarme per camper basato su **ESP32** con display LCD da 1,14 pollici e controllo via Bluetooth tramite app Flutter.

## Hardware utilizzato

- **Scheda ESP32 di sviluppo**
  - Modello: ESP32 con chip CH340 USB-C
  - WiFi 2.4 GHz e Bluetooth integrato
- **Display LCD**: 1,14 pollici (ST7789, 240x320 pixel)
- **Accelerometro**: ADXL335 (GY-61)
- **Relè**: per attivazione clacson
- **LED**: Verde e rosso per stato allarme (**da sostituire con LED RGB KY-016 FZ0455**)
- **LED RGB**: KY-016 FZ0455 (per stato allarme, vedi miglioramento futuro)

## Collegamenti principali

| ESP32 Pin      | Collegamento      | Descrizione               |
| -------------- | ---------------- | ------------------------- |
| 12             | LED Verde        | Allarme disinserito       |
| 13             | LED Rosso        | Allarme inserito          |
| 14             | Relè clacson     | Attivazione clacson       |
| 16             | Pulsante fisico  | Inserisci allarme (INPUT_PULLUP) |
| 17             | Pulsante fisico  | Disinserisci allarme (INPUT_PULLUP) |
| 34             | ADXL335 X        | Movimento asse X          |
| 35             | ADXL335 Y        | Movimento asse Y          |
| 32             | ADXL335 Z / TFT BL | Movimento asse Z / Retroilluminazione display |
| 15             | TFT CS           | Display ST7789 Chip Select|
| 2              | TFT DC           | Display ST7789 Data/Command|
| 4              | TFT RST          | Display ST7789 Reset      |
| 32             | TFT BL           | Display ST7789 Retroilluminazione |
| ---            | LED RGB KY-016   | Da collegare a 3 pin PWM per stato allarme |

*Nota: Alcuni pin sono condivisi tra display e accelerometro, verificare eventuali conflitti.*

## Funzionalità

- Visualizzazione stato allarme su display
- Modifica soglia di movimento tramite app Bluetooth
- **Controllo fisico con pulsanti: inserimento/disinserimento allarme tramite pulsanti su GPIO16 e GPIO17**
- Inserimento/disinserimento allarme e test clacson
- Rilevamento movimento tramite accelerometro
- Memorizzazione e visualizzazione soglia attuale
- **Debounce software per prevenire attivazioni accidentali dei pulsanti**

## App Flutter

L’app permette di:
- Connettersi via Bluetooth all’ESP32
- Modificare la soglia di movimento
- Inserire/disinserire l’allarme
- Attivare test clacson
- Visualizzare la soglia attuale e messaggi dallo stato del sistema

## Schema di collegamento

```
ESP32         Accelerometro GY-61
  34   <-->   X
  35   <-->   Y
  32   <-->   Z

ESP32         Display ST7789
  15   <-->   CS
   2   <-->   DC
   4   <-->   RST
  32   <-->   BL

ESP32         LED
  12   <-->   Verde
  13   <-->   Rosso

ESP32         Pulsanti fisici
  16   <-->   Pulsante inserisci allarme (con resistenza pull-up)
  17   <-->   Pulsante disinserisci allarme (con resistenza pull-up)

ESP32         LED RGB KY-016 FZ0455
  D1   <-->   Rosso (PWM)
  D2   <-->   Verde (PWM)
  D3   <-->   Blu (PWM)

ESP32         Relè clacson
  14   <-->   IN relè
```

## Modulo Bluetooth

La comunicazione Bluetooth utilizza la libreria `BluetoothSerial` dell’ESP32.
L’app invia comandi come:
- `SOGLIA:<valore>` per impostare la soglia
- `GET_SOGLIA` per leggere la soglia attuale
- `1` per inserire l’allarme
- `0` per disinserire l’allarme
- `T` per test clacson

## Alimentazione

- La scheda ESP32 può essere alimentata tramite USB-C.
- Il relè e il clacson devono essere alimentati separatamente secondo la specifica del relè.

## Task futuri / Miglioramenti

- **Migliorare la grafica della app Flutter**
- **Verificare e ottimizzare la logica e la funzionalità**
- **Sostituire i LED rosso e verde con LED RGB KY-016 FZ0455** per una segnalazione visiva più chiara e personalizzabile

## Note aggiuntive

- Utilizza cavi dupont per i collegamenti.
- Verifica la compatibilità del display ST7789 con la libreria Adafruit.
- La soglia di movimento è modificabile tramite app e viene memorizzata in RAM (non persistente dopo il riavvio).

## Credits

Progetto sviluppato da [fabiuz73](https://github.com/fabiuz73).
