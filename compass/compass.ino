/*
/////////////////////////////////
Htachi HM55B Compass
parallax (#)

AUTHOR:   kiilo kiilo@kiilo.org
License:  http://creativecommons.org/licenses/by-nc-sa/2.5/ch/

http://parallax.com/Store/Microcontrollers/BASICStampModules/tabid/134/txtSearch/hm55b/List/1/ProductID/98/Default.aspx?SortField=ProductName%2cProductName
http://sage.medienkunst.ch/tiki-index.php?page=HowTo_Arduino_Parallax_HM55B_Kompass
http://playground.arduino.cc/HM55B

/////////////////////////////////
*/
#include <math.h> // (no semicolon)
#include <Servo.h>

//// VARS
byte CLK_pin = 8;
byte EN_pin = 9;
byte DIO_pin = 10;

int X_Data = 0;
int Y_Data = 0;
int angle;

int servoFrontPin = 7;

Servo servo;

//// FUNCTIONS

void ShiftOut(int Value, int BitsCount) {
  for(int i = BitsCount; i >= 0; i--) {
    digitalWrite(CLK_pin, LOW);
    if ((Value & 1 << i) == ( 1 << i)) {
      digitalWrite(DIO_pin, HIGH);
      //Serial.print("1");
    }
    else {
      digitalWrite(DIO_pin, LOW);
      //Serial.print("0");
    }
    digitalWrite(CLK_pin, HIGH);
    delayMicroseconds(1);
  }
//Serial.print(" ");
}

int ShiftIn(int BitsCount) {
  int ShiftIn_result;
    ShiftIn_result = 0;
    pinMode(DIO_pin, INPUT);
    for(int i = BitsCount; i >= 0; i--) {
      digitalWrite(CLK_pin, HIGH);
      delayMicroseconds(1);
      if (digitalRead(DIO_pin) == HIGH) {
        ShiftIn_result = (ShiftIn_result << 1) + 1; 
        //Serial.print("x");
      }
      else {
        ShiftIn_result = (ShiftIn_result << 1) + 0;
        //Serial.print("_");
      }
      digitalWrite(CLK_pin, LOW);
      delayMicroseconds(1);
    }
  //Serial.print(":");

// below is difficult to understand:
// if bit 11 is Set the value is negative
// the representation of negative values you
// have to add B11111000 in the upper Byte of
// the integer.
// see: http://en.wikipedia.org/wiki/Two%27s_complement
  if ((ShiftIn_result & 1 << 11) == 1 << 11) {
    ShiftIn_result = (B11111000 << 8) | ShiftIn_result; 
  }


  return ShiftIn_result;
}

void HM55B_Reset() {
  pinMode(DIO_pin, OUTPUT);
  digitalWrite(EN_pin, LOW);
  ShiftOut(B0000, 3);
  digitalWrite(EN_pin, HIGH);
}

void HM55B_StartMeasurementCommand() {
  pinMode(DIO_pin, OUTPUT);
  digitalWrite(EN_pin, LOW);
  ShiftOut(B1000, 3);
  digitalWrite(EN_pin, HIGH);
}

int HM55B_ReadCommand() {
  int result = 0;
  pinMode(DIO_pin, OUTPUT);
  digitalWrite(EN_pin, LOW);
  ShiftOut(B1100, 3);
  result = ShiftIn(3);
  return result;
}


void pulseServo(int servoPin, int pulseLenUs)
{
  digitalWrite(servoPin, HIGH);
  delayMicroseconds(pulseLenUs);
  digitalWrite(servoPin, LOW);
  delay(20);
}

void setup() {
  Serial.begin(115200);
  pinMode(EN_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  pinMode(DIO_pin, INPUT);

  servo.attach(7);
    pinMode(servoFrontPin, OUTPUT);
  HM55B_Reset();
}

void loop() {
  
    int sum = 0;
    for(int i=0; i<5; i++){
      HM55B_StartMeasurementCommand();
      delay(40);
      HM55B_ReadCommand();
  
      X_Data = ShiftIn(11); // Field strength in X
      Y_Data = ShiftIn(11); // and Y direction
  
      digitalWrite(EN_pin, HIGH); // ok deselect chip
      angle = 180 * (atan2(-1 * Y_Data , X_Data) / M_PI); // angle is atan( -y/x) !!!
     // Serial.print(angle);
      sum = sum + angle;
    }
    
    sum = abs(sum);
    float avg = sum / 5;
    
    servo.write(avg);
    Serial.println(avg);
    delay(100);
/*
  for (int i=0; i<=25; i++) {
    pulseServo(servoFrontPin, 1500);
  }
  for (int i=0; i<=25; i++) {
    pulseServo(servoFrontPin, 2400);
  }
  for (int i=0; i<=25; i++) {
    pulseServo(servoFrontPin, 1500);
  }
  for (int i=0; i<=25; i++) {
    pulseServo(servoFrontPin, 600);
  }
 // delay(100);
*/
}

