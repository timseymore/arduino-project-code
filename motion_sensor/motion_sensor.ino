// Motion Sensor

// Define constants
#define wait_led  11
#define ready_led  12
#define triggered_led 13
#define motion_trigger 7 

// Define Variables
int motion_detected = 0;
int motion_value;



void setup() {
  Serial.begin(9600);
  pinMode(wait_led, OUTPUT);
  pinMode(ready_led, OUTPUT);
  pinMode(triggered_led, OUTPUT);
  pinMode(motion_trigger, INPUT);

  // Inital 1 minute delay to stabalize sensor
  digitalWrite(triggered_led, LOW);
  digitalWrite(ready_led, LOW);
  digitalWrite(wait_led, HIGH);
  delay(60000);
  digitalWrite(ready_led, HIGH);
  digitalWrite(wait_led, LOW);  

}

void loop() 
{
  // Get value from motion sensor
  motion_value = digitalRead(motion_trigger);

  // See if motion detected
  if (motion_value == 1)
  {
    // Display triggered led for 3 secs
    digitalWrite(triggered_led, HIGH);
    motion_detected = 1;
    delay(3000);
  }
    
  else
  {
    digitalWrite(triggered_led, LOW);
  }

  // Add delay after triggering to reset sensor
  if (motion_detected == 1)    
  {
    Serial.print("Motion Detected!);
    
    // After trigger, wait 6 sec to re-arm
    digitalWrite(triggered_led, LOW);
    digitalWrite(ready_led, LOW);
    digitalWrite(wait_led, HIGH);
    delay(6000);
    digitalWrite(ready_led, HIGH);
    digitalWrite(wait_led, LOW);
    motion_detected = 0;
  }

  
}


