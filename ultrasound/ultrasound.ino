/*
Tested with HY-SRF05, HC-SR04
Assuming a room temp of 20 degrees centigrade
The circuit:
  * VVC connection of the sensor attached to +5V
  * GND connection of the sensor attached to ground
  * TRIG connection of the sensor attached to digital pin 12
        * ECHO connection of the sensor attached to digital pin 13
*/
 
const int TRIG_PIN = 12;
const int ECHO_PIN = 13;

int led1 = 6;
int led2 = 7;

 
void setup() {
  // initialize serial communication:
  Serial.begin(9600);
 
  pinMode(TRIG_PIN,OUTPUT);
  pinMode(ECHO_PIN,INPUT);
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
}
 
void loop()
{
   long duration, distanceCm, distanceIn;
 
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN,HIGH);
 
  // convert the time into a distance
  distanceCm = duration / 29.1 / 2 ;
  distanceIn = duration / 74 / 2;
 
  if (distanceCm <= 0){
    Serial.println("Out of range");
  }
  else {
    Serial.print(distanceIn);
    Serial.print("in, ");
    Serial.print(distanceCm);
    Serial.print("cm");
    Serial.println();
    if(distanceCm > 10){
      digitalWrite(led2, LOW);
      digitalWrite(led1, HIGH);
    }
    if(distanceCm < 20 && distanceCm > 10){
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
    }else{
      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
    }
  }
  delay(500);
  
}
