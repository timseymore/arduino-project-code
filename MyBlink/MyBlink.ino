/*
  Blink

*/

const int delay_time = 500;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
}


void loop() { 
  digitalWrite(LED_BUILTIN, HIGH);
  delay(delay_time);                    
  digitalWrite(LED_BUILTIN, LOW);
  delay(delay_time);                    
}
