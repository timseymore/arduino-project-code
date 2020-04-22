// Motion Sensor

// Define constants
#define WAIT_LED  11
#define READY_LED  12
#define TRIGGERED_LED 13
#define MOTION_TRIGGER 7 

// Define Variables
int motion_detected = 0;
int motion_value;


void setup() {
  Serial.begin(9600);
  pinMode(WAIT_LED, OUTPUT);
  pinMode(READY_LED, OUTPUT);
  pinMode(TRIGGERED_LED, OUTPUT);
  pinMode(MOTION_TRIGGER, INPUT);

  // Inital 1 minute delay to stabalize sensor
  digitalWrite(TRIGGERED_LED, LOW);
  digitalWrite(READY_LED, LOW);
  digitalWrite(WAIT_LED, HIGH);
  delay(60000);
  digitalWrite(READY_LED, HIGH);
  digitalWrite(WAIT_LED, LOW);  

}

void loop() 
{
  // Get value from motion sensor
  motion_value = digitalRead(MOTION_TRIGGER);

  // See if motion detected
  if (motion_value == 1)
  {
    // Display triggered led for 3 secs
    digitalWrite(TRIGGERED_LED, HIGH);
    motion_detected = 1;
    delay(3000);
  }
    
  else
  {
    digitalWrite(TRIGGERED_LED, LOW);
  }

  // Add delay after triggering to reset sensor
  if (motion_detected == 1)    
  {
    Serial.print("Motion Detected!");
    
    // After trigger, wait 6 sec to re-arm
    digitalWrite(TRIGGERED_LED, LOW);
    digitalWrite(READY_LED, LOW);
    digitalWrite(WAIT_LED, HIGH);
    delay(6000);
    digitalWrite(READY_LED, HIGH);
    digitalWrite(WAIT_LED, LOW);
    motion_detected = 0;
  }

  
}
