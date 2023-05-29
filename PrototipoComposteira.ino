//Declarando bibliotecas

#include "DHT.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "BluetoothSerial.h"

//Configurando BLE
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//Declararando variaveis e pinagem do circuito
BluetoothSerial SerialBT;
const int pinoSensor = 34;
const int pinoAgua = 35;
const int buzzer = 4;
int analogSoloSeco = 4095; 
int analogSoloMolhado = 0;
int percSeco = 0;
int percMolhado = 100;

#define DHTPIN 4
#define DHTTYPE DHT11

LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  //Iniciando monitor serial e declarando pinagem dos sensores/leds
  Serial.begin(115200);
  SerialBT.begin("REYSO");
  dht.begin();
  lcd.begin();
  pinMode(pinoSensor, INPUT);
  pinMode(pinoAgua, INPUT);
  pinMode(buzzer, OUTPUT);
  lcd.backlight();
}

void loop() {
  delay(5000);
  
  int a = 
  //Transformando resistencia do sensor em porcentagem
  int h = constrain(analogRead(pinoSensor), analogSoloMolhado, analogSoloSeco);
  h = map(h, analogSoloMolhado, analogSoloSeco, percMolhado, percSeco);
  float t = dht.readTemperature();

  //Imprimindo informações no monitor Bluetooth
  if (Serial.available()) {
    SerialBT.print("Temperatura: ");
    SerialBT.println(t);
    SerialBT.print("Umidade: ");
    SerialBT.print(h);
    SerialBT.println("%");

    delay(20);
  }
  //Imprimindo informações no LCD
  lcd.clear();
  lcd.print("Temp:");
  lcd.print(t);
  lcd.setCursor(0, 1);
  lcd.print("Umidade:");
  lcd.print(h);
  lcd.print("%");

  //Lógica dos parametros para alertar o buzzer caso algo saia de controle
  if(t < 25 && t > 30){
    tone(buzzer, 800);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Temperatura fora do parametros!");
  }else if(h < 50 && h > 70){
    tone(buzzer, 800);
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Umidade fora do parametros!");
  }

  if(a >= 90){
    lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("Nivel de chorume completo");
  }
}