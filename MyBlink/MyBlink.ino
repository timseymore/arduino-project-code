/*
  Blink

*/

const int DELAY_TIME = 500;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() { 
  digitalWrite(LED_BUILTIN, HIGH);
  delay(DELAY_TIME);                    
  digitalWrite(LED_BUILTIN, LOW);
  delay(DELAY_TIME);                    
}
