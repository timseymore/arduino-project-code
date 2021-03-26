/*  SR04 Test Code 
  
    Use with LEDs to debug a SR04 Distance sensor in the Serial Monitor.

*/

#include "SR04.h"


#define ECHO_PIN 9
#define TRIG_PIN 10

#define RED   13
#define GREEN 11
#define BLUE  12

#define CAUTION_BOUNDS 20
#define DANGER_BOUNDS 10

long distance;
SR04 sr04 = SR04(ECHO_PIN, TRIG_PIN);

void setup() { 
  Serial.begin(9600);
  digitalWrite(RED, LOW);
  digitalWrite(BLUE, LOW);
  digitalWrite(GREEN, LOW);
}


void loop() {
  distance = sr04.Distance();
  handle_led(distance);
  Serial.print("Distance: ");    
  Serial.print(distance);
  Serial.println("cm");  
}


// Switch on appropriate LED for given distance
//  - GREEN -> Safe
//  - BLUE  -> Caution
//  - RED   -> Danger
void handle_led(int dist) {
  // Danger
  if (dist < DANGER_BOUNDS) {
    digitalWrite(RED, HIGH);
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, LOW);
  }
  // Safe
  else if (dist > CAUTION_BOUNDS) {
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, LOW);
    digitalWrite(GREEN, HIGH);
  }
  // Caution
  else {
    digitalWrite(RED, LOW);
    digitalWrite(BLUE, HIGH);
    digitalWrite(GREEN, LOW);
  }
}
