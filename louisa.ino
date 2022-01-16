#include <TinyWireM.h>
#include "font.h"

// Digispark, Pin 0 → I2C SDA, Pin 2 → I2C SCK, Pin 3+4 USB
// http://digistump.com/wiki/digispark/tutorials/connecting

const char *text1="] LOUISA ]";
const char *text2="^ LOUISA ^";
//const char *texts[] = { "] SWEETHEART ]", "] I LOVE YOU ]", "] DARLING ]", "] PRINCESS ]", "] HONEY ]", "] BABE ]" };
//int cur_t, max_t;

uint8_t I2C_ADDR = 0x72;
uint8_t displaybuffer[8];

uint32_t next_run;

#define BUTTON 3

#define LED_ON 1
#define LED_OFF 0

#define HT16K33_BLINK_CMD 0x80
#define HT16K33_BLINK_DISPLAYON 0x01
#define HT16K33_BLINK_OFF 0
#define HT16K33_BLINK_2HZ  1
#define HT16K33_BLINK_1HZ  2
#define HT16K33_BLINK_HALFHZ  3 
#define HT16K33_CMD_BRIGHTNESS 0xE0


void mtx_begin() {
  TinyWireM.begin(); 
  TinyWireM.beginTransmission(I2C_ADDR);
  TinyWireM.write(0x21);  // turn on oscillator
  TinyWireM.endTransmission();
}

void mtx_blinkRate(uint8_t b) {
  TinyWireM.beginTransmission(I2C_ADDR);
  if (b > 3) b = 0; // turn off if not sure
 
  TinyWireM.write(HT16K33_BLINK_CMD | HT16K33_BLINK_DISPLAYON | (b << 1));
  TinyWireM.endTransmission();
}
 
void mtx_setBrightness(uint8_t b) {
  if (b > 15) b = 15;
  TinyWireM.beginTransmission(I2C_ADDR);
  TinyWireM.write(HT16K33_CMD_BRIGHTNESS | b);
  TinyWireM.endTransmission();
}

void mtx_writeDisplay(void) {
  TinyWireM.beginTransmission(I2C_ADDR);
  TinyWireM.write((uint8_t)0x00); // start at address $00
 
  for (uint8_t i=0; i<8; i++) {
    TinyWireM.write(displaybuffer[i] & 0xFF);
    TinyWireM.write(displaybuffer[i] >> 8);
  }
  TinyWireM.endTransmission();
}

void mtx_clear(void) {
  for (uint8_t i=0; i<8; i++) {
    displaybuffer[i] = 0;
  }
}

void mtx_drawPixel(int8_t x, int8_t y, int8_t color) {
  // rotate
  x = 8-1-x;
  y = 8-1-y;

  if ((y < 0) || (y >= 8)) return;
  if ((x < 0) || (x >= 8)) return;

  // wrap around the x
  x += 7;
  x %= 8;

  if (color) {
    displaybuffer[y] |= 1 << x;
  } else {
    displaybuffer[y] &= ~(1 << x);
  }
}

void mtx_drawChar(int8_t pos_x, int8_t pos_y, int8_t c) {
  uint8_t x, y;
  unsigned char w;

  for (x=0; x<8; x++) {
    w = pgm_read_byte(&(font[c][x]));
    for (y=0; y<8; y++) {
      if (w&1) {
        mtx_drawPixel(x+pos_x, y+pos_y, LED_ON);
      }
      w=w>>1;
    }
  }
}
    
void setup() {
  randomSeed(analogRead(0));

  pinMode(1, OUTPUT);
  digitalWrite(1, HIGH);
  pinMode(BUTTON, INPUT_PULLUP);
  digitalWrite(BUTTON, HIGH);

  mtx_begin();
  mtx_blinkRate(HT16K33_BLINK_OFF);
  mtx_setBrightness(10);
  mtx_clear();
  digitalWrite(1, LOW);
  next_run = 0;
  //cur_t = 0;
  //max_t = sizeof(texts)/sizeof(char*);
}

void loop() {
  int8_t x, len;

  const char *text = text2;
  if(random(3) == 0) {
    text = text1;
  }

  //const char *text = texts[cur_t];

  mtx_clear();
  mtx_drawChar(0, 0, text[0]-32);
  mtx_writeDisplay();
  delay(1000);

  len = strlen(text);
  x = 0;
  while (-x <= 8*(len-1)) {
    mtx_clear();
    for (int8_t i = 0; i < len; i++) {
      mtx_drawChar(x+i*8, 0, text[i]-32);
    }
    mtx_writeDisplay();
    delay(100);
    x--;
  }
  delay(1000);
  mtx_clear();
  mtx_writeDisplay();

  next_run = millis() + 5*60000 + random(5*60000);
  while(digitalRead(BUTTON) == HIGH) {
    if (millis() > next_run) {
      break;
    }
    delay(100);
  }

  //cur_t++;
  //if (cur_t >= max_t) {
  //  cur_t = 0;
  //}
}
