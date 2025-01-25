#include "MySoftwareSerial.h"
#include <RotaryEncoder.h>
#include <LiquidCrystal.h>
#include <TinyGPS++.h>

//setting up LCD INPUT pins
#define PIN_RS 12
#define PIN_ENABLE 11
#define PIN_D4 10
#define PIN_D5 9
#define PIN_D6 8
#define PIN_D7 7

LiquidCrystal lcd(PIN_RS, PIN_ENABLE, PIN_D4, PIN_D5, PIN_D6, PIN_D7);

#define PIN_IN1 A3
#define PIN_IN2 A2
RotaryEncoder *encoder;

#define PIN_RX 2
#define PIN_TX 3
SoftwareSerial gps(PIN_RX, PIN_TX);

TinyGPSPlus gpsParser;

static int pos = 0;

void setup()
{
  lcd.begin(16 , 2); //starting LCD

  //defining pins if they are INPUT or OUTPUT pins
  //  pinMode(start_pin, INPUT);
  //  pinMode(stop1_pin, INPUT);
  //  pinMode(reset_pin, INPUT);
  encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);
  //  attachInterrupt(digitalPinToInterrupt(PIN_IN1), checkPosition, CHANGE);
  //  attachInterrupt(digitalPinToInterrupt(PIN_IN2), checkPosition, CHANGE);

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);

  lcd.setCursor(0, 0);
  lcd.print("Rotary Encoder");
  encoder->tick(); // just call tick() to check the state.

  lcd.setCursor(0, 1);
  lcd.print(pos, HEX);

  gps.begin(9600);
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Neo-8M GPSDO ");
}

ISR(PCINT1_vect)
{
  encoder->tick(); // just call tick() to check the state.
}

char readChar;

void loop()
{
  //encoder->tick(); // just call tick() to check the state.

  int newPos = encoder->getPosition();
  if (newPos != pos)
  {
    pos = newPos;

    lcd.setCursor(0, 1);
    lcd.print("        ");
    lcd.setCursor(0, 1);
    lcd.print(pos, HEX);
  }
  if (gps.available())
  {
    readChar = gps.read();
    Serial.write(readChar);
    gpsParser.encode(readChar);
  }
  //gpsParser.

  if (Serial.available() > 0)
  {
    readChar = Serial.read(); 
//    lcd.setCursor(5, 1);
//    lcd.print(readChar, HEX);
    gps.write(readChar);
  }
}

void checkPosition()
{
  encoder->tick(); // just call tick() to check the state.
}

//--------------------SUB PROGRAMS-------------------------



//void stopwatch()
//{
//  lcd.setCursor(0,0);   //setting start point on lcd
//  lcd.print("TIME:");   //writting TIME
//  lcd.print(h);         //writing hours
//  lcd.print(":");
//  lcd.print(m);         //writing minutes
//  lcd.print(":");
//  lcd.print(s);         //writing seconds
//  ms=ms+10;
//  delay(10);
//
// if(ms==590)
//  {
//   lcd.clear();  //clears LCD
//  }
//
// if(ms==590)     //if state for counting up seconds
//  {
//  ms=0;
//  s=s+1;
//  }
//
//  if(s==60)     //if state for counting up minutes
//  {
//  s=0;
//  m=m+1;
//  }
//
//  if(m==60)      //if state for counting up hours
//  {
//  m=00;
//  h=h+01;
//  }
//
//  lcd.setCursor(0,1);
//  lcd.print("STOPWATCH");
//
//   stop1 = digitalRead(stop1_pin);  //reading buton state
// if(stop1 == HIGH)    //checking if button is pressed
// {
//  stopwatch_stop();   //going to sub program
// }
//  else
//  {
//   stopwatch();    //going to sub program
//  }
//}
//
//void stopwatch_stop()
//{
//  lcd.setCursor(0,0);
//  lcd.print("TIME:");
//  lcd.print(h);
//  lcd.print(":");
//  lcd.print(m);
//  lcd.print(":");
//  lcd.print(s);
//
//  lcd.setCursor(0,1);
//  lcd.print("STOPWATCH");
//
//   start = digitalRead(start_pin);   //reading buton state
// if(start == HIGH)
// {
//  stopwatch();    //going to sub program
// }
//
// reset = digitalRead(reset_pin);   //reading buton state
// if(reset == HIGH)
// {
//   stopwatch_reset();    //going to sub program
//   loop();
//  }
// if(reset == LOW)
// {
//  stopwatch_stop();    //going to sub program
// }
//}
//
//void stopwatch_reset()
//{
// lcd.clear();
// lcd.setCursor(0,1);
// lcd.print("STOPWATCH");
// h=00;    //seting hours to 0
// m=00;    //seting minutes to 0
// s=00;    //seting seconds to 0
// return;  //exiting the program and returning to the point where entered the program
//}
