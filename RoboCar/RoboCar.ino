/*
 *  RoboCar.ino
 * 
 *  Object avoiding robot car using arduino uno platform
*/

#include "SR04.h"


//  Set Caution Distance bounds
//  - any distance within bounds (inclusive) is considered Caution
//  - any distance less than min is considered Danger
//  - any distance greater than max is considered Safe
#define MIN 15
#define MAX 30

// Set robot movement speed
// NOTE: positive integer up to 127, for double speed to be no greater than 255
#define SPEED 90
#define DOUBLE_SPEED SPEED*2  

// SR04 Sensors
#define TRIG_PIN_CENTER 12
#define ECHO_PIN_CENTER 11
#define TRIG_PIN_RIGHT 10
#define ECHO_PIN_RIGHT 13
#define TRIG_PIN_LEFT 2
#define ECHO_PIN_LEFT 6

// LEDs
#define LEFT_LED 1
#define RIGHT_LED 0

// Motors

// Side A
#define EN_A 9
#define IN1 8
#define IN2 7

// Side B
#define EN_B 3
#define IN3 5
#define IN4 4

// Distance variables
long front_distance;
long right_distance;
long left_distance;

// Create SR04 objects
SR04 front_sensor = SR04(ECHO_PIN_CENTER, TRIG_PIN_CENTER);
SR04 right_sensor = SR04(ECHO_PIN_RIGHT, TRIG_PIN_RIGHT);
SR04 left_sensor = SR04(ECHO_PIN_LEFT, TRIG_PIN_LEFT);

void setup() {

  pinMode(EN_A, OUTPUT);
  pinMode(EN_B, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(LEFT_LED, OUTPUT);
  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(LEFT_LED, LOW);
  
  // blink leds 10 times to indicate start up
  for (int i = 0; i < 10; i++) {
    blink_leds(RIGHT_LED, LEFT_LED, 200);
  }  

}

void loop() {
  front_distance = front_sensor.Distance();
  right_distance = right_sensor.Distance();
  left_distance = left_sensor.Distance();

  handle_object_distance(front_distance, right_distance, left_distance);

}


// Helper Functions

// EFFECTS: Moves robot foward at given speed
void moveFoward(int spd) {
  // Motor Side A Foward
  analogWrite(EN_A, spd);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  
  // Motor Side B Foward
  analogWrite(EN_B, spd);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH);
}

// EFFECTS: Moves robot reverse at given speed
void moveReverse(int spd) {
  // Motor Side A Reverse
  analogWrite(EN_A, spd);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  // Motor Side B Reverse
  analogWrite(EN_B, spd);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

// EFFECTS: Turns robot left at input speed
void turnLeft(int spd) {
  // Motor Side A Reverse
  analogWrite(EN_A, spd);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);  
  // Motor Side B Foward
  analogWrite(EN_B, spd);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, HIGH);
}

// EFFECTS: Turns robot right at input speed
void turnRight(int spd) {
  // Motor Side A Foward
  analogWrite(EN_A, spd);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);  
  // Motor Side B Reverse
  analogWrite(EN_B, spd);
  digitalWrite(IN3, HIGH); 
  digitalWrite(IN4, LOW);
}

// EFFECTS: Stops robot 
void stop() {
  // Motor Side A Stop
  analogWrite(EN_A, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);  
  // Motor Side B Stop
  analogWrite(EN_B, 0);
  digitalWrite(IN3, LOW); 
  digitalWrite(IN4, LOW);
}

// RETURNS: true if distance is in danger zone
bool in_danger(int distance) {
  return distance < MIN;
}

// RETURNS: true if distance is in safe zone
bool in_safe(int distance) {
  return distance > MAX;
}

// RETURNS: true if distance is in caution zone
bool in_caution(int distance) {
  return !in_danger(distance) && !in_safe(distance);
}

// EFFECTS: set direction based on distance to object
void handle_object_distance(int front, int right, int left) {
  // Danger Any
  if (in_danger(front) || in_danger(right) || in_danger(left)) {
    // stop, debug LEDs on,  and back up double speed
    stop();
    digitalWrite(RIGHT_LED, HIGH);
    digitalWrite(LEFT_LED, HIGH);    
    moveReverse(DOUBLE_SPEED);
  }
  // Safe All
  else if (in_safe(front) && in_safe(right) && in_safe(left)) {
    // debug LEDs off and full speed ahead
    digitalWrite(RIGHT_LED, LOW);
    digitalWrite(LEFT_LED, LOW);
    moveFoward(SPEED);
  }
  // Caution All
  else if (in_caution(front) && in_caution(right) && in_caution(left)) {
    // debug LEDs on and back up normal speed
    digitalWrite(RIGHT_LED, HIGH);
    digitalWrite(LEFT_LED, HIGH);
    moveReverse(SPEED);
  }
  // Caution Front, Right Safe, Left Safe
  else if (in_caution(front) && in_safe(right) && in_safe(left)) {
    // stop and pick longest safe path of left or right
    stop();
    if (left > right) {
      turnLeft(DOUBLE_SPEED);
    }
    else {
      turnRight(DOUBLE_SPEED);
    }
  }
  // Safe Front, Right Caution, Left Caution
  else if (in_safe(front) && in_caution(left) && in_caution(right)) {
    // debug lights on and back up normal speed
    digitalWrite(RIGHT_LED, HIGH);
    digitalWrite(LEFT_LED, HIGH);
    moveReverse(SPEED);
  }
  // Any Front, Right Caution, Left Safe
  else if (in_caution(right) && in_safe(left)) {
    // turn on right LED and turn left
    digitalWrite(RIGHT_LED, HIGH);
    digitalWrite(LEFT_LED, LOW);
    turnLeft(DOUBLE_SPEED);
  }
  // Any Front, Right Safe, Left Caution
  else if (in_caution(left) && in_safe(right)) {
    // turn on left LED and turn right
    digitalWrite(RIGHT_LED, LOW);
    digitalWrite(LEFT_LED, HIGH);
    turnRight(DOUBLE_SPEED);
  }
  // Unexpected condition
  else {
    // stop robot and blink debug leds 5 times
    stop();
    for (int i = 0; i < 5; i++) {
      blink_leds(RIGHT_LED, LEFT_LED, 100);
    }    
  }
}

void blink_led(int led, int dtime) {
  digitalWrite(led, HIGH);
  delay(dtime);
  digitalWrite(led, LOW);
  delay(dtime);
}

void blink_leds(int led1, int led2, int dtime) {
  digitalWrite(led1, HIGH);
  digitalWrite(led2, HIGH);
  delay(dtime);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  delay(dtime);
}
