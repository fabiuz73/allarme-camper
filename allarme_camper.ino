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

// Soglia movimento (regolabile)
const int sogliaMovimento = 100;
// Valore centrale (tipico ADC 12 bit ESP32)
const int centro = 2048;

bool allarmeInserito = false;

void setup() {
  pinMode(ledVerde, OUTPUT);
  pinMode(ledRosso, OUTPUT);
  pinMode(releClacson, OUTPUT);
  digitalWrite(ledVerde, HIGH);  // LED verde acceso (allarme disinserito)
  digitalWrite(ledRosso, LOW);   // LED rosso spento
  digitalWrite(releClacson, LOW);

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