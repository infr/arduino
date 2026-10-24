/*
Alarm with HC-SR04 ultrasonic sensor
The alarm is always on. When something comes closer than 1 m,
the buzzer beeps for 2 seconds. It can trigger again once the area is clear.
The circuit:
  * Vcc connection of the sensor attached to +5V
  * Gnd connection of the sensor attached to ground
  * Trig connection of the sensor attached to digital pin 7
  * Echo connection of the sensor attached to digital pin 8
  * Active buzzer long leg (+) to digital pin 5, short leg (-) to ground
*/

const int TRIG_PIN = 7;
const int ECHO_PIN = 8;
const int BUZZER_PIN = 5;

const long ALARM_DISTANCE_CM = 100;
const int HITS_TO_TRIGGER = 3;
const unsigned long ECHO_TIMEOUT_US = 30000;
const unsigned long MEASURE_INTERVAL_MS = 60;
const unsigned long BEEP_INTERVAL_MS = 150;
const unsigned long ALARM_DURATION_MS = 2000;
const unsigned long PRINT_INTERVAL_MS = 250;

// Set to true for a passive buzzer, which needs a tone instead of plain on/off
const bool PASSIVE_BUZZER = false;
const int BUZZER_HZ = 2000;

bool triggered = false;
bool waitingForClear = false;
bool beepOn = false;
int hits = 0;
int misses = 0;
unsigned long lastMeasure = 0;
unsigned long lastBeep = 0;
unsigned long lastPrint = 0;
unsigned long triggeredAt = 0;

void setup() {
  Serial.begin(9600);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
  unsigned long now = millis();

  if (now - lastMeasure >= MEASURE_INTERVAL_MS) {
    lastMeasure = now;
    long distance = measureCm();
    bool close = distance > 0 && distance < ALARM_DISTANCE_CM;
    // Count up and down instead of resetting, so a single dropped echo doesn't start over
    hits = close ? min(hits + 1, HITS_TO_TRIGGER) : max(hits - 1, 0);
    misses = close ? max(misses - 1, 0) : min(misses + 1, HITS_TO_TRIGGER);

    if (now - lastPrint >= PRINT_INTERVAL_MS) {
      lastPrint = now;
      if (distance > 0) {
        Serial.print(distance);
        Serial.println(" cm");
      } else {
        Serial.println("-");
      }
    }

    if (!triggered && !waitingForClear && hits >= HITS_TO_TRIGGER) {
      triggered = true;
      triggeredAt = now;
      Serial.print("ALARM at ");
      Serial.print(distance);
      Serial.println(" cm");
    } else if (waitingForClear && misses >= HITS_TO_TRIGGER) {
      waitingForClear = false;
      Serial.println("Clear");
    }
  }

  if (triggered && now - triggeredAt >= ALARM_DURATION_MS) {
    triggered = false;
    waitingForClear = true;
    Serial.println("Alarm off");
  }

  if (triggered) {
    if (now - lastBeep >= BEEP_INTERVAL_MS) {
      lastBeep = now;
      beepOn = !beepOn;
      setBuzzer(beepOn);
    }
  } else if (beepOn) {
    beepOn = false;
    setBuzzer(false);
  }
}

// Returns the distance in cm, or -1 when nothing is in range
long measureCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, ECHO_TIMEOUT_US);
  if (duration == 0) {
    return -1;
  }
  return duration / 58;
}

void setBuzzer(bool on) {
  if (PASSIVE_BUZZER) {
    if (on) {
      tone(BUZZER_PIN, BUZZER_HZ);
    } else {
      noTone(BUZZER_PIN);
    }
  } else {
    digitalWrite(BUZZER_PIN, on);
  }
}
