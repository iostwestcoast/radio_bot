#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
//#define PIN_PHOTO_SENSOR A1

RF24 radio(8, 10);
int data[3];

#define PIN_VRX A2
#define PIN_VRY A3
//#define PIN_BUTTON 3

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
  //int val = analogRead(PIN_PHOTO_SENSOR);
  int potval = analogRead(A1);



  radio.write(&data, sizeof(data));
  delay(50);

  Serial.println(potval);
     
  int valX = analogRead(PIN_VRX);
  int valY = analogRead(PIN_VRY);
  //int stop = digitalRead(PIN_BUTTON);

  Serial.print("X = ");
  Serial.println(analogRead(PIN_VRX));

  Serial.print("Y = ");
  Serial.println(analogRead(PIN_VRY));

  data[0] = potval;
  data[1] = valX;
  data[2] = valY;
}


