/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/
#ifndef DMTFTBASE_h
#define DMTFTBASE_h

#include "dm_platform.h"	


//Basic Colors
#define RED     0xf800
#define GREEN   0x07e0
#define BLUE    0x001f
#define BLACK   0x0000
#define YELLOW  0xffe0
#define WHITE   0xffff

//Other Colors
#define CYAN        0x07ff
#define BRIGHT_RED  0xf810
#define GRAY1       0x8410
#define GRAY2       0x4208


class DmTftBase {
public:
  DmTftBase(const uint16_t width, const uint16_t height) : _width(width), _height(height){};
  virtual ~DmTftBase() { };

  virtual void init(void) = 0;

  uint16_t width() { return _width; }
  uint16_t height() { return _height; }
  void setWidth(uint16_t width) {  _width = width; }
  void setHeight(uint16_t height) {  _height = height; }

  void setTextColor(uint16_t background, uint16_t foreground) { _bgColor = background; _fgColor = foreground; }

  virtual void setPixel(uint16_t x, uint16_t y, uint16_t color);
  virtual void setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) = 0;
  virtual void sendData(uint16_t data) = 0;
  
  void clearScreen(uint16_t color = BLACK);

  void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
  void drawVerticalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
  void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);

  void drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
  void fillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);

  void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
  void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

  void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

  void drawPoint(uint16_t x, uint16_t y, uint16_t radius=0);

  void drawChar(uint16_t x, uint16_t y, char ch, bool transparent);
  void drawNumber(uint16_t x, uint16_t y, int num, int digitsToShow, bool leadingZeros=false);
  void drawString(uint16_t x, uint16_t y, const char *p);
  void drawStringCentered(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *p);

  void drawImage(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* data);
  
  void select();
  void unSelect();
protected:
  virtual void sendCommand(uint8_t index) = 0;

  /* TODO : Digital out for Omega */
  DigitalOut* _pinCS;
  uint8_t _bitmaskCS;

private:
  uint16_t _width;
  uint16_t _height;

  uint16_t _bgColor;
  uint16_t _fgColor;
};
#endif
