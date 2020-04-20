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
#define LEFT_LED 22
#define RIGHT_LED 23

// Motors

// Left Side A
#define EN_A 9
#define IN1 8
#define IN2 7

// Right Side B
#define EN_B 3
#define IN3 5
#define IN4 4

// Declare helper Functions
bool in_danger(int distance);
bool in_safe(int distance);
bool in_caution(int distance);

// Define Classes
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

  void blink(int dtime) {
    on();
    delay(dtime);
    off();
    delay(dtime);
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

    // start-up code
    void init() {
      // ensure motors are off
      stop();

      // blink leds 10 times to indicate start-up
      for (int i = 0; i < 10; i++) {
        blink_leds(200);
      } 

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

    void turnRight(int speed) {
      right_motor.reverse(speed);
      left_motor.foward(speed);
    }

    void turnLeft(int speed) {
      right_motor.foward(speed);
      left_motor.reverse(speed);
    }

    int get_front_distance() {
      return this->front_sensor.Distance();
    }

        int get_right_distance() {
      return this->right_sensor.Distance();
    }

        int get_left_distance() {
      return this->left_sensor.Distance();
    }

  void blink_leds(int dtime) {
    this->right_led.on();
    this->left_led.on();
    delay(dtime);
    this->right_led.off();
    this->left_led.off();
    delay(dtime);
  }

  // EFFECTS: set direction based on distance to object
  void handle_object_distance() {
    int front = get_left_distance();
    int right = get_left_distance();
    int left = get_left_distance();
    // Danger Any
    if (in_danger(front) || in_danger(right) || in_danger(left)) {
      // stop, debug LEDs on,  and back up double speed
      stop();
      this->right_led.on();
      this->left_led.on();   
      moveReverse(DOUBLE_SPEED);
    }
    // Safe All
    else if (in_safe(front) && in_safe(right) && in_safe(left)) {
      // debug LEDs off and full speed ahead
      this->right_led.off();
      this->left_led.off();
      moveFoward(SPEED);
    }
    // Caution All
    else if (in_caution(front) && in_caution(right) && in_caution(left)) {
      // debug LEDs on and back up normal speed
      this->right_led.on();
      this->left_led.on();
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
      this->right_led.on();
      this->left_led.on();
      moveReverse(SPEED);
    }
    // Any Front, Right Caution, Left Safe
    else if (in_caution(right) && in_safe(left)) {
      // turn on right LED and turn left
      this->right_led.on();
      this->left_led.off();
      turnLeft(DOUBLE_SPEED);
    }
    // Any Front, Right Safe, Left Caution
    else if (in_caution(left) && in_safe(right)) {
      // turn on left LED and turn right
      this->right_led.off();
      this->left_led.on();
      turnRight(DOUBLE_SPEED);
    }
    // Unexpected condition
    else {
      // stop robot and blink debug leds 5 times
      stop();
      for (int i = 0; i < 5; i++) {
        blink_leds(100);
      }    
    }
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


void setup() {}

void loop() {

  robot.handle_object_distance();

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
