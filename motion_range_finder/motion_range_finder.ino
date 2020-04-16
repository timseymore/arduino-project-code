/* 
 * Motion Sensor and Range Finder with LCD Display 
 */

#include <LiquidCrystal.h>


// Define pins for LCD display
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define sensor pins
#define trigPin 10
#define echoPin 9
#define pirPin 8
#define detectedLED 11
#define readyLED 12
#define waitLED 13

// Define variables
int pirValue;
int motionDetected;
float distance;
int timesDetected;
float shortestDistance;
float lastDistance;


///////////////////
// Setup And Run //
///////////////////

void setup()
{
  motionDetected = 0;
  timesDetected = 0;
  shortestDistance = 0; // set for a default

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(detectedLED, OUTPUT);
  pinMode(readyLED, OUTPUT);
  pinMode(waitLED, OUTPUT);
  pinMode(pirPin, INPUT);

  digitalWrite(detectedLED, LOW);
  digitalWrite(readyLED, LOW);
  digitalWrite(waitLED, HIGH); 

  lcd.begin(16, 2);
  printWarmupMessage();
}

void loop()
{  
  // Get sensor data
  pirValue = digitalRead(pirPin);
  distance = calculateDistance();

  // check for and handle motion events
  if (pirValue == 1) {    
    
    lastDistance = distance;
    if (shortestDistance == 0 || distance < shortestDistance) {
      shortestDistance = distance;
    } 
    handleMotionDetected(distance);    
    motionDetected = 1;
  }

  if (motionDetected == 1) {
    rearmSystem();
    timesDetected++;
    motionDetected = 0;
  } 

  // Return to ready state
  systemArmedState();

}


//////////////////////
// Helper Functions //
//////////////////////

void printWarmupMessage() {
  lcd.setCursor(2, 0); // center the text, 2 spaces from the left edge
  lcd.print("Motion Finder");
  delay(10000);
  lcd.clear();

  int count = 0;
  for (int i = 0; i < 10; i++) {
    lcd.setCursor(0, 0);
    lcd.print("Warming up");
    lcd.setCursor(10, 0);
    lcd.print(getDots(count));
    delay(5000);
    lcd.clear();
    count++;
    // keep count between 0 and 3 inclusive
    if (count > 3) {
      count = 0;
    }        
  }
}

// EFFECTS: returns string of dots according to value of c
String getDots(int c) {
  String dots;
  if (c == 0) {
    dots = "";
  }
  else if (c % 2 == 0) {
    dots = "..";
  }
  else if (c % 3 == 0) {
    dots = "...";
  }
  else {
    dots = ".";
  }
  return dots;
}

// EFFECTS: Prints alert message to display, with distance from triggered event
//          displays triggered LED 
void handleMotionDetected(float current) {
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Motion detected"); 
  lcd.setCursor(0, 1); 
  // Print current to lcd if within range
  if  (current >= 400 || current <= 2) {
      lcd.print("Out of range");
  }
  else {
    lcd.print(current);
    lcd.setCursor(13, 1);
    lcd.print("cm");
  }    
  // Display Triggered LED
  digitalWrite(readyLED, LOW);
  digitalWrite(detectedLED, HIGH);
  // Display triggered state for 3 seconds
  delay(3000);
}

// EFFECTS: returns the calculated distance in cm from HC-SR04 sensor
float calculateDistance() {
  // Write a pulse to the HC-SR04 Trigger Pin  
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);  
  // Measure the response from the HC-SR04 Echo Pin 
  float duration = pulseIn(echoPin, HIGH);  
  // Determine distance from duration
  // Using 343 metres per second as speed of sound  
  return (duration / 2) * 0.0343;
}

// EFFECTS: displays wait led and prints rearm message to screen for 6 seconds
void rearmSystem() {
  // Reset LEDs to wait position      
  digitalWrite(detectedLED, LOW);
  digitalWrite(readyLED, LOW);
  digitalWrite(waitLED, HIGH);
  // Print message to display
  lcd.setCursor(0, 0);
  lcd.clear();
  lcd.print("Rearming");
  // Add delay to reset sensor 
  delay(6000);
  lcd.clear();
}

void systemArmedState() {
  // Set LEDs to ready position
  digitalWrite(waitLED, LOW);
  digitalWrite(readyLED, HIGH);
  // Print ready screen to display
  lcd.setCursor(0, 0);
  
  lcd.print("Area Secure"); 
  lcd.setCursor(0,1);
  lcd.print("Flags:");
  lcd.setCursor(6,1);
  lcd.print(timesDetected);
  lcd.setCursor(9,1);
  lcd.print("@:");
  lcd.setCursor(11,1);
  lcd.print(shortestDistance);
}
