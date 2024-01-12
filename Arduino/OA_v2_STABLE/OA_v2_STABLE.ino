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
const int enLeft = 11;
const int left = 6;
const int leftRev = 7;

#define SOUND_VELOCITY 0.034
long duration;
float distance;

const int mem_length = 90;
float memory[mem_length][2];
int mem_counter = 0;
int max_dist;
int pos;
int base_speed = 65;

const int spookDistance = 45;  // Distance at which the rover stops ignoring obstacles
const int revDistance=20; // Distance at which the arduino reverses
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
  servo.write(servo_pos);   // Brings servo to the initial starting angle
  delay(100);
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
    analogWrite(enRight, base_speed);
    analogWrite(enLeft, base_speed);                
    digitalWrite(right, HIGH);
    digitalWrite(left, HIGH);
    digitalWrite(rightRev, LOW);
    digitalWrite(leftRev, LOW);    
  } else if (distance < revDistance) {
    analogWrite(enRight,base_speed);
    analogWrite(enLeft, base_speed);
    digitalWrite(right, LOW);
    digitalWrite(left, LOW);
    digitalWrite(rightRev, HIGH);
    digitalWrite(leftRev, HIGH);
    delay(800);
   } else {
      max_dist=0;
      pos=0;
      
      for (int i = 0; i < mem_length; i++ ) {
              if (memory[i][1] > max_dist) {
                      max_dist = memory[i][1];
                      pos = memory[i][0];
                  }
          }

      float ch_direction = map(pos, 45, 135, -20, 20);
      
      analogWrite(enRight, 0 + ch_direction);
      analogWrite(enLeft,  0 - ch_direction);
      if (pos >= 0) {
      digitalWrite(right, LOW);
      digitalWrite(left, HIGH);
      digitalWrite(rightRev, HIGH);
      digitalWrite(leftRev, LOW);  
      } 
      if (pos <= 0) {
      digitalWrite(right, HIGH);
      digitalWrite(left, LOW);
      digitalWrite(rightRev, LOW);
      digitalWrite(leftRev, HIGH);  
      } 
       
    }

      mem_counter = (mem_counter + 1) % mem_length;
      memory[mem_counter][0] = distance;
      memory[mem_counter][1] = servo_pos;
  delay(30);
}