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
int enA = 9;
int in1 = 8;
int in2 = 7;

// Side B
int enB = 3;
int in3 = 5;
int in4 = 4;

// Distance variables
long front_distance;
long right_distance;
long left_distance;

// Create SR405 objects
SR04 front_sensor = SR04(ECHO_PIN_CENTER, TRIG_PIN_CENTER);
SR04 right_sensor = SR04(ECHO_PIN_RIGHT, TRIG_PIN_RIGHT);
SR04 left_sensor = SR04(ECHO_PIN_LEFT, TRIG_PIN_LEFT);

void setup() {

  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(RIGHT_LED, OUTPUT);
  pinMode(LEFT_LED, OUTPUT);

  digitalWrite(RIGHT_LED, LOW);
  digitalWrite(LEFT_LED, LOW);
  
  // blink leds 10 times to indicate start up
  for (int i = 0; i < 10; i++) {
    blink_leds(RIGHT_LED, LEFT_LED, 250);
  }
  
}

void loop() {
  front_distance = front_sensor.Distance();
  right_distance = right_sensor.Distance();
  left_distance = left_sensor.Distance();

  handle_object_distance(front_distance, right_distance, left_distance);

}


// Functions

// EFFECTS: Moves robot foward at input speed
void moveFoward(int spd) {
  // Motor A
  analogWrite(enA, spd);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  // Motor B
  analogWrite(enB, spd);
  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);
}

// EFFECTS: Moves robot in reverse at input speed
void moveReverse(int spd) {
  // Motor A
  analogWrite(enA, spd);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  // Motor B
  analogWrite(enB, spd);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

// EFFECTS: Turns robot left at input speed
void turnLeft(int spd) {
  // Motor A
  analogWrite(enA, spd);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);  
  // Motor B
  analogWrite(enB, spd);
  digitalWrite(in3, LOW); 
  digitalWrite(in4, HIGH);
}

// EFFECTS: Turns robot right at input speed
void turnRight(int spd) {
  // Motor A
  analogWrite(enA, spd);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);  
  // Motor B
  analogWrite(enB, spd);
  digitalWrite(in3, HIGH); 
  digitalWrite(in4, LOW);
}

// EFFECTS: Stops robot 
void stop() {
  // Motor A
  analogWrite(enA, 0);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  // Motor B
  analogWrite(enB, 0);
  digitalWrite(in3, LOW); 
  digitalWrite(in4, LOW);
}

// EFFECTS: set direction based on distance to object
void handle_object_distance(int front, int right, int left) {

  bool danger_front = front < MIN;
  bool caution_front = front <= MAX && front >= MIN;
  bool safe_front = front > MAX;
  bool danger_right = right < MIN;
  bool caution_right = right <= MAX && right >= MIN;
  bool safe_right = right > MAX;
  bool danger_left = left < MIN;
  bool caution_left = left <= MAX && left >= MIN;
  bool safe_left = left > MAX;

  // Danger Any
  if (danger_front || danger_right || danger_left) {
    // stop, debug LEDs on,  and back up double speed
    stop();
    digitalWrite(RIGHT_LED, HIGH);
    digitalWrite(LEFT_LED, HIGH);    
    moveReverse(DOUBLE_SPEED);
  }
  // Safe All
  else if (safe_front && safe_right && safe_left) {
    // debug LEDs off and full speed ahead
    digitalWrite(RIGHT_LED, LOW);
    digitalWrite(LEFT_LED, LOW);
    moveFoward(SPEED);
  }
  // Caution All
  else if (caution_front && caution_right && caution_left) {
    // debug LEDs on and back up normal speed
    digitalWrite(RIGHT_LED, HIGH);
    digitalWrite(LEFT_LED, HIGH);
    moveReverse(SPEED);
  }
  // Caution Front, Right Safe, Left Safe
  else if (caution_front && safe_right && safe_left) {
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
  else if (safe_front && caution_left && caution_right) {
    // debug lights on and back up normal speed
    digitalWrite(RIGHT_LED, HIGH);
    digitalWrite(LEFT_LED, HIGH);
    moveReverse(SPEED);
  }
  // Any Front, Right Caution, Left Safe
  else if (caution_right && safe_left) {
    // turn on right LED and turn left
    digitalWrite(RIGHT_LED, HIGH);
    digitalWrite(LEFT_LED, LOW);
    turnLeft(DOUBLE_SPEED);
  }
  // Any Front, Right Safe, Left Caution
  else if (caution_left && safe_right) {
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
