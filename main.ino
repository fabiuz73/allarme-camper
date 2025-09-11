#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

// Pin LED
const int LED_ROSSO = 25;
const int LED_VERDE = 26;

// Pin accelerometro (GY-61 analogico)
const int PIN_X = 34;
const int PIN_Y = 35;
const int PIN_Z = 32;

bool allarmeInserito = false;
int sogliaMovimento = 150;  // sensibilità (più basso = più sensibile)

// valori di riferimento a riposo
int xRef, yRef, zRef;

void setup() {
  Serial.begin(115200);

  // Avvio Bluetooth (solo una volta)
  if (!SerialBT.begin("ESP32_Allarme")) {
    Serial.println("Errore avvio Bluetooth!");
    return;
  }
  Serial.println("Bluetooth avviato, dispositivo: ESP32_Allarme");

  pinMode(LED_ROSSO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  // LED iniziale
  digitalWrite(LED_ROSSO, LOW);
  digitalWrite(LED_VERDE, HIGH);

  // calibrazione iniziale (posizione ferma)
  delay(1000);
  xRef = analogRead(PIN_X);
  yRef = analogRead(PIN_Y);
  zRef = analogRead(PIN_Z);

  Serial.println("Calibrazione completata.");
}

void loop() {
  // Ricezione comandi da app
  if (SerialBT.available()) {
    char c = SerialBT.read();
    if (c == '1') {
      allarmeInserito = true;
      digitalWrite(LED_ROSSO, HIGH);
      digitalWrite(LED_VERDE, LOW);
      SerialBT.println("Allarme inserito");
      Serial.println("Allarme inserito");
    } else if (c == '0') {
      allarmeInserito = false;
      digitalWrite(LED_ROSSO, LOW);
      digitalWrite(LED_VERDE, HIGH);
      SerialBT.println("Allarme disinserito");
      Serial.println("Allarme disinserito");
    }
  }

  // Se allarme inserito → controlla accelerometro
  if (allarmeInserito) {
    int x = analogRead(PIN_X);
    int y = analogRead(PIN_Y);
    int z = analogRead(PIN_Z);

    int dx = abs(x - xRef);
    int dy = abs(y - yRef);
    int dz = abs(z - zRef);

    if (dx > sogliaMovimento || dy > sogliaMovimento || dz > sogliaMovimento) {
      SerialBT.println("MOVIMENTO RILEVATO!");
      Serial.println("⚠ MOVIMENTO RILEVATO!");
      delay(2000); // pausa per evitare troppi messaggi
    }
  }

  delay(200);
}