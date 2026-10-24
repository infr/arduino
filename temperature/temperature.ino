/*
Tested with TI LM35AH (TO-46 metal can), 10 mV per degree centigrade
and a Keyes common anode RGB LED module (pins V R B G)
Measures +2 to 110 degrees centigrade
LED shows white under 25, yellow from 25 to 27 and red over 27 degrees
The circuit:
  * +VS connection of the sensor attached to +5V
  * GND connection of the sensor attached to ground (the metal can is also GND)
  * VOUT connection of the sensor (middle leg) attached to analog pin A5
  * V connection of the LED attached to +5V
  * R connection of the LED attached to digital pin 11
  * B connection of the LED attached to digital pin 10
  * G connection of the LED attached to digital pin 9
*/

const int SENSOR_PIN = A5;
const int RED_PIN = 11;
const int BLUE_PIN = 10;
const int GREEN_PIN = 9;
const int SAMPLES = 10;

const float WARM_C = 25.0;
const float HOT_C = 27.0;

// Uno internal reference is nominally 1.1 V but varies per board.
// Adjust this against a known temperature to calibrate.
const float REFERENCE_MV = 1100.0;

void setup() {
  Serial.begin(9600);
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  analogReference(INTERNAL);
  analogRead(SENSOR_PIN); // first reading after a reference change is unreliable
}

// Common anode: a pin turns its color on when it goes LOW, so values are inverted
void setColor(int red, int green, int blue) {
  analogWrite(RED_PIN, 255 - red);
  analogWrite(GREEN_PIN, 255 - green);
  analogWrite(BLUE_PIN, 255 - blue);
}

void loop() {
  long total = 0;
  for (int i = 0; i < SAMPLES; i++) {
    total += analogRead(SENSOR_PIN);
    delay(10);
  }

  float millivolts = (total / (float)SAMPLES) * REFERENCE_MV / 1024.0;
  float tempC = millivolts / 10.0;

  if (tempC > HOT_C) {
    setColor(255, 0, 0);
  } else if (tempC >= WARM_C) {
    setColor(255, 255, 0);
  } else {
    setColor(255, 255, 255);
  }

  Serial.print(tempC, 1);
  Serial.println(" C");
  delay(1000);
}
