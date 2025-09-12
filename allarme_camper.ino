#include "BluetoothSerial.h"
#include <TFT_eSPI.h>

BluetoothSerial SerialBT;
TFT_eSPI tft = TFT_eSPI();

// Pin collegamento hardware
const int rgbLedR = 12;  // RGB LED Red pin
const int rgbLedG = 13;  // RGB LED Green pin  
const int rgbLedB = 14;  // RGB LED Blue pin
const int releClacson = 27; // Relay pin moved to avoid conflict

// Pin accelerometro ADXL335 (GY-61)
const int xPin = 34;
const int yPin = 35;
const int zPin = 32;

// Soglia movimento (regolabile)
const int sogliaMovimento = 100;
// Valore centrale (tipico ADC 12 bit ESP32)
const int centro = 2048;

bool allarmeInserito = false;
bool bluetoothConnesso = false;
unsigned long ultimoBlinkLed = 0;
bool statoLedRosso = false;

// Funzioni per controllo RGB LED
void setRgbLed(int r, int g, int b) {
  digitalWrite(rgbLedR, r ? HIGH : LOW);
  digitalWrite(rgbLedG, g ? HIGH : LOW);
  digitalWrite(rgbLedB, b ? HIGH : LOW);
}

void rgbLedOff() {
  setRgbLed(0, 0, 0);
}

void rgbLedGreen() {
  setRgbLed(0, 1, 0);
}

void rgbLedRed() {
  setRgbLed(1, 0, 0);
}

// Funzioni per controllo display
void aggiornaDisplay() {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(2);
  
  if (!bluetoothConnesso) {
    tft.setCursor(10, 50);
    tft.println("ALLARME CAMPER");
    tft.setCursor(10, 80);
    tft.println("NON CONNESSO");
  } else if (!allarmeInserito) {
    tft.setCursor(10, 50);
    tft.println("ALLARME");
    tft.setCursor(10, 80);
    tft.println("CONNESSO");
  } else {
    tft.setCursor(10, 50);
    tft.println("ALLARME");
    tft.setCursor(10, 80);
    tft.println("INSERITO");
  }
}

void setup() {
  pinMode(rgbLedR, OUTPUT);
  pinMode(rgbLedG, OUTPUT);
  pinMode(rgbLedB, OUTPUT);
  pinMode(releClacson, OUTPUT);
  
  rgbLedOff();  // RGB LED spento inizialmente
  digitalWrite(releClacson, LOW);

  // Inizializzazione display
  tft.init();
  tft.setRotation(1); // Orientamento landscape
  tft.fillScreen(TFT_BLACK);
  
  Serial.begin(115200);
  SerialBT.begin("ESP32-CAMPER");
  SerialBT.println("Allarme pronto!");
  
  // Display iniziale
  aggiornaDisplay();
}

void loop() {
  // Verifica connessione Bluetooth
  bool nuovoStatoBluetooth = SerialBT.hasClient();
  if (nuovoStatoBluetooth != bluetoothConnesso) {
    bluetoothConnesso = nuovoStatoBluetooth;
    aggiornaDisplay();
    
    if (!bluetoothConnesso) {
      // Bluetooth disconnesso - spegni LED RGB e resetta allarme
      rgbLedOff();
      allarmeInserito = false;
    } else {
      // Bluetooth connesso - LED verde
      rgbLedGreen();
    }
  }
  
  // Ricezione comandi Bluetooth
  if (SerialBT.available()) {
    String comando = SerialBT.readStringUntil('\n');
    comando.trim();

    if (comando == "1") {
      allarmeInserito = true;
      aggiornaDisplay();
      SerialBT.println("Allarme inserito");
    } else if (comando == "0") {
      allarmeInserito = false;
      aggiornaDisplay();
      rgbLedGreen(); // Torna verde quando connesso ma disinserito
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

  // Se allarme inserito, controlla accelerometro e gestisci LED lampeggiante
  if (allarmeInserito && bluetoothConnesso) {
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
    
    // LED rosso lampeggiante ogni secondo
    if (millis() - ultimoBlinkLed >= 1000) {
      ultimoBlinkLed = millis();
      statoLedRosso = !statoLedRosso;
      if (statoLedRosso) {
        rgbLedRed();
      } else {
        rgbLedOff();
      }
    }
  }
  
  delay(50); // Piccola pausa per evitare sovraccarico CPU
}