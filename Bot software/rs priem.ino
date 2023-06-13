#include <SPI.h> //RF24 by TMRh20
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>

Servo servo;

#define servoPin 11
int data[3];

RF24 radio(9, 10);

#define SPEED_1      5 
#define DIR_1        4
 
#define SPEED_2      6
#define DIR_2        7

#define LED_PIN 3
#define LED_NUM 3
#include "FastLED.h"
CRGB leds[LED_NUM];
byte counter;

void setup(){
  Serial.begin(9600);

  radio.begin();
  radio.setChannel(1);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openReadingPipe(0, 0x2234567890LL);
  radio.startListening();

  servo.attach(servoPin);

  for (int i = 4; i < 8; i++) {     
  pinMode(i, OUTPUT);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, LED_NUM);
  FastLED.setBrightness(200);
  }
}

void loop(){
  if (radio.available()){
    radio.read(&data, sizeof(data));
    Serial.println(String(data[0]) + "; " + String(data[1]) + "; " + String(data[2]));
  

  int potVal = data[0];
  servo.write(map(potVal, 0, 1023, 0, 180));

  delay(50);
  
  int valX = data[1];
  int valY = data[2];

  if (valX < 112) {
   digitalWrite(DIR_1, LOW);
   analogWrite(SPEED_1, 255);

   digitalWrite(DIR_2, HIGH);
   analogWrite(SPEED_2, 255);
  }
  else if (valX > 912) {
   digitalWrite(DIR_1, HIGH);
   analogWrite(SPEED_1, 255);

   digitalWrite(DIR_2, LOW);
   analogWrite(SPEED_2, 255);
  }
  else if (valY < 112) {
   digitalWrite(DIR_1, LOW);
   analogWrite(SPEED_1, 255);

   digitalWrite(DIR_2, LOW);
   analogWrite(SPEED_2, 255);
  }
  else if (valY > 912) {
   digitalWrite(DIR_1, HIGH);
   analogWrite(SPEED_1, 255);

   digitalWrite(DIR_2, HIGH);
   analogWrite(SPEED_2, 255);
  }
  
  else if (112 < valX < 912){
   digitalWrite(DIR_1, HIGH);
   analogWrite(SPEED_1, 0);

   digitalWrite(DIR_2, HIGH);
   analogWrite(SPEED_2, 0);
  }
  else if (112 < valY < 912){
   digitalWrite(DIR_1, HIGH);
   analogWrite(SPEED_1, 0);

   digitalWrite(DIR_2, HIGH);
   analogWrite(SPEED_2, 0);
  }
  }

  for (int i = 0; i < LED_NUM; i++) {
    leds[i].setHue(counter + i * 255 / LED_NUM);
  }
  counter++;        // counter меняется от 0 до 255 (тип данных byte)
  FastLED.show();
}

