#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"


#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
const int pinoSensor = 34;
const int LED_VERMELHO = 12;
const int LED_VERDE = 14;
int analogSoloSeco = 4095;
int analogSoloMolhado = 0;
int percSoloSeco = 0;
int percSoloMolhado = 100;
#define DHTPIN 4
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(115200);
  SerialBT.begin("REYSO");
  dht.begin();
  lcd.begin();
  pinMode(pinoSensor, INPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(LED_VERDE, OUTPUT);

  lcd.backlight();
}

void loop() {
  delay(2000);

  int h = constrain(analogRead(pinoSensor), analogSoloMolhado, analogSoloSeco);
  h = map(h, analogSoloMolhado, analogSoloSeco, percSoloMolhado, percSoloSeco);
  float t = dht.readTemperature();

  if (Serial.available()) {
    SerialBT.print("Temperatura: ");
    SerialBT.println(t);
    SerialBT.print("Umidade: ");
    SerialBT.print(h);
    SerialBT.println("%");

    delay(20);
  }

  lcd.clear();
  lcd.print("Temp:");
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Umidade:");
  lcd.print(h);
  lcd.print("%");

  if (t > 22 && t < 28) {
    digitalWrite(LED_VERDE, HIGH);
    digitalWrite(LED_VERMELHO, LOW);
  } else {
    digitalWrite(LED_VERDE, LOW);
    digitalWrite(LED_VERMELHO, HIGH);
  }
}