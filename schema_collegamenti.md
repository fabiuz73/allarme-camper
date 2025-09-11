# Schema dettagliato collegamenti - Allarme Camper ESP32

## Componenti principali
- **Scheda ESP32 USB-C NodeMCU**
- **Accelerometro ADXL335 (GY-61)**
- **LED Verde** (allarme disinserito)
- **LED Rosso** (allarme inserito, lampeggiante)
- **Modulo Relè** (per attivare clacson)
- **Clacson camper** (collegato tramite relè)

## Pin ESP32 (puoi cambiare se vuoi)
- LED verde: GPIO 12
- LED rosso: GPIO 13
- Relè clacson: GPIO 14
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
                       |         |         |
                 [ADXL VCC]  [LED verde] [LED rosso]
                       |         |         |
                GPIO 34 (X)      |         |
                GPIO 35 (Y)      |         |
                GPIO 32 (Z)      |         |
                       |         |         |
                    [ADXL GND]---+---------+---------+
                       |
                  [ADXL X/Y/Z out]---GPIO 34/35/32 (analog IN)
                       |
                   [ADXL GND]---GND ESP32
                       
     GPIO 12 ------------> Resistenza 220Ω ---> LED verde ---> GND
     GPIO 13 ------------> Resistenza 220Ω ---> LED rosso ---> GND
     GPIO 14 ------------> IN modulo relè
     Relè VCC/GND -------> 5V/GND
     Relè NO/COM --------> Clacson (collegamento come da schema clacson camper)

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