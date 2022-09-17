#ifndef _LED_LAMP_H
#define _LED_LAMP_H

#include "APA102.h"

#include <vector_uC.h>
#include <event_timer.h>

#define ivector TVector<uint16_t>

const uint32_t RGB_TIMEOUT = 10000;

class LEDlamp
{
protected:
  ivector rgb; //current value, [0..256]
  
  const uint16_t threshold = 4; //amount of change needed to "take"
  EventTimer rgbTimer; //time until rgb is "frozen"
  APA102 strip = APA102(162);//, dataPin, clockPin);

public:
  LEDlamp(void) : rgb(3) {}
  void Init(void)
  {
    strip.Init();
  }

  void UpdateRGB(uint16_t r, uint16_t g, uint16_t b)
  {
    ivector rgb(3);
    rgb[0] = r;
    rgb[1] = g;
    rgb[2] = b;

    UpdateRGB(rgb);
  }
  
  void UpdateRGB(const ivector& rgbNew)
  {
    uint16_t norm = rgb.CalcL2Norm();
    
    ivector delta = rgbNew - rgb;
    uint16_t dist = delta.CalcL2Norm();

    if(dist * threshold > norm)
    {
      rgbTimer.Start(RGB_TIMEOUT);
    }

    rgbTimer.CheckExpired(); //updates timer state
    
    if(rgbTimer.IsRunning()) //if it's running, update the strip
    {
      rgb = rgbNew;
      SetStrip(rgb[0], rgb[1], rgb[2]);
    }

    Serial.print(rgb[0]);
    Serial.print('\t');
    Serial.print(rgb[1]);
    Serial.print('\t');
    Serial.print(rgb[2]);
    Serial.print('\n');
  }

  void SetStrip(int r, int g, int b)
  {
    for (uint16_t i=0; i < strip.CountPixels(); i++) 
    {
      strip[i] = strip.MakeColor(r, g, b);
    }  
    
    strip.Show();   // write all the pixels out
  }
};

#endif
