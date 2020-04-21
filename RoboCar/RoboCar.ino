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
#define SPEED 125
#define DOUBLE_SPEED SPEED*2  

// SR04 Sensors
#define TRIG_PIN_CENTER 12
#define ECHO_PIN_CENTER 11
#define TRIG_PIN_RIGHT 10
#define ECHO_PIN_RIGHT 13
#define TRIG_PIN_LEFT 2
#define ECHO_PIN_LEFT 6

// LEDs
#define LEFT_LED 22
#define RIGHT_LED 23

// Motors

// Side A
#define EN_A 9
#define IN1 8
#define IN2 7

// Side B
#define EN_B 3
#define IN3 5
#define IN4 4

// TODO: Define Classes
class Led {
  private:
    int pin;
  
  public:
    Led(int pin) {
      this->pin = pin;
      init();
    }

    void init() {
      pinMode(pin, OUTPUT);
      off();
    }

    void on() {
      digitalWrite(pin, HIGH);
    }

    void off() {
      digitalWrite(pin, LOW);
    }
};

class Motor {
  private:
    int en;
    int in1;
    int in2;
  
  public:
    Motor(int en, int in1, int in2) {
      this->en = en;
      this->in1 = in1;
      this->in2 = in2;
      init();
    }

    void init() {
      pinMode(en, OUTPUT);
      pinMode(in1, OUTPUT);
      pinMode(in2, OUTPUT);
      stop();
    }

    void stop() {
      analogWrite(en, 0);
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);  
    }

    void foward(int speed) {
      analogWrite(en, speed);
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);  
    }

    void reverse(int speed) {
      analogWrite(en, speed);
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);  
    }

};

class Robot {
  private:
    SR04 front_sensor;
    SR04 right_sensor;
    SR04 left_sensor;
    Led right_led;
    Led left_led;
    Motor right_motor;
    Motor left_motor;

  public:
    Robot(SR04& front_sensor, SR04& right_sensor, SR04& left_sensor,
           Led& right_led, Led& left_led,
           Motor& right_motor, Motor& left_motor)
    : front_sensor(front_sensor)
    , right_sensor(right_sensor)
    , left_sensor(left_sensor)
    , right_led(right_led)
    , left_led(left_led)
    , right_motor(right_motor)
    , left_motor(left_motor)
     {      
      init();
    }

    void init() {
      stop();
    }

    void moveFoward(int speed) {
      right_motor.foward(speed);
      left_motor.foward(speed);
    }

    void moveReverse(int speed) {
      right_motor.reverse(speed);
      left_motor.reverse(speed);
    }

    void stop() {
      right_motor.stop();
      left_motor.stop();
    }

    turnRight(int speed) {
      right_motor.reverse(speed);
      left_motor.foward(speed);
    }

    turnLeft(int speed) {
      right_motor.foward(speed);
      left_motor.reverse(speed);
    }

};


// Create SR04 objects
SR04 front_sensor = SR04(ECHO_PIN_CENTER, TRIG_PIN_CENTER);
SR04 right_sensor = SR04(ECHO_PIN_RIGHT, TRIG_PIN_RIGHT);
SR04 left_sensor = SR04(ECHO_PIN_LEFT, TRIG_PIN_LEFT);

// Create LED objects
Led right_led(RIGHT_LED);
Led left_led(LEFT_LED);

// Create Motor objects
Motor motor_a(EN_A, IN1, IN2);
Motor motor_b(EN_B, IN3, IN4);

// Create robot object
Robot robot(front_sensor, right_sensor, left_sensor,
            right_led, left_led, motor_b, motor_a);

// Distance variables
long front_distance;
long right_distance;
long left_distance;


void setup() {
  
  // blink leds 10 times to indicate start up
  for (int i = 0; i < 10; i++) {
    blink_leds(right_led, left_led, 200);
  }  

}

void loop() {
  front_distance = front_sensor.Distance();
  right_distance = right_sensor.Distance();
  left_distance = left_sensor.Distance();

  handle_object_distance(front_distance, right_distance, left_distance);

}


// Helper Functions

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
    robot.stop();
    right_led.on();
    left_led.on();   
    robot.moveReverse(DOUBLE_SPEED);
  }
  // Safe All
  else if (in_safe(front) && in_safe(right) && in_safe(left)) {
    // debug LEDs off and full speed ahead
    right_led.off();
    left_led.off();
    robot.moveFoward(SPEED);
  }
  // Caution All
  else if (in_caution(front) && in_caution(right) && in_caution(left)) {
    // debug LEDs on and back up normal speed
    right_led.on();
    left_led.on();
    robot.moveReverse(SPEED);
  }
  // Caution Front, Right Safe, Left Safe
  else if (in_caution(front) && in_safe(right) && in_safe(left)) {
    // stop and pick longest safe path of left or right
    robot.stop();
    if (left > right) {
      robot.turnLeft(DOUBLE_SPEED);
    }
    else {
      robot.turnRight(DOUBLE_SPEED);
    }
  }
  // Safe Front, Right Caution, Left Caution
  else if (in_safe(front) && in_caution(left) && in_caution(right)) {
    // debug lights on and back up normal speed
    right_led.on();
    left_led.on();
    robot.moveReverse(SPEED);
  }
  // Any Front, Right Caution, Left Safe
  else if (in_caution(right) && in_safe(left)) {
    // turn on right LED and turn left
    right_led.on();
    left_led.off();
    robot.turnLeft(DOUBLE_SPEED);
  }
  // Any Front, Right Safe, Left Caution
  else if (in_caution(left) && in_safe(right)) {
    // turn on left LED and turn right
    right_led.off();
    left_led.on();
    robot.turnRight(DOUBLE_SPEED);
  }
  // Unexpected condition
  else {
    // stop robot and blink debug leds 5 times
    robot.stop();
    for (int i = 0; i < 5; i++) {
      blink_leds(right_led, left_led, 100);
    }    
  }
}

void blink_led(Led led, int dtime) {
  led.on();
  delay(dtime);
  led.off();
  delay(dtime);
}

void blink_leds(Led led1, Led led2, int dtime) {
  led1.on();
  led2.on();
  delay(dtime);
  led1.off();
  led2.off();
  delay(dtime);
}
