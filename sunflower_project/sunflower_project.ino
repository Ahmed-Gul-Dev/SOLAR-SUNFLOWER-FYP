#define led 13
#define pls 9
#define dir 11
#define r1 4
#define r2 6
#define flowsensor 3

#define topLDR A3
#define botLDR A2
#define servoFeedback A1

int top = 0, bot = 0;  //LDR sensors
int pulses = 0;
unsigned long pmillis = 0;

void count() {
  pulses++;
}

void setup() {
  Serial.begin(9600);
  Serial.println("Success.");

  pinMode(flowsensor, INPUT);
  attachInterrupt(digitalPinToInterrupt(flowsensor), count, FALLING);
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  pinMode(pls, OUTPUT);
  digitalWrite(pls, LOW);
  pinMode(dir, OUTPUT);
  digitalWrite(dir, LOW);
  pinMode(r1, OUTPUT);
  digitalWrite(r1, HIGH);
  pinMode(r2, OUTPUT);
  digitalWrite(r2, HIGH);
  delay(100);
  while (analogRead(servoFeedback) < 610) {
    digitalWrite(r1, LOW);
  }
  digitalWrite(r1, HIGH);


  // Serial.println("Back to origin");
  // digitalWrite(dir, HIGH);
  // for (int i = 0; i < 180; i++) {
  //   digitalWrite(pls, HIGH);
  //   delay(20);
  //   digitalWrite(pls, LOW);
  //   delay(20);
  // }
  // Serial.println("reached origin");
}

int open = 0, close = 1;
int flowpulses = 0;
bool windfault = false;
void loop() {

  if (analogRead(topLDR) > 900 && analogRead(botLDR) > 900 && close == 1) {
    open = 1;
    close = 0;
    digitalWrite(dir, LOW);
    for (int i = 0; i < 180; i++) {
      digitalWrite(pls, HIGH);
      delay(20);
      digitalWrite(pls, LOW);
      delay(20);
    }
  } else if (analogRead(topLDR) < 300 && analogRead(botLDR) < 300 && open == 1) {
    open = 0;
    close = 1;
    backtoOrigin();
    originServo();
  }
  delay(250);
  Serial.print(analogRead(topLDR));
  Serial.print("\t");
  Serial.println(analogRead(botLDR));
  // // Serial.print("\t");
  // // // Serial.println(pulses);
  // Serial.print("\t");
  // Serial.println(analogRead(servoFeedback));

  if (open == 1) {
    if (pulses > 10) {
      pulses = 0;
      backtoOrigin();
      originServo();
      pmillis = millis();
      while (millis() - pmillis < 5000) {
        if (pulses > 10) {
          pulses = 0;
          pmillis = millis();
        }
      }
      digitalWrite(dir, LOW);
      for (int i = 0; i < 180; i++) {
        digitalWrite(pls, HIGH);
        delay(20);
        digitalWrite(pls, LOW);
        delay(20);
      }
      while (analogRead(servoFeedback) > 240) {
        digitalWrite(r2, LOW);
      }
      digitalWrite(r2, HIGH);
      pulses = 0;
    }

    if (analogRead(topLDR) > 900) {
      digitalWrite(r2, LOW);
    } else {
      digitalWrite(r2, HIGH);
    }

    if (analogRead(botLDR) > 900) {
      digitalWrite(r1, LOW);
    } else {
      digitalWrite(r1, HIGH);
    }
  }
}

void originServo() {
  while (analogRead(servoFeedback) < 610) {
    digitalWrite(r1, LOW);
  }
  digitalWrite(r1, HIGH);
}

void backtoOrigin() {
  Serial.println("Back to origin");
  digitalWrite(dir, HIGH);
  for (int i = 0; i < 180; i++) {
    digitalWrite(pls, HIGH);
    delay(20);
    digitalWrite(pls, LOW);
    delay(20);
  }
}