/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

#ifndef DM_TFT_RA8875_h
#define DM_TFT_RA8875_h

#include "DmTftBase.h"
#include "omegaLib/DigitalOut.h"
#include "omegaLib/spi.h"

enum RA8875Size {
    RA8875_480x272,
    RA8875_800x480
};

class DmTftRa8875 : public DmTftBase
{
public:
    DmTftRa8875(uint8_t cs = SPI_CS_PIN, uint8_t sel = NC_PIN, uint8_t miso = SPI_MISO_PIN, uint8_t mosi = SPI_MOSI_PIN, uint8_t clk = SPI_CLK_PIN);

    virtual ~DmTftRa8875();
    virtual void init(); 
    void w25CtrlByRa8875(void);
    void w25CtrlByMCU(void);
    void clearScreen(uint16_t color = BLACK);

    void setFontColor(uint16_t background,uint16_t foreground);
    void setFontZoom(uint8_t Hsize, uint8_t Vsize);
    void eableKeyScan(bool on);
    bool isKeyPress(void);
    uint8_t getKeyValue(void);
    void backlightOn(bool on);
    void backlightAdjust(uint8_t value);
    void drawImageContinuous(uint32_t startaddress, uint32_t count, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void drawImageBlock(uint32_t startaddress, uint32_t count, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t pic_width, uint16_t block_width, uint16_t block_height);
    uint16_t width(void);
    uint16_t height(void);
    void softReset(void);
    
    void drawPoint(uint16_t x, uint16_t y, uint16_t radius=0);
    void drawNumber(uint16_t x, uint16_t y, int num, int digitsToShow, bool leadingZeros=false);
    void drawString(uint16_t x, uint16_t y, const char *p);
    void drawStringCentered(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *p);
    
    void drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);    
    void fillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    
    void drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color);
    void drawVerticalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
    void drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color);
    
    void drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);
    void fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color);

    void drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);   
    void fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);   
    
    void drawEllipse(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint16_t color);
    void fillEllipse(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint16_t color);
    
    void drawCurve(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color);
    void fillCurve(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color);
    
    void drawRoundRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t r1, uint16_t r2, uint16_t color);
    void fillRoundRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t r1, uint16_t r2, uint16_t color);
private:
    void writeBus(uint8_t data);
    uint8_t readBus(void);
    virtual void sendCommand(uint8_t index);
    uint8_t readStatus(void);
    virtual void sendData(uint16_t data);
    void send8BitData(uint8_t data);
    uint8_t readData(void);
    virtual void writeReg(uint8_t reg, uint8_t val);
    uint8_t readReg(uint8_t reg);

    void rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, bool filled);
    void circle(int16_t x, int16_t y, int16_t r, uint16_t color, bool filled);
    void triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, bool filled);
    void ellipse(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint16_t color, bool filled);
    void curve(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color, bool filled);
    void roundrectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t r1, uint16_t r2, uint16_t color, bool filled);
    
    virtual void setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1);
    void int2str(int n, char *str);
    uint16_t _width;
    uint16_t _height;
    uint16_t _bgColor;
    uint16_t _fgColor;
    enum RA8875Size _size;

    uint8_t _cs, _sel;
    uint8_t _miso, _mosi, _clk;
    DigitalOut *_pinSEL;
    SPI *_spi;
};

#endif

