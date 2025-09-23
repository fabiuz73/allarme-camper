# Implementazione Pulsanti Fisici - Allarme Camper ESP32

## Panoramica
È stata aggiunta la funzionalità di controllo tramite pulsanti fisici per inserire e disinserire l'allarme, oltre ai comandi Bluetooth esistenti.

## Collegamenti Hardware

### Pulsanti
- **GPIO16**: Pulsante "Inserisci Allarme" 
- **GPIO17**: Pulsante "Disinserisci Allarme"

### Schema di Collegamento
```
ESP32 GPIO16  ----  [Pulsante 1]  ----  GND
ESP32 GPIO17  ----  [Pulsante 2]  ----  GND
```

**Nota**: I pulsanti sono configurati con `INPUT_PULLUP`, quindi:
- **Stato normale**: Pin HIGH (3.3V) tramite resistenza pull-up interna
- **Pulsante premuto**: Pin LOW (0V) quando il pulsante collega il pin a GND

## Funzionalità Implementate

### Pulsante GPIO16 - Inserisci Allarme
- Equivalente al comando Bluetooth "1"
- Accende LED rosso, spegne LED verde
- Mostra "ALLARME INSERITO" sul display
- Invia messaggio Bluetooth di conferma

### Pulsante GPIO17 - Disinserisci Allarme
- Equivalente al comando Bluetooth "0"
- Accende LED verde, spegne LED rosso
- Spegne clacson se attivo
- Mostra "ALLARME DISINSERITO" sul display
- Invia messaggio Bluetooth di conferma

## Debounce Software
- **Tempo di debounce**: 200ms
- **Funzionalità**: Previene attivazioni accidentali multiple
- **Meccanismo**: Una volta premuto un pulsante, il sistema ignora ulteriori pressioni per 200ms

## File Modificati
1. **`allarme_centrato_display_Version8.ino`** - Versione principale con display
2. **`allarme_camper.ino`** - Versione semplificata
3. **`README_Version3.md`** - Documentazione aggiornata

## Codice Implementato

### Dichiarazioni
```cpp
// Pin pulsanti fisici per controllo allarme
const int buttonInserisci = 16;  // Pulsante per inserire allarme (GPIO16)
const int buttonDisinserisci = 17;  // Pulsante per disinserire allarme (GPIO17)

// Variabili per debounce dei pulsanti
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;  // 200ms di debounce
```

### Setup
```cpp
// Configurazione pulsanti fisici con INPUT_PULLUP
pinMode(buttonInserisci, INPUT_PULLUP);
pinMode(buttonDisinserisci, INPUT_PULLUP);
```

### Loop principale
```cpp
// Gestione pulsanti fisici con debounce
unsigned long currentTime = millis();
if (currentTime - lastButtonPress > debounceDelay) {
  // Lettura pulsante inserisci allarme (GPIO16)
  if (digitalRead(buttonInserisci) == LOW) {
    // Logica identica a comando Bluetooth "1"
    allarmeInserito = true;
    digitalWrite(ledVerde, LOW);
    digitalWrite(ledRosso, HIGH);
    // ... resto della logica
  }
  // Lettura pulsante disinserisci allarme (GPIO17)
  else if (digitalRead(buttonDisinserisci) == LOW) {
    // Logica identica a comando Bluetooth "0"
    allarmeInserito = false;
    digitalWrite(ledVerde, HIGH);
    digitalWrite(ledRosso, LOW);
    digitalWrite(releClacson, LOW);
    // ... resto della logica
  }
}
```

## Test e Validazione
✅ Sintassi verificata (parentesi bilanciate)
✅ Pin definiti correttamente  
✅ INPUT_PULLUP configurato nel setup
✅ Debounce implementato correttamente
✅ Logica identica ai comandi Bluetooth
✅ Documentazione aggiornata

## Compatibilità
- Mantiene piena compatibilità con comandi Bluetooth esistenti
- Non interferisce con accelerometro o display
- Preserva tutta la funzionalità esistente