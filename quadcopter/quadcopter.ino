/*
//////////////////////////////////

QUAD COPTER SIMULATOR
2016 Kim Salmi
Creative Commons Attribution ShareAlike 3.0.

/////////////////////////////////
Orginal Axis Accelerometer code
AUTHOR:   kiilo kiilo@kiilo.org
License:  http://creativecommons.org/licenses/by-nc-sa/2.5/ch/
/////////////////////////////////
*/

int CS_pin = 9;
int CLK_pin = 10;
int DIO_pin = 11;

int ledFlight = 6;
int ledMax = 7;

// Could be controlled by a joystick (currently uses cordinates on boot)
int joyX = 0;
int joyY = 0;

int aX  = 0;
int aY  = 0;
int aZ  = 0;

// Game Settings
int screenHeight = 14;
int screenWidth = 40;
int gameX = 0;
int gameY = screenHeight - 2;
String flightStatus = "Ready";
String drone = "o-o";


// Game code
void flyUp(){
  gameY=gameY-1;
  flightStatus="Flying ^";
  if(gameY <= 2){
    gameY = 2;
    digitalWrite(ledMax, HIGH);
    flightStatus="Flying MAX";
  }
}

void flyDown(){
  gameY=gameY+1;
  flightStatus="Flying v";
  if(gameY >= screenHeight-2){
    gameY = screenHeight-2;
    flightStatus="Landed";
  }
}


void flyLeft(){
  gameX=gameX-2;
  flightStatus="Flying <";
  if(gameX < 1){
    gameX = 1;
  }
}
void flyRight(){
  gameX=gameX+2;
  flightStatus="Flying >";
  if(gameX > screenWidth-1){
    gameX = screenWidth-1;
  }
}

void drawScreen(){
  Serial.println("");
  for(int i=0; i<screenHeight;i++){
    if(i==(screenHeight-1)){ // Draw the floor
      for(int a=0; a<screenWidth; a++){
        Serial.print("-");
      }
      Serial.println("");
    }else if(i==1){ // Draw statusbar
      Serial.print("Height: ");
      Serial.print(screenHeight-gameY-2);
      Serial.print("0 meters | Status: ");
      Serial.print(flightStatus);
      Serial.println("");
    }else if(i==gameY){ // Draw the drone
      for(int j=0; j<gameX; j++){
        Serial.print(" ");
      }
      Serial.println(drone);
    }
    else{
      Serial.println("");
    }
  }
  delay(400);
}

void introDraw(){
    Serial.println("");
    Serial.println("*** QUAD COPTER SIMULATOR ***");
    Serial.println("*** 2016 Kim Salmi ***");
    Serial.println("*** Creative Commons Attribution ShareAlike 3.0. ***");
    for(int i=0; i<(screenHeight-5);i++){
      Serial.println("*");
    }
    Serial.println("Please change window height to this");
    delay(7500);
}


// Axis Accelerometer
void StartBit() {
  pinMode(DIO_pin, OUTPUT);
  digitalWrite(CS_pin, LOW);
  digitalWrite(CLK_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(DIO_pin, HIGH);
  digitalWrite(CLK_pin, HIGH);
  delayMicroseconds(1);
}

void ShiftOutNibble(byte DataOutNibble) {
  for(int i = 3; i >= 0; i--) {
    digitalWrite(CLK_pin, LOW);
    if ((DataOutNibble & (1 << i)) == (1 << i)) {  // DataOutNibble AND 1 x 2^i Equals 1 x 2^i ?
      digitalWrite(DIO_pin, HIGH);      
      }
    else {
      digitalWrite(DIO_pin, LOW);
      } // with CLK rising edge the chip reads the DIO from arduino in
    digitalWrite(CLK_pin, HIGH); // data rate is f_clk 2.0 Mhz --> 0,5 micro seeconds 
    delayMicroseconds(1); 
  }
}

void SampleIt() {
  digitalWrite(CLK_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(CLK_pin, HIGH);
  delayMicroseconds(1);

  pinMode(DIO_pin, INPUT);
  digitalWrite(CLK_pin, LOW);
  delayMicroseconds(1);
  digitalWrite(CLK_pin, HIGH);
  if (digitalRead(DIO_pin)== LOW) {
    }
}

byte ShiftInNibble() {
  byte resultNibble;
  resultNibble = 0;

    for(int i = 3 ; i >= 0; i--) { // The chip Shift out results on falling CLK 
      digitalWrite(CLK_pin, LOW);
      delayMicroseconds(1);      
      if( digitalRead(DIO_pin) == HIGH) { // BIT set or not?
        resultNibble += 1 << i; // Store 1 x 2^i in our ResultNibble
      }
      else {
        resultNibble += 0 << i;
      }
      digitalWrite(CLK_pin, HIGH);
    }
return resultNibble;
}

void EndBit() {
  digitalWrite(CS_pin, HIGH);
  digitalWrite(CLK_pin, HIGH);
}

int GetValue(byte Command) { // x = B1000, y = B1001, z = B1010
  int Result = 0;
  StartBit();
  ShiftOutNibble(Command);
  SampleIt();
  Result =  2048 - ((ShiftInNibble() << 8) + (ShiftInNibble() << 4) + ShiftInNibble());
  EndBit();

  return Result;
}


void setup() {
  Serial.begin(9600);
  pinMode(CS_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  pinMode(DIO_pin, OUTPUT);
  pinMode(ledFlight, OUTPUT);
  pinMode(ledMax, OUTPUT);
  // initialize device & reset
  digitalWrite(CS_pin,LOW);
  digitalWrite(CLK_pin,LOW);
  delayMicroseconds(1);
  digitalWrite(CS_pin, HIGH);
  digitalWrite(CLK_pin,HIGH);
  
  aX = GetValue(B1000);
  aY = GetValue(B1001);
  aZ = GetValue(B1010);

  //Set default position (could be controlled by a joystick)
  joyY = aY;
  joyX = aX;
  introDraw();
}


void loop() {
  aX = GetValue(B1000);
  aY = GetValue(B1001);
  aZ = GetValue(B1010);

  if((aY-joyY)>10){
    flyUp();
  }
  if((aY-joyY)<-10){
    flyDown();  
  }

  if(0<screenHeight-gameY-2){ // are we flying?
    digitalWrite(ledFlight, HIGH);
    
    if((aX-joyX)>10){
      flyLeft();
    }
    if((aX-joyX)<-10){
      flyRight();  
    }
  }

  drawScreen();
  digitalWrite(ledFlight, LOW);
  digitalWrite(ledMax, LOW);
}


