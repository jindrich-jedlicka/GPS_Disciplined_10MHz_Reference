#include <NMEAGPS.h>
#include <NeoSWSerial.h>
#include <RotaryEncoder.h>
#include <LiquidCrystal.h>

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
NeoSWSerial gpsSerial(PIN_RX, PIN_TX);

static NMEAGPS nmeaGps;

static int pos = 0;

void setup()
{
  lcd.begin(16 , 2); //starting LCD

  //defining pins if they are INPUT or OUTPUT pins
  //  pinMode(start_pin, INPUT);
  //  pinMode(stop1_pin, INPUT);
  //  pinMode(reset_pin, INPUT);
  encoder = new RotaryEncoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::FOUR3);

  PCICR |= (1 << PCIE1);
  PCMSK1 |= (1 << PCINT10) | (1 << PCINT11);

  lcd.setCursor(0, 0);
  lcd.print("Rotary Encoder");
  encoder->tick(); // just call tick() to check the state.

  lcd.setCursor(0, 1);
  lcd.print(pos, HEX);

  gpsSerial.begin(9600);
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Neo-8M GPSDO ");
}

ISR(PCINT0_vect)
{
  NeoSWSerial::rxISR(PINB);
}

ISR(PCINT1_vect)
{
  NeoSWSerial::rxISR(PINC);
  encoder->tick(); // just call tick() to check the state.
}

ISR(PCINT2_vect)
{
  NeoSWSerial::rxISR(PIND);
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
  if (gpsSerial.available())
  {
    readChar = gpsSerial.read();
    Serial.write(readChar);
    //gpsParser.encode(readChar);
  }
  //nmeaGps.satellites[0];
  //gpsParser.

  if (Serial.available() > 0)
  {
    readChar = Serial.read(); 
//    lcd.setCursor(5, 1);
//    lcd.print(readChar, HEX);
    gpsSerial.write(readChar);
  }
}

void checkPosition()
{
  encoder->tick(); // just call tick() to check the state.
}
