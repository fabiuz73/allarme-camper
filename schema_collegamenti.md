# Schema dettagliato collegamenti - Allarme Camper ESP32

## Componenti principali
- **Scheda ESP32 USB-C NodeMCU**
- **Accelerometro ADXL335 (GY-61)**
- **Display TFT compatibile con TFT_eSPI** (es. ILI9341, ST7735)
- **LED RGB** (al posto dei singoli LED verde/rosso)
- **Modulo Relè** (per attivare clacson)
- **Clacson camper** (collegato tramite relè)

## Pin ESP32 (aggiornato per display TFT e RGB LED)
- RGB LED Red: GPIO 12
- RGB LED Green: GPIO 13  
- RGB LED Blue: GPIO 14
- Relè clacson: GPIO 27
- Display TFT: utilizza pin SPI standard (configurabili in TFT_eSPI)
- ADXL335 X: GPIO 34 (ingresso analogico)
- ADXL335 Y: GPIO 35 (ingresso analogico)
- ADXL335 Z: GPIO 32 (ingresso analogico)

---

## Schema elettrico semplificato

```
            +----------------------+
            |      ESP32           |
            |    USB-C NodeMCU     |
            +----------------------+
         3.3V o VCC ---+---------+---------+---------+
                       |         |         |         |
                 [ADXL VCC]  [TFT VCC] [RGB VCC]     |
                       |         |         |         |
                GPIO 34 (X)      |         |         |
                GPIO 35 (Y)      |         |         |
                GPIO 32 (Z)      |    [TFT SPI]     |
                       |         |         |         |
                    [ADXL GND]---+---------+---------+
                       |
                  [ADXL X/Y/Z out]---GPIO 34/35/32 (analog IN)
                       |
                   [ADXL GND]---GND ESP32
                       
     GPIO 12 ------------> RGB LED Red pin
     GPIO 13 ------------> RGB LED Green pin  
     GPIO 14 ------------> RGB LED Blue pin
     RGB LED GND --------> GND ESP32
     GPIO 27 ------------> IN modulo relè
     Relè VCC/GND -------> 5V/GND
     Relè NO/COM --------> Clacson (collegamento come da schema clacson camper)
     TFT Display --------> Pin SPI standard (vedere configurazione TFT_eSPI)

```

## Note
- **Resistenze per LED:** tipicamente 220Ω in serie.
- **Relè:** modulo a 5V, compatibile con ESP32, deve supportare la corrente del clacson.
- **ADXL335:** alimenta a 3.3V!  
- Collega gli OUT X/Y/Z dell’ADXL335 ai pin analogici dell’ESP32.
- Scegli i pin giusti e modificali nel file `.ino` se necessario.

## Consigli
- Alimenta l’ESP32 via USB-C durante i test.
- Puoi usare una breadboard per collegamenti temporanei.
- Usa morsetti per il clacson (occhio alla corrente!).