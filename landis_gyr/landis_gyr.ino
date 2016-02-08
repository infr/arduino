/*
Mode A (IEC 62056-21)
/ = 2FH or AFH
? = 3FH
! = 21H
CR = 0DH
LF = 0AH

Mode B(?) (DLMS/COSEM)
(https://github.com/gurux)

Could help:
https://github.com/securestate/termineter
http://www.build-electronic-circuits.com/arduino-remote-control/
https://github.com/z3t0/Arduino-IRremote
https://github.com/sigio/dsmr22-landis-gyr-interface
http://forum.arduino.cc/index.php?topic=119247.0

About the meter:
http://www.landisgyr.fi/webfoo/wp-content/uploads/2012/09/D000028193_E350_d_en11.pdf

About the optical sensor:
Has been used: http://www.seeedstudio.com/wiki/index.php?title=Photo_Reflective_Sensor
Will be used: http://www.bebek.fi/kauppa/lisatiedot.php?PHPSESSID=v0fdpcds4eupvfaf92rjq5ppu5&&tuote_id=15973
http://www.vishay.com/docs/83760/tcrt5000.pdf
*/

#include <SoftwareSerial.h>

int led = 7;
int del = 100;

SoftwareSerial softSerial(10, 11); // TX, RX

byte a;

void setup() {
    pinMode(led, OUTPUT);
    Serial.begin(115200);
    while (!Serial) {
      ;  
    }
    
    softSerial.begin(9600);
    //softSerial.println("Hello, world?");
    Serial.println("Data from port:");
}

void loop() {
  digitalWrite(led, HIGH);
  delayMicroseconds(del);
  digitalWrite(led, LOW);
  delayMicroseconds(del);

  if(digitalRead(6)){
    del = del + 10;
    Serial.print("+");
    Serial.println(del);
  }
  
  if(digitalRead(5)){
    del = del - 10;
    Serial.print("-");
    Serial.println(del);

  }
  
   /*
  softSerial.listen();
  
  // while there is data coming in, read it
  // and send to the hardware serial port:
  while (softSerial.available() > 0) {
    Serial.write(softSerial.read());
    digitalWrite(led, HIGH);
  }
  
  delay(1000);
  digitalWrite(led, LOW);
*/
 /*
  byte cmd[] = {0xAF,0x3F,0x21,0x8D,0x0A};
  softSerial.write(cmd,5);
  
  while (softSerial.available() > 0){
    a = softSerial.read() & 0x7F; // cheap way of converting from 8N1 to 7E1
    char b = a; // convert serial byte to ASCII character
    Serial.print(b);
  }
  //Serial.println();
  delay(100);
 
  digitalWrite(TXpin, HIGH);
  delay(100);
  output = analogRead(RXpin);
  if(output 1){
     digitalWrite(led, HIGH);
     Serial.println("BING!");
     delay(1000);
     Serial.println("");
  }
  Serial.println(output);
  delay(100);
  digitalWrite(led, LOW);
  digitalWrite(TXpin, LOW);
  delay(200);*/
  

  Serial.println("Meter type");
  
  byte cmd[] = {0xAF,0x3F,0x21,0x8D,0x0A}; // query the meter for data "/?!"+<13><10> (OR {0x2F,0x3F,0x21,0x0D,0x0A}??)
  iskSer.write (cmd,5);
  
  byte a;
  while (iskSer.available() > 0){
    a = iskSer.read() & 0x7F; // cheap way of converting from 8N1 to 7E1
    char b = a; // convert serial byte to ASCII character
    Serial.print(b);
  }
  Serial.println();

  //<06>"000"<13><10> for energy data
  //loop through every single byte in the buffer and convert it to readable characters
  
}
