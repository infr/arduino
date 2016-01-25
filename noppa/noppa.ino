int led = 13;
int extLed = 12;
int ledMorse = 11;

void setup() {
  // put your setup code here, to run once:
  pinMode(led, OUTPUT);
  pinMode(extLed, OUTPUT);
  pinMode(ledMorse, OUTPUT);
}

void loop() {
  efekti(25);
  // Heitetään noppaa ja näytetään tulos
  int noppa = random(1, 6);
  delay(2000);
  intToLed(noppa);
  delay(1000);
  efekti(15);
  delay(1000);
  // Morsetetaan vielä tulos
  digitalWrite(ledMorse, HIGH);
  delay(2000);
  intToMorse(noppa);
  delay(1000);
  digitalWrite(ledMorse, LOW);
  delay(1000);
}

void intToLed(int a){
    for(int i = 0; i<a; i++){
      digitalWrite(extLed, HIGH);
      delay(500);
      digitalWrite(extLed, LOW);
      delay(500);
  }
  
}

void efekti(int a){
    for(int i = 0; i<a; i++){
    digitalWrite(led, HIGH);
    digitalWrite(extLed, HIGH);
    digitalWrite(ledMorse, HIGH);
    delay(40);
    digitalWrite(led, LOW);
    digitalWrite(extLed, LOW);
    digitalWrite(ledMorse, LOW);
    delay(40);
  }
}

void intToMorse(int i){

    switch(i){
      case 1:
        morseShort();
        morseLong();
        morseLong();
        morseLong();
        morseLong();
      break;
      case 2:
        morseShort();
        morseShort();
        morseLong();
        morseLong();
        morseLong();
        break;
    case 3:
        morseShort();
        morseShort();  
        morseShort();
        morseLong();
        morseLong();
      break;
       case 4:
        morseShort();
        morseShort();  
        morseShort();
        morseShort();
        morseLong();
      break;
      case 5:
        morseShort();
        morseShort();  
        morseShort();
        morseShort();
        morseShort();
      break;
       case 6:
        morseLong();
        morseShort();  
        morseShort();
        morseShort();
        morseShort();
      break;
      
    }
}

void morseShort(){
    digitalWrite(extLed, HIGH);
    delay(300);
    digitalWrite(extLed, LOW);
    delay(1000);
}

void morseLong(){
    digitalWrite(extLed, HIGH);
    delay(1500);
    digitalWrite(extLed, LOW);
    delay(1000);
}

