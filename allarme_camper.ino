#include "BluetoothSerial.h"
BluetoothSerial SerialBT;

// Pin collegamento hardware
const int ledVerde = 12;
const int ledRosso = 13;
const int releClacson = 14;

// Pin accelerometro ADXL335 (GY-61)
const int xPin = 34;
const int yPin = 35;
const int zPin = 32;

// Pin pulsanti fisici per controllo allarme
const int buttonInserisci = 16;  // Pulsante per inserire allarme (GPIO16)
const int buttonDisinserisci = 17;  // Pulsante per disinserire allarme (GPIO17)

// Soglia movimento (regolabile)
const int sogliaMovimento = 100;
// Valore centrale (tipico ADC 12 bit ESP32)
const int centro = 2048;

bool allarmeInserito = false;

// Variabili per debounce dei pulsanti
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;  // 200ms di debounce

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRosso, OUTPUT);
  pinMode(releClacson, OUTPUT);
  digitalWrite(ledVerde, HIGH);  // LED verde acceso (allarme disinserito)
  digitalWrite(ledRosso, LOW);   // LED rosso spento
  digitalWrite(releClacson, LOW);

  // Configurazione pulsanti fisici con INPUT_PULLUP
  pinMode(buttonInserisci, INPUT_PULLUP);
  pinMode(buttonDisinserisci, INPUT_PULLUP);

  Serial.begin(115200);
  SerialBT.begin("ESP32-CAMPER");
  SerialBT.println("Allarme pronto!");
}

void loop() {
  // Ricezione comandi Bluetooth
  if (SerialBT.available()) {
    String comando = SerialBT.readStringUntil('\n');
    comando.trim();

    if (comando == "1") {
      allarmeInserito = true;
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledRosso, HIGH); // LED rosso acceso (sarà lampeggiato dopo)
      SerialBT.println("Allarme inserito");
    } else if (comando == "0") {
      allarmeInserito = false;
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledRosso, LOW);
      digitalWrite(releClacson, LOW);
      SerialBT.println("Allarme disinserito");
    } else if (comando == "T") {
      digitalWrite(releClacson, HIGH);
      SerialBT.println("Test clacson attivato");
      delay(1000); // Clacson suona per 1 secondo
      digitalWrite(releClacson, LOW);
    } else {
      SerialBT.println("Comando non riconosciuto");
    }
  }

  // Gestione pulsanti fisici con debounce
  unsigned long currentTime = millis();
  if (currentTime - lastButtonPress > debounceDelay) {
    // Lettura pulsante inserisci allarme (GPIO16)
    if (digitalRead(buttonInserisci) == LOW) {
      allarmeInserito = true;
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledRosso, HIGH);
      SerialBT.println("Allarme inserito tramite pulsante");
      Serial.println("Allarme inserito tramite pulsante fisico GPIO16");
      lastButtonPress = currentTime;  // Aggiorna il tempo dell'ultimo pulsante premuto
    }
    // Lettura pulsante disinserisci allarme (GPIO17)
    else if (digitalRead(buttonDisinserisci) == LOW) {
      allarmeInserito = false;
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledRosso, LOW);
      digitalWrite(releClacson, LOW);
      SerialBT.println("Allarme disinserito tramite pulsante");
      Serial.println("Allarme disinserito tramite pulsante fisico GPIO17");
      lastButtonPress = currentTime;  // Aggiorna il tempo dell'ultimo pulsante premuto
    }
  }

  // Se allarme inserito, controlla accelerometro
  if (allarmeInserito) {
    int x = analogRead(xPin);
    int y = analogRead(yPin);
    int z = analogRead(zPin);

    // Verifica movimento (adatta le soglie dopo test)
    if (abs(x - centro) > sogliaMovimento ||
        abs(y - centro) > sogliaMovimento ||
        abs(z - centro) > sogliaMovimento) {
      // Movimento rilevato!
      digitalWrite(releClacson, HIGH);
      SerialBT.println("Movimento rilevato! Clacson attivo!");
      delay(2000); // Suona per 2 secondi
      digitalWrite(releClacson, LOW);
      delay(1000); // Anti-rimbalzo
    }
    // LED rosso lampeggiante
    digitalWrite(ledRosso, HIGH);
    delay(400);
    digitalWrite(ledRosso, LOW);
    delay(400);
  }
}