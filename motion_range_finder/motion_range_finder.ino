/* 
 * Motion Sensor and Range Finder with LCD Display 
 * 
 * 2019 Tim Seymore
 */

#include <LiquidCrystal.h>
#include <SR04.h>


///////////////
// Constants //
///////////////

#define TRUE  0
#define FALSE 1
#define PIR_TRIG_PIN  8
#define SR_ECHO_PIN   9
#define SR_TRIG_PIN  10
#define DETECT_LED   11
#define READY_LED    12
#define WAIT_LED     13


/////////////
// Classes //
/////////////

class SystemManager {

  public:
    SystemManager ()
    {
      this->init();
    };

    void init () {
      Serial.begin(9600);
      Serial.print("Setting up System Manager...");
    }

    void run() {
      Serial.print("Running System Manager...");
    }

}; 


//////////////////////
// Global Variables //
//////////////////////

const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
SR04          sr04(SR_ECHO_PIN, SR_TRIG_PIN);
SystemManager system();

int   pirValue;
int   motionDetected;
int   timesDetected;
long  distance;
long  shortestDistance;
long  lastDistance;


///////////////////
// Setup And Run //
///////////////////

void setup() {
  
  pinMode(DETECT_LED,   OUTPUT);
  pinMode(READY_LED,    OUTPUT);
  pinMode(WAIT_LED,     OUTPUT);
  pinMode(PIR_TRIG_PIN, INPUT);

  digitalWrite(DETECT_LED, LOW);
  digitalWrite(READY_LED,  LOW);
  digitalWrite(WAIT_LED,   HIGH);
  
  motionDetected   = FALSE; 
  timesDetected    = 0;
  shortestDistance = 0; // set for a default 

  lcd.begin(16, 2);

  printWarmupMessage();

}

void loop() { 

    // Get sensor data
  pirValue = digitalRead(PIR_TRIG_PIN);
  distance = sr04.Distance();

  // Local variables
  bool isMovement   = motionDetected == TRUE;      
  bool pirTriggered = pirValue == 1;
  bool isShortest   = shortestDistance == 0 || distance < shortestDistance;

  // Check for and handle motion events
  if (pirTriggered) {        
    lastDistance = distance;
    if (isShortest) {
      shortestDistance = distance;
    } 
    handleMotionDetected(distance);    
    motionDetected = TRUE;
  }

  // Rearm after motion detected
  if (isMovement) {
    rearmSystem();
    timesDetected++;
    motionDetected = FALSE;
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

String getDots(int i) {
  String dots;
  if (i == 0) {
    dots = "";
  }
  else if (i % 2 == 0) {
    dots = "..";
  }
  else if (i % 3 == 0) {
    dots = "...";
  }
  else {
    dots = ".";
  }
  return dots;
}

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
  digitalWrite(DETECT_LED, HIGH);
  // Display triggered state for 3 seconds
  delay(3000);
}

void rearmSystem() {
  // Reset LEDs to wait position      
  digitalWrite(DETECT_LED, LOW);
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
