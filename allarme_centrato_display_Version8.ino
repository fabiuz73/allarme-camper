#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include "BluetoothSerial.h"

// Pin Display ST7789
#define TFT_CS    15
#define TFT_DC    2
#define TFT_RST   4
#define TFT_BL    32

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
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

// Soglia movimento (modificabile da Bluetooth)
int sogliaMovimento = 100;
const int centro = 2048;

bool allarmeInserito = false;
bool giaConnesso = false;

// Variabili per debounce dei pulsanti
unsigned long lastButtonPress = 0;
const unsigned long debounceDelay = 200;  // 200ms di debounce

// Funzione per mostrare testo centrato sul display
void showMessage(String testo, uint16_t colore = ST77XX_GREEN) {
  tft.fillScreen(ST77XX_BLACK);
  int16_t x1, y1;
  uint16_t w, h;
  tft.setTextSize(2);
  tft.setTextColor(colore);
  tft.getTextBounds(testo, 0, 0, &x1, &y1, &w, &h);
  int x = (tft.width()  - w) / 2 - x1;
  int y = (tft.height() - h) / 2 - y1;
  tft.setCursor(x, y);
  tft.print(testo);
}

void setup() {
  pinMode(TFT_BL, OUTPUT);
  digitalWrite(TFT_BL, HIGH);

  tft.init(240, 320);
  tft.setRotation(1);
  tft.fillScreen(ST77XX_BLACK);

  pinMode(ledVerde, OUTPUT);
  pinMode(ledRosso, OUTPUT);
  pinMode(releClacson, OUTPUT);
  digitalWrite(ledVerde, HIGH);
  digitalWrite(ledRosso, LOW);
  digitalWrite(releClacson, LOW);

  // Configurazione pulsanti fisici con INPUT_PULLUP
  pinMode(buttonInserisci, INPUT_PULLUP);
  pinMode(buttonDisinserisci, INPUT_PULLUP);

  Serial.begin(115200);
  SerialBT.begin("ESP32-CAMPER");
  SerialBT.println("Allarme pronto!");

  showMessage("ALLARME CAMPER", ST77XX_GREEN);
}

void loop() {
  // Stato connessione Bluetooth
  if (SerialBT.hasClient()) {
    if (!giaConnesso) {
      showMessage("ALLARME CONNESSO", ST77XX_YELLOW);
      giaConnesso = true;
    }
  } else {
    if (giaConnesso) {
      showMessage("ALLARME CAMPER", ST77XX_GREEN);
      giaConnesso = false;
    }
  }

  // Ricezione comandi Bluetooth
  if (SerialBT.available()) {
    String comando = SerialBT.readStringUntil('\n');
    comando.trim();
    Serial.println("Comando ricevuto via Bluetooth: " + comando);

    if (comando.length() == 0) return; // IGNORA COMANDI VUOTI

    if (comando.startsWith("SOGLIA:")) {
      String sogliaStr = comando.substring(7);
      int nuovaSoglia = sogliaStr.toInt();
      if (nuovaSoglia > 0 && nuovaSoglia < 5000) {
        sogliaMovimento = nuovaSoglia;
        SerialBT.println("Soglia aggiornata: " + String(sogliaMovimento));
        Serial.println("Soglia aggiornata: " + String(sogliaMovimento));
        showMessage("SOGLIA: " + String(sogliaMovimento), ST77XX_CYAN);
        delay(1000);
        showMessage(allarmeInserito ? "ALLARME INSERITO" : "ALLARME DISINSERITO", allarmeInserito ? ST77XX_RED : ST77XX_GREEN);
      } else {
        SerialBT.println("Valore soglia non valido!");
        Serial.println("Valore soglia non valido!");
        showMessage("SOGLIA ERRATA", ST77XX_YELLOW);
        delay(1000);
        showMessage(allarmeInserito ? "ALLARME INSERITO" : "ALLARME DISINSERITO", allarmeInserito ? ST77XX_RED : ST77XX_GREEN);
      }
    }
    else if (comando == "GET_SOGLIA") {
      SerialBT.println("Soglia attuale: " + String(sogliaMovimento));
      Serial.println("Soglia attuale: " + String(sogliaMovimento));
    }
    else if (comando == "1") {
      allarmeInserito = true;
      digitalWrite(ledVerde, LOW);
      digitalWrite(ledRosso, HIGH);
      SerialBT.println("Allarme inserito");
      showMessage("ALLARME INSERITO", ST77XX_RED);
    } else if (comando == "0") {
      allarmeInserito = false;
      digitalWrite(ledVerde, HIGH);
      digitalWrite(ledRosso, LOW);
      digitalWrite(releClacson, LOW);
      SerialBT.println("Allarme disinserito");
      showMessage("ALLARME DISINSERITO", ST77XX_GREEN);
    } else if (comando == "T") {
      digitalWrite(releClacson, HIGH);
      SerialBT.println("Test clacson attivato");
      showMessage("TEST CLACSON!", ST77XX_CYAN);
      delay(1000);
      digitalWrite(releClacson, LOW);
      showMessage(allarmeInserito ? "ALLARME INSERITO" : "ALLARME DISINSERITO", allarmeInserito ? ST77XX_RED : ST77XX_GREEN);
    } else {
      SerialBT.println("Comando non riconosciuto");
      Serial.println("Comando non riconosciuto!");
      showMessage("COMANDO ERRATO", ST77XX_YELLOW);
      delay(1000);
      showMessage(allarmeInserito ? "ALLARME INSERITO" : "ALLARME DISINSERITO", allarmeInserito ? ST77XX_RED : ST77XX_GREEN);
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
      showMessage("ALLARME INSERITO", ST77XX_RED);
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
      showMessage("ALLARME DISINSERITO", ST77XX_GREEN);
      lastButtonPress = currentTime;  // Aggiorna il tempo dell'ultimo pulsante premuto
    }
  }

  // Se allarme inserito, controlla accelerometro
  if (allarmeInserito) {
    int x = analogRead(xPin);
    int y = analogRead(yPin);
    int z = analogRead(zPin);

    if (abs(x - centro) > sogliaMovimento ||
        abs(y - centro) > sogliaMovimento ||
        abs(z - centro) > sogliaMovimento) {
      digitalWrite(releClacson, HIGH);
      SerialBT.println("Movimento rilevato! Clacson attivo!");
      Serial.println("Movimento rilevato! Clacson attivo!");
      showMessage("MOVIMENTO RILEVATO!", ST77XX_YELLOW);
      delay(2000);
      digitalWrite(releClacson, LOW);
      showMessage("ALLARME INSERITO", ST77XX_RED);
      delay(1000);
    }
    // LED rosso lampeggiante
    digitalWrite(ledRosso, HIGH);
    delay(400);
    digitalWrite(ledRosso, LOW);
    delay(400);
  }
}