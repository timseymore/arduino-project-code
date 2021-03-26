/*  SR04 Example Usage  

    A refactored version of the Arduino example sketch.

 */

#include "SR04.h"


#define ECHO_PIN   11
#define TRIG_PIN   12
#define DELAY_TIME 1000

long distance;
SR04 sr04 = SR04(SR_ECHO_PIN, SR_TRIG_PIN);

void setup() {
   Serial.begin(9600);
}

void loop() {
   distance = sr04.Distance();
   Serial.print("Distance: ");
   Serial.print(distance);
   Serial.println("cm");
   delay(DELAY_TIME);
}
