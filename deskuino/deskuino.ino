/*
    Deskuino

    Arduino desktop controller
*/


// Define LED Pins
#define BLUE 6
#define GREEN 5
#define RED 3

#define DELAY_TIME 10 


// Classes
class AmbientLed {
    private:
        int red_pin;
        int green_pin;
        int blue_pin;

    public:
        AmbientLed(int red, int green, int blue) {
            this->red_pin = red;
            this->green_pin = green;
            this->blue_pin = blue;
        }
}


void setup () {
    pinMode(RED, OUTPUT);
    pinMode(GREEN, OUTPUT);
    pinMode(BLUE, OUTPUT);
}

void loop () {    

}

// Set the ambient LED pins to given value
// - red_strength : how bright to light red LED (1-255)
// - blue_strength : how bright to light blue LED (1-255)
// - green_strength : how bright to light green LED (1-255)
void SetLED(int red_strength, int blue_strength, int green_strength) {
        analogWrite(RED, red_strength);
        analogWrite(GREEN, green_strength);
        analogWrite(BLUE, blue_strength);
}

// Fades the ambient LED between red, green, and blue
void FadeAmbientLed() {
    fade_between(RED, GREEN);
    fade_between(GREEN, BLUE);
    fade_between(BLUE, RED);
}

// Fade out_val from full to off while bringing in_val to full from off
// - out_pin : pin number for led to fade out
// - in_pin : pin number for led to fade in
void fade_between (int out_pin, int in_pin) {
    int out_val = 255;
    int in_val = 0;
    for (int i = 0; i < 255; i += 1) {
        out_val -= 1;
        in_val += 1;
        analogWrite(out_pin, out_val);
        analogWrite(in_pin, in_val);
        delay(DELAY_TIME);
    }
    // ensure correct brightness when exiting 
    analogWrite(out_pin, 0);
    analogWrite(in_pin, 255);

}

// Turn on one led full while other two are turned off
// - on : pin number for led to turn on
// - off1 : pin number for led to turn off
// - off2 : pin number for led to turn off
void one_full (int on, int off1, int off2) {
    analogWrite(on, 255);
    analogWrite(off1, 0);
    analogWrite(off2, 0);
}
