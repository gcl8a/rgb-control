#ifndef __APA102_H
#define __APA102_H

#include <SPI.h>
#include <vector_uC.h>

#define Int32Vector TVector<uint32_t>

class LEDStrip {};

class APA102 : public LEDStrip
{
private:
  Int32Vector pixels;

public:
  APA102(uint16_t count) : pixels(count) {} 
  
  void init(void)
  {
    SPI.begin();
    SPI.beginTransaction(SPISettings(8000000ul, MSBFIRST, SPI_MODE0));

    // start frame
    SPI.transfer(0);
    SPI.transfer(0);
    SPI.transfer(0);
    SPI.transfer(0);

    // send a bunch of zero brightness commands to turn the strand off
    for(uint16_t i = 0; i < 255; i++) 
    {
      SPI.transfer(0xe0);
      SPI.transfer(0);
      SPI.transfer(0);
      SPI.transfer(0);
    }

    SPI.endTransaction();
  }

  uint16_t countPixels(void) {return pixels.Length();}

  void show(void)
  {
    SPI.beginTransaction(SPISettings(8000000ul, MSBFIRST, SPI_MODE0));

    // start frame is 32 bits of zeros to init xfer
    SPI.transfer(0);
    SPI.transfer(0);
    SPI.transfer(0);
    SPI.transfer(0);

    for(uint16_t i = 0; i < pixels.Length(); i++)
    {
      uint8_t buffer[4];
      for(int j = 24; j >= 0; j-=8) 
      {
        uint8_t datum = pixels[i] >> j;
        SPI.transfer(datum);
      }
    }
    
    for(uint16_t j = 0; j < pixels.Length() / 2; j++) SPI.transfer(0xe0);

    SPI.endTransaction();
  }

  static uint32_t makeColor(uint32_t red, uint32_t grn, uint32_t blu) //uint32, since I'll need to shift them
  {
    uint32_t color = 0xff000000; //first byte is all 1's (lower 5 bits are intensity, but best to leave them 1's)
    color |= (blu << 16) | (grn << 8) | (red << 0);

    return color;
  }
//
//  uint32_t SetPixel(uint16_t i, uint32_t color)
//  {
//    return pixels[i] = color;
//  }

  uint32_t& operator [] (int i)
  {
    return pixels[i];
  }
};

#endif
