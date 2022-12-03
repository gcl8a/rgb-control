#include "APA102.h"

APA102 strip(1);

void ADCtoHSV(void)
{
  uint32_t hue = analogRead(A0) % 768ul; //let hue wrap around so it's more continuous
  uint32_t sat = analogRead(A1); //might need to be 32_t for later multiplication?
  uint32_t val = analogRead(A2);

  Serial.print(hue);
  Serial.print('\t');
  Serial.print(sat);
  Serial.print('\t');
  Serial.print(val);
  Serial.print('\t');

  uint32_t chroma = val * sat; //0 < chroma < 1024^2

  int32_t Hmod = hue % 256;
  uint32_t X = (chroma * (128ul - abs(Hmod - 128))) / 128ul;

  uint32_t red = 0;
  uint32_t grn = 0;
  uint32_t blu = 0;

  if(hue < 128)      {red = chroma; grn = X; blu = 0;}
  else if(hue < 256) {red = X; grn = chroma; blu = 0;}
  else if(hue < 384) {red = 0; grn = chroma; blu = X;}
  else if(hue < 512) {red = 0; grn = X; blu = chroma;}
  else if(hue < 640) {red = X; grn = 0; blu = chroma;}
  else               {red = chroma; grn = 0; blu = X;}

  uint32_t m = val * 1024ul - chroma;
  red = (red + m) / 4096;
  grn = (grn + m) / 4096;
  blu = (blu + m) / 4096;

  Serial.print(red);
  Serial.print('\t');
  Serial.print(grn);
  Serial.print('\t');
  Serial.print(blu);
  Serial.print('\t');

  //lamp.UpdateRGB(red, grn, blu);
}

void ADCtoHSVfloat(void)
{
  float hue = analogRead(A0);
  float sat = analogRead(A1) / 1024.0;
  float val = analogRead(A2) / 1024.0;

  Serial.print(hue);
  Serial.print('\t');
  Serial.print(sat);
  Serial.print('\t');
  Serial.print(val);
  Serial.print('\t');

  float Hprime = hue / (1024.0 / 6.0);
  float chroma = val * sat;

  int iH = Hprime / 2;
  float Hmod2 = Hprime - iH * 2;
  float X = chroma * (1.0 - fabs(Hmod2 - 1.0));

  float rp = 0;
  float gp = 0;
  float bp = 0;

  if(Hprime < 1.0)      {rp = chroma; gp = X; bp = 0;}
  else if(Hprime < 2.0) {rp = X; gp = chroma; bp = 0;}
  else if(Hprime < 3.0) {rp = 0; gp = chroma; bp = X;}
  else if(Hprime < 4.0) {rp = 0; gp = X; bp = chroma;}
  else if(Hprime < 5.0) {rp = X; gp = 0; bp = chroma;}
  else                  {rp = chroma; gp = 0; bp = X;}

  float m = val - chroma;
  float red = 256 * (rp + m);
  float green = 256 * (gp + m);
  float blue = 256 * (bp + m);

  Serial.print(red );
  Serial.print('\t');
  Serial.print(green);
  Serial.print('\t');
  Serial.print(blue);
  Serial.print('\n');

  //lamp.UpdateRGB(red, green, blue);
}

void ADCtoRGB(void)
{
  int red = analogRead(A0) / 4;
  int green = analogRead(A1) / 4;
  int blue = analogRead(A2) / 4;

  Serial.print(red);
  Serial.print('\t');
  Serial.print(green);
  Serial.print('\t');
  Serial.print(blue);
  Serial.print('\n');

  //lamp.UpdateRGB(red, green, blue);
}

void setup() 
{
  Serial.begin(115200);
  while(!Serial) {}
  strip.init();

  // strip[0] = APA102::makeColor(10, 0, 0);
  // strip[1] = APA102::makeColor(0, 10, 0);
  // strip[2] = APA102::makeColor(0, 0, 10);
  // strip[3] = APA102::makeColor(10, 0, 0);
  // strip[4] = APA102::makeColor(0, 10, 0);
  // strip[5] = APA102::makeColor(0, 0, 10);
  // strip[6] = APA102::makeColor(10, 0, 0);
  // strip[7] = APA102::makeColor(0, 10, 0);
  // strip[8] = APA102::makeColor(0, 0, 10);
  // strip[9] = APA102::makeColor(10, 0, 0);
  // strip[10] = APA102::makeColor(0, 10, 0);
  // strip[11] = APA102::makeColor(0, 0, 10);
  // strip[12] = APA102::makeColor(10, 0, 0);
  // strip[13] = APA102::makeColor(0, 10, 0);
  // strip[14] = APA102::makeColor(0, 0, 10);
  // strip[15] = APA102::makeColor(255, 0, 0);
  // strip[16] = APA102::makeColor(0, 255, 0);
  // strip[17] = APA102::makeColor(0, 0, 255);
  // strip[18] = APA102::makeColor(10, 0, 0);
  // strip[19] = APA102::makeColor(0, 10, 0);
  // strip[20] = APA102::makeColor(0, 10, 0);

  // strip.show();
}

void loop() 
{
  delay(20);
  uint32_t currTime = millis();
  uint16_t angle = (currTime / 20) % 360;
  uint8_t r = 100 * (1+sin(angle * 3.14 / 180.));
  uint8_t g = 100 * (1+sin((angle+120) * 3.14 / 180.));
  uint8_t b = 100 * (1+sin((angle+240) * 3.14 / 180.));
  // Some example procedures showing how to display to the pixels
  strip[0] = APA102::makeColor(r, g, b);
  strip.show();
}
