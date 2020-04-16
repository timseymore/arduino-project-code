/*
 *
 * LED Patterns used with snowflake ornament
 * 
 */

const int NUMSTRIPS = 4;
const int NUMLEDS = 5; // number of leds per strip

// Assign all LEDS to a pin number
int leds[NUMSTRIPS][NUMLEDS] = {
//  1   2   3   4   5
  {22, 24, 26, 28, 30},  // Strip 1
  {23, 25, 27, 29, 31},  // Strip 2
  {42, 46, 48, 50, 52},  // Strip 3
  {45, 47, 49, 51, 53}  // Strip 4

};




////////////////////
// SET UP AND RUN //
////////////////////

void setup() {

  allOff(leds);

}

void loop() {

//  allFull(leds);

//  sparkleAll(leds);

// chaser(leds, 25);

//  chaseBack(leds, 70);

}



// HELPER FUNCTIONS

// all LEDS at val brightness
void writeAll(int leds[NUMSTRIPS][NUMLEDS], int val) {
  for (int j = 0; j < NUMSTRIPS; j++) {
      for (int i = 0; i < NUMLEDS; i++) {
        int temp = leds[j][i];
        analogWrite(temp, val);
      }
  }

}

// all LEDs off
void allOff(int leds[NUMSTRIPS][NUMLEDS]) {
  writeAll(leds, 255);
}

 // all LEDs full brightness
void allFull(int leds[NUMSTRIPS][NUMLEDS]) {
  writeAll(leds, 0);
}

// blink all LEDS for given ms
void allBlinkOnce(int leds[NUMSTRIPS][NUMLEDS], int ms) {
  allFull(leds);
  delay(ms);
  allOff(leds);
  delay(ms);  
}

// sparkle all leds
void sparkleAll(int leds[NUMSTRIPS][NUMLEDS]) {
  allBlinkOnce(leds, 60);
  allBlinkOnce(leds, 40);
  allBlinkOnce(leds, 20);
  delay(40);
}

// chase one strip at a time
void chaserStrip(int leds[NUMSTRIPS][NUMLEDS], int t) {
  for (int i = 0; i < NUMSTRIPS; i++) {
    for (int j = 0; j < NUMLEDS; j++) {
      analogWrite(leds[i][j], 0);
      delay(t);
      analogWrite(leds[i][j], 255);
    }
  }
}

// TODO: implement
// chase all strips at same time
void chaser(int leds[NUMSTRIPS][NUMLEDS], int t) {

  for (int i = 0; i < NUMLEDS; i++) {
    for (int j = 0; j < NUMSTRIPS; j++) {
      analogWrite(leds[j][i], 0);
      delay(t);
      analogWrite(leds[j][i], 255);
    }
  }
}

// TODO: reformat to work with multi dimensional array
// chase back and forth
void chaseBack(int leds[NUMSTRIPS][NUMLEDS], int t) {
    for (int i = 0; i < NUMLEDS; i++) {
    analogWrite(leds[i], 0);
    delay(t);
    analogWrite(leds[i], 255);
  }
  for (int j = 0; j < NUMLEDS; j++) {
    analogWrite(leds[(NUMLEDS-1)-j], 0);
    delay(t);
    analogWrite(leds[(NUMLEDS-1)-j], 255);
  }
}


//////////////////////////////////////////////////
// PWM Functions not used with current hardware //
//////////////////////////////////////////////////

// fade all LEDs down at given delay speed
void allFadeDown(int leds[NUMSTRIPS][NUMLEDS], float t) {
  for (int i = 0; i < 255; i++)  {
    writeAll(leds, i);
    delay(t);
  }  
}

// fade all LEDs up at given delay speed
void allFadeUp(int leds[NUMSTRIPS][NUMLEDS], float t) {
  for (int j = 255; j > 0; j--) {
    writeAll(leds, j);
    delay(t);
  }
}

// fade one led up at given delay speed
void oneFadeUp(int led, float t) {
  for (int j = 255; j > 0; j--) {
    analogWrite(led, j);
    delay(t);
  }
}

// fade one led down at given delay speed
void oneFadeDown(int led, float t) {
  for (int i = 0; i < 255; i++)  {
    analogWrite(led, i);
    delay(t);
  }  
}


// TODO: reformat to work with multi dimensional array
// fade up one led at time
void chaserFade(int leds[NUMSTRIPS][NUMLEDS], int t) {
    for (int i = 0; i < NUMLEDS; i++) {
    oneFadeUp(leds[i], t);
    oneFadeDown(leds[i], t);
    allOff(leds);
  }
} 
