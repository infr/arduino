#include <Servo.h>
#include <Math.h>

Servo servo;
int joyX = 0;
int joyY = 1;

void setup() {
  // put your setup code here, to run once:
   servo.attach(7);
   Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int x = analogRead(joyX);   
  int y = analogRead(joyY);
  Serial.print("Y: ");   
  Serial.println(y);
  float kerroin = (float) y / 1023;
  float out = kerroin * 100;
  Serial.print("out: ");
  Serial.println(out);
  Serial.println();
  
  servo.write(out);
  delay(10);

}
