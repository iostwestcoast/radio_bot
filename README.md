# Radio Bot

Remote controlled bot based on an Arduino using a radio module

Сomponents:
NRF,
NRF+,
x2 5v to 3.3 v,
Motor Shield,
Arduino Leonardo/Amperka Iskra Neo,
Arduino Pro Micro,
x2 18650,
x2 DC motor,
x2 joystick,
RGB LED strip,
potentiometer,
x2 bradboard,
powerbank,
servo 360.

The final look of the bot:

![photo_2023-06-13_16-50-49](https://github.com/iostwestcoast/radio_bot/assets/114690482/110eeee0-d2ee-4c9b-81e7-477d4c5edd63)


The final view of the control panel:

![photo_2023-06-13_16-51-03](https://github.com/iostwestcoast/radio_bot/assets/114690482/19a8508d-04a6-48b0-b743-19bfc72776d4)

https://github.com/iostwestcoast/radio_bot/assets/114690482/ab062ac3-bc5f-469a-a076-65d38420400c

The bot was created to pass a certain obstacle course and unfortunately for technical reasons did not meet expectations

In the end, 1/4 of the obstacle was passed and it took 9.45 minutes to complete the entire course

![image](https://github.com/iostwestcoast/radio_bot/assets/114690482/be7ff708-898a-4c7a-b90d-fde0acee8878)

1. Control panel code:
```C++
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(8, 10);
int data[3];

#define PIN_VRX A2
#define PIN_VRY A3

void setup() {
  Serial.begin(9600);

  radio.begin();
  radio.setChannel(1);
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_HIGH);
  radio.openWritingPipe(0x2234567890LL);
  radio.stopListening();

  pinMode(A1, INPUT);
}

void loop() {
  int potval = analogRead(A1);

  radio.write(&data, sizeof(data));
  delay(50);

  Serial.println(potval);
     
  int valX = analogRead(PIN_VRX);
  int valY = analogRead(PIN_VRY);

  Serial.print("X = ");
  Serial.println(analogRead(PIN_VRX));

  Serial.print("Y = ");
  Serial.println(analogRead(PIN_VRY));

  data[0] = potval;
  data[1] = valX;
  data[2] = valY;
}
```

2. Bot code:
```C++
#include <SPI.h>
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
  counter++;
  FastLED.show();
}
```

