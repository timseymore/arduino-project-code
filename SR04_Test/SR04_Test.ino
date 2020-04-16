/* SR04 Test Code */

#include "SR04.h"


#define TRIG_PIN 10
#define ECHO_PIN 9

#define RED 13
#define YELLOW 12
#define GREEN 11 

#define MAX 20
#define MIN 10


long distance;
SR04 sr04 = SR04(ECHO_PIN,TRIG_PIN);

void setup() { 
  Serial.begin(9600);
  digitalWrite(RED, LOW);
  digitalWrite(YELLOW, LOW);
  digitalWrite(GREEN, LOW);
}


void loop() {
  Serial.println(distance);
  distance = sr04.Distance();
  handle_led(distance);   
}


// EFFECTS: switch on appropriate LED for given distance
//          GREEN  - safe
//          YELLOW - caution
//          RED    - danger
void handle_led(int dist) {
  // Danger
  if (dist < MIN) {
    digitalWrite(RED, HIGH);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, LOW);
  }
  // Safe
  else if (dist > MAX) {
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, LOW);
    digitalWrite(GREEN, HIGH);
  }
  // Caution
  else {
    digitalWrite(RED, LOW);
    digitalWrite(YELLOW, HIGH);
    digitalWrite(GREEN, LOW);
  }
}
