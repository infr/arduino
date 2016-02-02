
/*//////////////////////////////////

QUAD COPTER SIMULATOR
2016 Kim Salmi
Creative Commons Attribution ShareAlike 3.0.

/////////////////////////////////
Orginal Axis Accelerometer code
AUTHOR:   kiilo kiilo@kiilo.org
License:  http://creativecommons.org/licenses/by-nc-sa/2.5/ch/
/////////////////////////////////*/

int CS_pin = 9;
int CLK_pin = 10;
int DIO_pin = 11;

int ledHover = 6;
int ledMax = 7;

int defaultX = 0;
int defaultY = 0;

int aX  = 0;
int aY  = 0;
int aZ  = 0;

int joyPinY = 0;
int joyPinX = 1;

// Game Settings
int screenHeight = 14;
int screenWidth = 40;
int posX = 0;
int posY = screenHeight - 2;
String flightStatus = "o";
String drone = "o-o";
bool hoverOn = 0;
int updateSpeed = 200;

int lineLength = 0;
int joyX = 0; 
int joyY = 0;


// Game code
void flyUp(){
  posY=posY-1;
  flightStatus="^";
  if(posY <= 2){
    posY = 2;
    digitalWrite(ledMax, HIGH);
    flightStatus="MAX";
  }
}

void flyDown(){
  posY=posY+1;
  flightStatus="v";
  if(posY >= screenHeight-2){
    posY = screenHeight-2;
    flightStatus="o";
  }
}


void flyLeft(){
  posX=posX-2;
  flightStatus="<";
  if(posX < 1){
    posX = 1;
  }
}
void flyRight(){
  posX=posX+2;
  flightStatus=">";
  if(posX > screenWidth-1){
    posX = screenWidth-1;
  }
}

void drawScreen(){
  Serial.println("");
  for(int i=0; i < screenHeight;i++){
    if(i==(screenHeight-1)){ // Draw the floor
      for(int a=0; a < screenWidth; a++){
        Serial.print("-");
      }
      Serial.println("");
    }else if(i==1){ // Draw statusbar
      Serial.print("Height: ");
      Serial.print(screenHeight-posY-2);
      Serial.print("0 m | Status: ");
      Serial.print(flightStatus);
      Serial.print(" | Line: ");
      Serial.print(lineLength);
      Serial.print(" | Hover: ");
      Serial.print(hoverOn);
      Serial.println("");
    }else if(i==posY){ // Draw the drone
      for(int j=0; j < posX; j++){
        Serial.print(" ");
      }
      Serial.println(drone);
    }else if((i > posY) && i < (posY + lineLength + 1)){
      for(int j=0; j < posX; j++){
        Serial.print(" ");
      }
      Serial.println(" |");
    }
    else{
      Serial.println("");
    }
  }
  delay(updateSpeed);
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
    delay(3000);
}

void lineUp(){
    lineLength = lineLength - 1;
    if(lineLength < 0){
      lineLength = 0;
    }
}

void lineDown(){
    lineLength = lineLength + 1;
    if(lineLength > (screenHeight - 3)){
        lineLength = screenHeight - 3;
    }
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
  Serial.begin(115200);
  pinMode(CS_pin, OUTPUT);
  pinMode(CLK_pin, OUTPUT);
  pinMode(DIO_pin, OUTPUT);
  pinMode(ledHover, OUTPUT);
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

  // Calibrate default
  defaultY = aY;
  defaultX = aX;
  introDraw();
}


void loop() {
  aX = GetValue(B1000);
  aY = GetValue(B1001);
  aZ = GetValue(B1010);

  
  joyY = analogRead(joyPinY);
  delayMicroseconds(10);
  joyX = analogRead(joyPinX);

  if((aY-defaultY)>10 && !hoverOn){
    flyUp();
  }
  if((aY-defaultY)<-10 && !hoverOn){
    flyDown();  
  }

  if(joyX > 1023*0.6){
    lineUp();
  }
  if(joyX < 1023*0.4){
    lineDown();
  }

  if(0 < screenHeight-posY-2){ // are we flying?
    
    if(joyY > 1023*0.6){
        digitalWrite(ledHover, HIGH);
        hoverOn = 1;
    }
    if(joyY < 1023*0.4){
        digitalWrite(ledHover, LOW);
        hoverOn = 0;
    }
    
    if((aX-defaultX)>10){
      flyLeft();
    }
    if((aX-defaultX)<-10){
      flyRight();  
    }
  }else{
    if(joyY > 1023*0.6){
        drone = "X-----X";
        updateSpeed = 400;
        screenWidth = 75;
    }
    if(joyY < 1023*0.4){
        drone = "<><>";
        updateSpeed = 100;
        screenWidth = 75;
    }
  }

  drawScreen();

  digitalWrite(ledMax, LOW);
}

