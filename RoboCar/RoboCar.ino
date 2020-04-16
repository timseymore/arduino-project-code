#include "SR04.h"


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

// Distance bounds
#define MAX 40
#define MIN 20

// Motors

// Side A
int enA = 9;
int in1 = 8;
int in2 = 7;

// Side B
int enB = 3;
int in3 = 5;
int in4 = 4;

// Robot movement speed
int speed;

// Distance variables
long front_distance;
long right_distance;
long left_distance;

// Create SR405 objects
SR04 front_sensor = SR04(ECHO_PIN_CENTER, TRIG_PIN_CENTER);
SR04 right_sensor = SR04(ECHO_PIN_RIGHT, TRIG_PIN_RIGHT);
SR04 left_sensor = SR04(ECHO_PIN_LEFT, TRIG_PIN_LEFT);

void setup() {

  // Set max speed here (1 - 255//2)
  // NOTE: turn speed is speed*2 >= 255
  speed = 80;  

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
    blink_leds(RIGHT_LED, LEFT_LED, 1000);
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


// EFFECTS: set direction based on distance to object
void handle_object_distance(int front, int right, int left) {
  // Danger Any
  if (front < MIN || right < MIN || left < MIN) {
    moveReverse(speed*2);
  }
  // Safe All
  else if (front > MAX && right > MAX && left > MAX) {
    moveFoward(speed);
  }
  // Any Front, Right Caution, Left Safe
  else if (right < MAX && right > MIN && left > MAX) {
    turnLeft(speed*2);
  }
  // Any Front, Right Safe, Left Caution
  else if (left < MAX && left > MIN && right > MAX) {
    turnRight(speed*2);
  }
  // Any Front, Right Caution, Left Caution
  else if (left < MAX && left > MIN && right < MAX) {
    moveReverse(speed*2);
  }
  // Caution Front, Right Safe, Left Safe
  else {
    // pick longest safe path of left or right
    if (left > right) {
      turnLeft(speed*2);
    }
    else {
      turnRight(speed*2);
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


