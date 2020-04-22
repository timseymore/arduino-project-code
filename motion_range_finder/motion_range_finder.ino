/* 
 * Motion Sensor and Range Finder with LCD Display 
 */

#include <LiquidCrystal.h>


// Define pins for LCD display
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

// Define sensor pins
#define TRIG_PIN 10
#define ECHO_PIN 9
#define PIR_PIN 8
#define DETECTED_LED 11
#define READY_LED 12
#define WAIT_LED 13

// TODO: Implement OOP structure
class System {
  private:
    LiquidCrystal lcd;

  public:
    System (LiquidCrystal lcd)
    :lcd(lcd)
    {
      init();
    };

    void init () {
      lcd.begin(16, 2);
      printWarmupMessage();
    }

}

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

  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(DETECTED_LED, OUTPUT);
  pinMode(READY_LED, OUTPUT);
  pinMode(WAIT_LED, OUTPUT);
  pinMode(PIR_PIN, INPUT);

  digitalWrite(DETECTED_LED, LOW);
  digitalWrite(READY_LED, LOW);
  digitalWrite(WAIT_LED, HIGH); 

  lcd.begin(16, 2);
  printWarmupMessage();
}

void loop()
{  
  // Get sensor data
  pirValue = digitalRead(PIR_PIN);
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
  digitalWrite(READY_LED, LOW);
  digitalWrite(DETECTED_LED, HIGH);
  // Display triggered state for 3 seconds
  delay(3000);
}

// EFFECTS: returns the calculated distance in cm from HC-SR04 sensor
float calculateDistance() {
  // Write a pulse to the HC-SR04 Trigger Pin  
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);  
  // Measure the response from the HC-SR04 Echo Pin 
  float duration = pulseIn(ECHO_PIN, HIGH);  
  // Determine distance from duration
  // Using 343 metres per second as speed of sound  
  return (duration / 2) * 0.0343;
}

// EFFECTS: displays wait led and prints rearm message to screen for 6 seconds
void rearmSystem() {
  // Reset LEDs to wait position      
  digitalWrite(DETECTED_LED, LOW);
  digitalWrite(READY_LED, LOW);
  digitalWrite(WAIT_LED, HIGH);
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
  digitalWrite(WAIT_LED, LOW);
  digitalWrite(READY_LED, HIGH);
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
