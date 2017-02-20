
#include <Adafruit_CircuitPlayground.h>
#include <Adafruit_NeoPixel.h>

#define BUTTON_PIN_GO   2    //to go forward in steps
#define BUTTON_PIN_BACK  3   //to go back in steps


#define PIXEL_PIN 6    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 30

volatile int cue = 0;
int stripNum= 2;   //change for number of tapes

// Parameter 1 = number of pixels in strip,  neopixel stick has 8
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream
//   NEO_GRB     Pixels are wired for GRB bitstream, correct for neopixel stick
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip), correct for neopixel stick
Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT * stripNum, 6, NEO_GRB + NEO_KHZ800);

void Dec(){
  cue = cue -1;
}

void Inc(){
  cue = cue + 1;
}


void setup() {
  pinMode(BUTTON_PIN_GO, INPUT_PULLUP);
  attachInterrupt(0, Inc, CHANGE);
  attachInterrupt(1, Dec, CHANGE);
  interrupts();
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  
}

void loop() {
  Serial.print("Cue: "); Serial.println(cue);
  show();
}

void show(){
  switch(cue){
       case 0: rainbowCycle(15);
                break;
       case 1: twoPulse(100, 20, 1, 1, 10);
                break;
       case 2: rainbowCycle(15);
                break;
       case 3: twoPulse(100, 20, 1, 1, 10);
                break;
  }
  
}

void twoPulse(int high, int low, double redMult, double blueMult, uint8_t wait){  
  for(int j=0; j<3; j++){
    int start1 = high;
    int start2 = low;
    while(start1 != low){
      for(int i = 0; i<60; i= i+2){
      
        strip.setPixelColor(i, start1*redMult, start1, start1*blueMult);
        strip.setPixelColor(i+1, start2*redMult, start2, start2*blueMult);
      }
      
      start1 = start1 - 1;
      start2 = start2 +1;
      strip.show();
      delay(wait);
    }

    while(start1!=high){
      for(int i = 0; i<60; i= i+2){
        strip.setPixelColor(i, start1*redMult, start1, start1*blueMult);
        strip.setPixelColor(i+1, start2*redMult, start2, start2*blueMult);
       }
      
      start1 = start1 +1;
      start2 = start2 -1;
      strip.show();
      delay(wait);
    }
  }
}

void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256 * 2; j++) { // 5 cycles of all colors on wheel
      for(i=0; i< 60; i++) {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
      }
      strip.show();
      delay(wait);
  }
}

uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


