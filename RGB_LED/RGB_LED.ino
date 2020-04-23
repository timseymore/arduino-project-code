//www.elegoo.com
//2016.12.8

// Define Pins
#define BLUE 6
#define GREEN 5
#define RED 3

// fading time between colors
#define DELAY_TIME 10 

// define variables
int redValue;
int greenValue;
int blueValue;


void setup() {
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
    digitalWrite(RED, HIGH);
    digitalWrite(GREEN, LOW);
    digitalWrite(BLUE, LOW);
}

void loop() {    

    redValue = 255; 
    greenValue = 0;
    blueValue = 0;

    // fade out red bring green full when i=255
    fade (redValue, greenValue, RED, GREEN);

    redValue = 0;
    greenValue = 255;
    blueValue = 0;

    // fade out green bring blue full when i=255
    fade (greenValue, blueValue, GREEN, BLUE);

    redValue = 0;
    greenValue = 0;
    blueValue = 255;

    // fade out blue bring red full when i=255
    fade (blueValue, redValue, BLUE, RED);

}


void fade (int out_val, int in_val, int out_pin, int in_pin) {
    for(int i = 0; i < 255; i += 1) {
        out_val -= 1;
        in_val += 1;
        analogWrite(out_pin, out_val);
        analogWrite(in_pin, in_val);
        delay(DELAY_TIME);
    }

}

