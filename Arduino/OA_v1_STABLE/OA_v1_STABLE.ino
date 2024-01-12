#include <Servo.h>
Servo servo;
const int servoPin=2;

// UltraSound Pins
const int trigPin = 9;
const int echoPin=10;

// Motor controller
const int enRight = 3;
const int right = 4;
const int rightRev = 5; // The right reverse pin
const int enLeft = 6;
const int left = 7;
const int leftRev = 8;

#define SOUND_VELOCITY 0.034
long duration;
float distance;

const int mem_length = 90;
float memory[mem_length][2];
int mem_counter = 0;

const int spookDistance = 35;  // Distance at which the rover stops ignoring obstacles
const int revDistance=10; // Distance at which the arduino reverses
int servo_pos=90; // Initial starting angle
int increaseServoAngle = 1; // Motor step angle (keep positive)



void setup() {
  pinMode(trigPin,  OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(enRight, OUTPUT);
  pinMode(enLeft, OUTPUT);
  pinMode(right, OUTPUT);
  pinMode(rightRev, OUTPUT);
  pinMode(left, OUTPUT);
  pinMode(leftRev, OUTPUT);
  servo.attach(servoPin);
  servo.write(servo_pos);
  delay(10);
}

void loop() {
  if (servo_pos >= 135 or servo_pos <= 45) {
    increaseServoAngle = -1*increaseServoAngle;
  }

  servo_pos += increaseServoAngle;
  servo.write(servo_pos);  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  duration = pulseIn(echoPin, HIGH);
  distance = duration*SOUND_VELOCITY/2;
  
  if (distance > spookDistance) {
    analogWrite(enRight, 150);
    analogWrite(enLeft, 150);                
    digitalWrite(right, HIGH);
    digitalWrite(left, HIGH);
    digitalWrite(rightRev, LOW);
    digitalWrite(leftRev, LOW);    
  } else if (distance < revDistance) {
    analogWrite(enRight,75);
    analogWrite(enLeft, 70);
    digitalWrite(right, LOW);
    digitalWrite(left, LOW);
    digitalWrite(rightRev, HIGH);
    digitalWrite(leftRev, HIGH);
  } else {
      int max_dist=0;
      int pos;
      
      for (int i = 0; i < mem_length; i++ ) {
              if (memory[i][1] > max_dist) {
                      max_dist = memory[i][1];
                      pos = memory[i][0];
                  }
          }

      float ch_direction = map(pos, 45, 135, -50, 50);
      
      analogWrite(enRight, 100 + ch_direction);
      analogWrite(enLeft,  100 - ch_direction);

      digitalWrite(right, HIGH);
      digitalWrite(left, HIGH);
      digitalWrite(rightRev, LOW);
      digitalWrite(leftRev, LOW);    
    }

      mem_counter = (mem_counter + 1)%20;
      memory[mem_counter][0] = distance;
      memory[mem_counter][1] = servo_pos;
}