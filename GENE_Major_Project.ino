#include <FastLED.h>

#define LED_TYPE WS2811
#define COLOUR_ORDER RGB
#define LED_PIN_1 3
#define LED_PIN_2 9 // 
#define NUM_CHASES 12 // The number of drops it takes to fill the fruit
#define FILL_TIME 12000 // Time it takes for the fruit to fill, in milliseconds
#define FADE_TIME 5000 // Time it takes for the exit sequence (fade out) to happen in milliseconds
#define NUM_LEDS 100 // Number of LEDs per string
#define MAX_BRIGHTNESS 255 // Maximum intensity value for each LED (could be as high as 255)
#define MIN_BRIGHTNESS 0 // Resting brightness for each LED in a branch with fruit on it
#define LEAD_LED_BR 0.6 // Leading edge LED brightness
#define TAIL1_LED_BR 0.4 // First trailing edge LED brightness
#define TAIL2_LED_BR 0.1 // Second trailing edge LED brightness
#define PULSE_BRIGHTNESS 10 // Brightness of the pulsing branches
#define PERIPHERAL_BRIGHTNESS 50 // Brightness of the surrounding LEDS in the non-drip branches
#define R_RATIO 1
#define G_RATIO 0.05
#define B_RATIO 0.05
#define FADE_TIME 2000 // Time it takes for fade out
#define MOTOR_SPEED 1.0 // Define motor speed in revolutions per second
#define MOTOR_STEPS 200 // Number of steps per revolution of the motor
#define MOTOR_TRAVEL 1600 // Number of total steps in the motor's range of motion
#define PULSE 7 // Define the pulse pin for the motor
#define DIRECTION 6 // Define the direction pin
#define ENABLE 5 // Define the motor enable pin (locks the shaft from manual turning)


float fruitFill = 0; // Percentage fullness of the fruit
int seatButton = 2; // Pin for the seat button
int green = 0;
int red = 0;
int blue = 0;
int branchBrightness = 0; // Initialize the brightness of the peripheral branches
int motorPosition = 0; // Initialize a virtual encoder for the motor

CRGBArray<NUM_LEDS> leds1;
CRGBArray<NUM_LEDS> leds2;


void setup() {
  FastLED.addLeds<LED_TYPE, LED_PIN_1, COLOUR_ORDER>(leds1, NUM_LEDS);
  FastLED.addLeds<LED_TYPE, LED_PIN_2, COLOUR_ORDER>(leds2, NUM_LEDS);
  Serial.begin(115200);
  pinMode(seatButton, INPUT_PULLUP);
  

  pinMode (PULSE, OUTPUT);
  pinMode (DIRECTION, OUTPUT);
  pinMode (ENABLE, OUTPUT);
}

void loop() {

  if (digitalRead(seatButton) == LOW){
    Serial.println(seatButton);
    for (int j = 1; j < NUM_CHASES; j++) {
      for (int i = 0; i < (NUM_LEDS+1); i++) {
        
        //leds1[i] = CRGB(G_RATIO * PULSE_BRIGHTNESS, R_RATIO * PULSE_BRIGHTNESS, B_RATIO * PULSE_BRIGHTNESS);
        if (i < NUM_LEDS - 2) {
          leds1[i] = CRGB(G_RATIO * MAX_BRIGHTNESS * LEAD_LED_BR, R_RATIO * MAX_BRIGHTNESS * LEAD_LED_BR, B_RATIO * MAX_BRIGHTNESS * LEAD_LED_BR);
        }
        if (i > 0 && i < NUM_LEDS - 1) {
          leds1[i - 1] = CRGB(G_RATIO * MAX_BRIGHTNESS, R_RATIO * MAX_BRIGHTNESS, B_RATIO * MAX_BRIGHTNESS);
        }
        if (i > 1 && i < NUM_LEDS) {
          leds1[i - 2] = CRGB(G_RATIO * MAX_BRIGHTNESS * TAIL1_LED_BR, R_RATIO * MAX_BRIGHTNESS * TAIL1_LED_BR, B_RATIO * MAX_BRIGHTNESS * TAIL1_LED_BR);
        }
        if (i > 2) {
          leds1[i - 3] = CRGB(G_RATIO * MAX_BRIGHTNESS * TAIL2_LED_BR, R_RATIO * MAX_BRIGHTNESS * TAIL2_LED_BR, B_RATIO * MAX_BRIGHTNESS * TAIL2_LED_BR);
        }
        if (i > 3) {
          leds1[i - 4] = CRGB(G_RATIO * MIN_BRIGHTNESS, R_RATIO * MIN_BRIGHTNESS, B_RATIO * MIN_BRIGHTNESS);
        }
        red = fruitFill * MAX_BRIGHTNESS;
        green = fruitFill * MAX_BRIGHTNESS;
        blue = fruitFill * MAX_BRIGHTNESS;
        //Serial.println(i);
        //leds1[NUM_LEDS-3] = CRGB(G_RATIO * MIN_BRIGHTNESS, R_RATIO * MIN_BRIGHTNESS, B_RATIO * MIN_BRIGHTNESS);
        leds1[NUM_LEDS-3] = CRGB(green,red,blue);
        leds1[NUM_LEDS-2] = CRGB(green,red,blue);
        leds1[NUM_LEDS-1] = CRGB(green,red,blue);

        //Pulse the brightness of the surrounding branches
        branchBrightness = PULSE_BRIGHTNESS * sin(i/2*6.28/(NUM_LEDS+1));
        for (int p = 0; p <= NUM_LEDS; p ++) {
          leds2[p] = CRGB(branchBrightness,branchBrightness,branchBrightness);
        }
        
        FastLED.show();
        delay(FILL_TIME / NUM_CHASES / NUM_LEDS*1.0);
                
      }
      fruitFill = fruitFill + (1.0 / NUM_CHASES);
      FastLED.show();
      //Index the motor
      for (int g = 0; g < MOTOR_TRAVEL/NUM_CHASES; g++) {
        digitalWrite(DIRECTION, HIGH);
        digitalWrite(ENABLE, HIGH);
        digitalWrite(PULSE, HIGH);
        delay(1000/MOTOR_SPEED/MOTOR_STEPS/2);
        digitalWrite(PULSE, LOW);
        delay(1000/MOTOR_SPEED/MOTOR_STEPS/2);
        motorPosition = motorPosition + 1;
      }
      //Serial.println(motorPosition);
      //RESTING PULSE CODE
    }
while (digitalRead(seatButton) ==        LOW){}
  }
  // Fade out code
  for (int f = 0; f < 100; f++) {
    red = fruitFill * MAX_BRIGHTNESS*(100-f)/100;
    green = fruitFill * MAX_BRIGHTNESS*(100-f)/100;
    blue = fruitFill * MAX_BRIGHTNESS*(100-f)/100;
    leds1[NUM_LEDS-3] = CRGB(green,red,blue);
    leds1[NUM_LEDS-2] = CRGB(green,red,blue);
    leds1[NUM_LEDS-1] = CRGB(green,red,blue);
    FastLED.show();
    for (int y = 0; y < motorPosition * 0.01; y++) {
        digitalWrite(DIRECTION, LOW);
        digitalWrite(ENABLE, HIGH);
        digitalWrite(PULSE, HIGH);
        delay(1000/MOTOR_SPEED/MOTOR_STEPS/2);
        digitalWrite(PULSE, LOW);
        delay(1000/MOTOR_SPEED/MOTOR_STEPS/2);
      }
      //Serial.println(motorPosition);
  }
  fruitFill = 0;
  motorPosition = 0;
    for (int k = 1; k < NUM_LEDS; k++) { // initialize all of the LEDs in the fruit branch with the minimum brightness
    leds1[k] = CRGB(R_RATIO * MIN_BRIGHTNESS, G_RATIO * MIN_BRIGHTNESS, B_RATIO * MIN_BRIGHTNESS);
  }
  FastLED.show();
}
