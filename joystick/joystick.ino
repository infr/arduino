 int led1 = 7;
 int led2 = 6;
 int joyPin1 = 0;
 int joyPin2 = 1;
 int x = 0;
 int y = 0;
 bool led1Status = 0;       // Onko kytketty vai ei
 bool led2Status = 0;
 int blinkTimer = 500;      // Miten nopeasti vilkutetaan

 void setup() {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT); 
  Serial.begin(9600);
 }


 void loop() {
  // Luetaan joystickin asento (0-1024)
  x = analogRead(joyPin1);   
  delay(10);
  y = analogRead(joyPin2);   

  // Sytytetään/sammutetaan ledejä mikäli joystick ylhäällä tai alhaalla
  if(y>700){
     led1Status = switchStatus(led1, led1Status);
  }
  
  if(y<300){
     led2Status = switchStatus(led2, led2Status);
  }

  // Listätään/vähennetään vilkkumisnopeutta mikäli joystick sivuilla
  if(x>700){
     decreaseSpeed();
  }
  if(x<300){
    increaseSpeed();
  }

  // Vilkutetaan ledejä aiemmilla parametreilla
  if(led1Status){
    digitalWrite(led1, HIGH);
  }
  if(led2Status){
    digitalWrite(led2, HIGH);
  }
  delay(blinkTimer);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  delay(blinkTimer);
 }


void decreaseSpeed(){
  blinkTimer = blinkTimer + 50;
  Serial.print("Less speed: ");
  Serial.println(blinkTimer);
}

void increaseSpeed(){
  blinkTimer = blinkTimer - 50;
  if(blinkTimer<51){
    blinkTimer = 50;
  }
  Serial.print("More speed: ");
  Serial.println(blinkTimer);
}

bool switchStatus(int led, bool ledStatus){
    if(ledStatus){
      digitalWrite(led, LOW);
      Serial.print("Led ");
      Serial.print(led);
      Serial.println(": off");
      return 0;
    }else{
      digitalWrite(led, HIGH);
      Serial.print("Led ");
      Serial.print(led);
      Serial.println(": on");
      return 1;
    } 
}

