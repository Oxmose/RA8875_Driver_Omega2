/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 ********************************************************************************************/

/*  Notice:
    The panel resolution should be config in DmTftRa8875::init() function.
    RA8875Size size = RA8875_480x272 or RA8875Size size = RA8875_800x480;
*/

#include "DmTftRA8875.h"

DmTftRa8875::DmTftRa8875(uint8_t cs, uint8_t sel, uint8_t miso, uint8_t mosi, uint8_t clk)
    : DmTftBase(480, 272)
{
    _cs = cs;
    _sel = sel;
    _miso = miso;
    _mosi = mosi;
    _clk = clk;
}

DmTftRa8875::~DmTftRa8875()
{
    delete _pinCS;
    delete _pinSEL;
    delete _spi;

    _pinCS = NULL;
    _pinSEL = NULL;
    _spi = NULL;
}

uint16_t DmTftRa8875::width(void)
{
    return _width;
}

uint16_t DmTftRa8875::height(void)
{
    return _height;
}

void DmTftRa8875::writeBus(uint8_t data)
{
    _spi->write(data);
}

uint8_t DmTftRa8875::readBus(void)
{
    return _spi->write(0x00); // dummy byte to read
}

void DmTftRa8875::sendCommand(uint8_t index)
{
    cbi(_pinCS, _bitmaskCS);

    writeBus(0x80);
    writeBus(index);

    sbi(_pinCS, _bitmaskCS);
}

uint8_t DmTftRa8875::readStatus(void)
{
    cbi(_pinCS, _bitmaskCS);

    writeBus(0xC0);
    uint8_t data = readBus();

    sbi(_pinCS, _bitmaskCS);

    return data;
}

void DmTftRa8875::sendData(uint16_t data)
{
    uint8_t dh = data>>8;
    uint8_t dl = data&0xff;

    cbi(_pinCS, _bitmaskCS);

    writeBus(0x00);
    writeBus(dh);
    writeBus(dl);

    sbi(_pinCS, _bitmaskCS);

}

void DmTftRa8875::send8BitData(uint8_t data)
{
    cbi(_pinCS, _bitmaskCS);

    writeBus(0x00);
    writeBus(data);

    sbi(_pinCS, _bitmaskCS);
}

uint8_t DmTftRa8875::readData(void)
{
    cbi(_pinCS, _bitmaskCS);

    writeBus(0x40);
    uint8_t data = readBus();

    sbi(_pinCS, _bitmaskCS);

    return data;
}

void DmTftRa8875::writeReg(uint8_t reg, uint8_t val)
{
    sendCommand(reg);
    send8BitData(val);
}

uint8_t DmTftRa8875::readReg(uint8_t reg)
{
    sendCommand(reg);
    return readData();
}

void DmTftRa8875::setAddress(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    /* Set active window X */
    writeReg(0x30, x0);                                        // horizontal start point
    writeReg(0x31, x0 >> 8);
    writeReg(0x34, (uint16_t)(x1) & 0xFF);            // horizontal end point
    writeReg(0x35, (uint16_t)(x1) >> 8);

    /* Set active window Y */
    writeReg(0x32, y0);                                        // vertical start point
    writeReg(0x33, y0 >> 8);
    writeReg(0x36, (uint16_t)(y1) & 0xFF);           // vertical end point
    writeReg(0x37, (uint16_t)(y1) >> 8);

    writeReg(0x46, x0);
    writeReg(0x47, x0 >> 8);
    writeReg(0x48, y0);
    writeReg(0x49, y0 >> 8);

    sendCommand(0x02);
}

void DmTftRa8875::softReset(void)
{
    //softreset
    sendCommand(0x01);
    send8BitData(0x01);
    delay(10);
    sendCommand(0x01);
    send8BitData(0x00);
    delay(100); // This much delay needed??
}

void DmTftRa8875::init(void)
{
    // DM_TFT43_108 = RA8875_480x272;  DM_TFT50_111 = RA8875_800x480
    RA8875Size size = RA8875_800x480;
    
    setTextColor(BLACK, WHITE);

    _pinCS = new DigitalOut((PinName)_cs);
    _pinSEL = new DigitalOut((PinName)_sel);
    sbi(_pinSEL, _bitmaskSEL);  // w25 control by MCU

    sbi(_pinCS, _bitmaskCS);

    _spi = new SPI((PinName)_mosi, (PinName)_miso, (PinName)_clk);
    _spi->format(8,3);
    _spi->frequency(2000000); // Max SPI speed for display is 10 and for 17 for LPC15xx
    softReset();

    cbi(_pinCS, _bitmaskCS);

    /* Timing values */
    uint8_t pixclk;
    uint8_t hsync_start;
    uint8_t hsync_pw;
    uint8_t hsync_finetune;
    uint8_t hsync_nondisp;
    uint8_t vsync_pw;
    uint16_t vsync_nondisp;
    uint16_t vsync_start;

    if(size == RA8875_480x272) {
        _width = 480;
        _height = 272;
        setWidth(_width);
        setHeight(_height);
        // PLL init
        // 20MHz*(10+1)/((RA8875_PLLC1_PLLDIV1 +1) * 2^RA8875_PLLC2_DIV4))
        writeReg(0x88, 0x00 + 10);
        delay(1);
        writeReg(0x89, 0x02);
        delay(1);

        pixclk = 0x80 | 0x02;
        hsync_nondisp = 10;
        hsync_start = 8;
        hsync_pw = 48;
        hsync_finetune = 0;
        vsync_nondisp = 3;
        vsync_start = 8;
        vsync_pw = 10;
    }

    if(size == RA8875_800x480) {
        _width = 800;
        _height = 480;
        setWidth(_width);
        setHeight(_height);
        // PLL init
        // 20MHz*(10+1)/((RA8875_PLLC1_PLLDIV1 +1) * 2^RA8875_PLLC2_DIV4))
        writeReg(0x88, 0x00 + 10);
        delay(1);
        writeReg(0x89, 0x02);
        delay(1);

        pixclk = 0x80 | 0x01;
        hsync_nondisp = 26;
        hsync_start = 32;
        hsync_pw = 96;
        hsync_finetune = 0;
        vsync_nondisp = 32;
        vsync_start = 23;
        vsync_pw = 2;
    }

    writeReg(0x10, 0x0C | 0x00);

    writeReg(0x04, pixclk);
    delay(1);

    /* Horizontal settings registers */
    writeReg(0x14, (_width / 8) - 1);                          // H width: (HDWR + 1) * 8 = 480
    writeReg(0x15, 0x00 + hsync_finetune);
    writeReg(0x16, (hsync_nondisp - hsync_finetune - 2)/8);    // H non-display: HNDR * 8 + HNDFTR + 2 = 10
    writeReg(0x17, hsync_start/8 - 1);                         // Hsync start: (HSTR + 1)*8
    writeReg(0x18, 0x00 + (hsync_pw/8 - 1));        // HSync pulse width = (HPWR+1) * 8

    /* Vertical settings registers */
    writeReg(0x19, (uint16_t)(_height - 1) & 0xFF);
    writeReg(0x1A, (uint16_t)(_height - 1) >> 8);
    writeReg(0x1B, vsync_nondisp-1);                          // V non-display period = VNDR + 1
    writeReg(0x1C, vsync_nondisp >> 8);
    writeReg(0x1D, vsync_start-1);                            // Vsync start position = VSTR + 1
    writeReg(0x1E, vsync_start >> 8);
    writeReg(0x1F, 0x00 + vsync_pw - 1);            // Vsync pulse width = VPWR + 1

    /* Set active window X */
    writeReg(0x30, 0);                                        // horizontal start point
    writeReg(0x31, 0);
    writeReg(0x34, (uint16_t)(_width - 1) & 0xFF);            // horizontal end point
    writeReg(0x35, (uint16_t)(_width - 1) >> 8);

    /* Set active window Y */
    writeReg(0x32, 0);                                        // vertical start point
    writeReg(0x33, 0);
    writeReg(0x36, (uint16_t)(_height - 1) & 0xFF);           // vertical end point
    writeReg(0x37, (uint16_t)(_height - 1) >> 8);
    /* Clear the entire window */
    writeReg(0x8E, 0x80 | 0x00);
    /* Wait for the command to finish */
    while (readReg(0x8E) & 0x80);
    //delay(100);
    // display on
    writeReg(0x01, 0x00 | 0x80);
    // GPIOX on
    writeReg(0xC7, 1);
    sbi(_pinCS, _bitmaskCS);

    //clearScreen();    
    setFontColor(BLACK, WHITE);
    //setFontZoom(0, 0);

    // spi flash control by MCU
    w25CtrlByMCU();

    //backlight on
    backlightOn(true);
    backlightAdjust(255); // max luminance

}

void DmTftRa8875::eableKeyScan(bool on)
{
    if(on) {
        writeReg(0xC0, (1 << 7) | (0 << 4 ) | 1 );       // enable key scan
    } else {
        writeReg(0xC0, (0 << 7));
    }
}

uint8_t DmTftRa8875::getKeyValue(void)
{
    uint8_t data = 0xFF;
    sendCommand(0xC2);
    data = readData();

    // Clear key interrupt status
    writeReg(0xF1,readReg(0xF1) | 0x10);
    delay(1);
    return data;

}

bool DmTftRa8875::isKeyPress(void)
{
    uint8_t temp;
    temp = readReg(0xF1);

    if(temp & 0x10) {
        return true;
    } else {
        return false;
    }
}

void DmTftRa8875::backlightOn(bool on)
{
    if(on) {
        writeReg(0x8A, (1 << 7) | (10 << 0 ));       // enable PWM1
    } else {
        writeReg(0x8A, (0 << 7) | (10 << 0 ));
    }
}

void DmTftRa8875::backlightAdjust(uint8_t value)
{
    writeReg(0x8B, value);
}

void DmTftRa8875::w25CtrlByMCU(void)
{
    cbi(_pinSEL, _bitmaskSEL);
}

void DmTftRa8875::w25CtrlByRa8875(void)
{
    sbi(_pinSEL, _bitmaskSEL);
}

void DmTftRa8875::drawImageContinuous(uint32_t startaddress,uint32_t count,uint16_t x0,uint16_t y0,uint16_t x1,uint16_t y1)
{
    w25CtrlByRa8875();

    setAddress(x0, y0, x1-1, y1-1);

    writeReg(0xE0, 0x00);

    writeReg(0x05, (0 << 7) | (0 << 6) | (1 << 5) | (0 << 3) | (1 << 2) | (3 << 1));
    writeReg(0x06, (0 << 0)); // set serial flash frequency

    writeReg(0xB0,startaddress & 0xFF);  //  DMA Source Start Address
    writeReg(0xB1,startaddress >> 8);
    writeReg(0xB2,startaddress >> 16);

    writeReg(0xB4, count & 0xFF);  // DMA Transfer Number
    writeReg(0xB6, (count & 0xFF00) >> 8 );
    writeReg(0xB8, (count & 0xFF0000) >> 16);


    writeReg(0xBF, 0x00);  // Continuous mode
    writeReg(0xBF, readReg(0xBF) | 0x01); //start DMA

    /* Wait for the DMA Transfer to finish */
    while (readReg(0xBF) & 0x01);

    w25CtrlByMCU();

}

void DmTftRa8875::drawImageBlock(uint32_t startaddress, uint32_t count, uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t pic_width, uint16_t block_width, uint16_t block_height)
{
    w25CtrlByRa8875();

    setAddress(x0, y0, x1-1, y1-1);

    writeReg(0xE0, 0x00);

    writeReg(0x05, (0 << 7) | (0 << 6) | (1 << 5) | (0 << 3) | (1 << 2) | (3 << 1));
    writeReg(0x06, (0 << 0)); // set serial flash frequency

    writeReg(0xB0,startaddress & 0xFF);  //  DMA Source Start Address
    writeReg(0xB1,startaddress >> 8);
    writeReg(0xB2,startaddress >> 16);

    writeReg(0xB4, block_width & 0xFF);  // DMA block width
    writeReg(0xB5, block_width >> 8 );
    writeReg(0xB6, block_height & 0xFF);  // DMA block height
    writeReg(0xB7, block_height >> 8 );

    writeReg(0xB8, pic_width & 0xFF);  // DMA soruce picture width
    writeReg(0xB9, pic_width >> 8 );

    writeReg(0xBF, 0x02);  // block mode
    writeReg(0xBF, readReg(0xBF) | 0x01); //start DMA

    /* Wait for the DMA Transfer to finish */
    while (readReg(0xBF) & 0x01);

    w25CtrlByMCU();

}

void DmTftRa8875::drawPoint(uint16_t x, uint16_t y, uint16_t radius)
{
    if (radius == 0) {
        cbi(_pinCS, _bitmaskCS);

        setAddress(x,y,x,y);
        sendData(_fgColor);

        sbi(_pinCS, _bitmaskCS);
    } else {
        fillRectangle(x-radius,y-radius,x+radius,y+radius, _fgColor);
    }
}


void DmTftRa8875::setFontColor(uint16_t background,uint16_t foreground)
{
    /* Set Background Color */
    _bgColor = background;
    writeReg(0x60, (background & 0xf800) >> 11);
    writeReg(0x61, (background & 0x07e0) >> 5);
    writeReg(0x62, (background & 0x001f));

    /* Set Fore Color */
    _fgColor = foreground;
    writeReg(0x63, (foreground & 0xf800) >> 11);
    writeReg(0x64, (foreground & 0x07e0) >> 5);
    writeReg(0x65, (foreground & 0x001f));
}


void DmTftRa8875::setFontZoom(uint8_t Hsize,uint8_t Vsize)
{
    writeReg(0x22, ((Hsize & 0x03) <<2 | (Vsize & 0x03)));
}

void DmTftRa8875::int2str(int n, char *str)
{
    char buf[10] = "";
    int i = 0;
    int len = 0;
    int temp = n < 0 ? -n: n;

    if (str == NULL) {
        return;
    }
    while(temp) {
        buf[i++] = (temp % 10) + '0';
        temp = temp / 10;
    }

    len = n < 0 ? ++i: i;
    str[i] = 0;
    while(1) {
        i--;
        if (buf[len-i-1] ==0) {
            break;
        }
        str[i] = buf[len-i-1];
    }
    if (i == 0 ) {
        str[i] = '-';
    }
}

void DmTftRa8875::drawNumber(uint16_t x, uint16_t y, int num, int digitsToShow, bool leadingZeros)
{
    char p[10];
    int2str(num, p);
    
    // clear the last number on the screen; default font width is 8.
    for(int i=0; i<digitsToShow; i++)
        drawString(x+i*8, y," ");
        
    drawString(x, y, p);
}


void DmTftRa8875::drawString(uint16_t x, uint16_t y, const char *p)
{
    setAddress(0, 0, _width-1, _height-1);

    writeReg(0x40, (1 << 7) | (1 << 0)); // enter text mode
    writeReg(0x2A, x);
    writeReg(0x2B, x >> 8);
    writeReg(0x2C, y);
    writeReg(0x2D, y >> 8);

    writeReg(0x2F, 0x00);
    writeReg(0x21, (0 << 7) | (0 << 5) | (0 << 1) | (0 << 0));
    sendCommand(0x02);

    while (*p != '\0') {
        send8BitData(*p);
        while ((readStatus() & 0x80) == 0x80);  // wait finish
        p++;
    }
    writeReg(0x40, (0 << 7));   // enter graphics mode
}

void DmTftRa8875::drawStringCentered(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const char *p)
{

    int len = strlen(p);
    uint16_t tmp = len * 8;
    if (tmp <= width) {
        x += (width - tmp)/2;
    }
    if (16 <= height) {
        y += (height - 16)/2;
    }

    drawString(x, y, p);
}

void DmTftRa8875::clearScreen(uint16_t color)
{
    rectangle(0, 0, _width-1, _height-1, color, true);
}

void DmTftRa8875::rectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color, bool filled)
{
    setAddress(0, 0, _width-1, _height-1);

    /* Set x0 */
    writeReg(0x91, x0);
    writeReg(0x92, x0 >> 8);

    /* Set y0 */
    writeReg(0x93, y0);
    writeReg(0x94, y0 >> 8);

    /* Set x1 */
    writeReg(0x95, x1);
    writeReg(0x96, x1 >> 8);

    /* Set y1 */
    writeReg(0x97, y1);
    writeReg(0x98, y1 >> 8);

    /* Set Color */
    writeReg(0x63, (color & 0xf800) >> 11);
    writeReg(0x64, (color & 0x07e0) >> 5);
    writeReg(0x65, (color & 0x001f));

    /* Draw! */
    if (filled) {
        writeReg(0x90, 0xB0);
    } else {
        writeReg(0x90, 0x90);
    }

    /* Wait for the command to finish */
    while (readReg(0x90) & 0x80);
}

void DmTftRa8875::drawRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    rectangle(x0, y0, x1, y1, color, false);
}
void DmTftRa8875::fillRectangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    rectangle(x0, y0, x1, y1, color, true);
}

void DmTftRa8875::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
    setAddress(0, 0, _width-1, _height-1);

    /* Set x0 */
    writeReg(0x91, x0);
    writeReg(0x92, x0 >> 8);

    /* Set y0 */
    writeReg(0x93, y0);
    writeReg(0x94, y0 >> 8);

    /* Set x1 */
    writeReg(0x95, x1);
    writeReg(0x96, x1 >> 8);

    /* Set y1 */
    writeReg(0x97, y1);
    writeReg(0x98, y1 >> 8);

    /* Set Color */
    writeReg(0x63, (color & 0xf800) >> 11);
    writeReg(0x64, (color & 0x07e0) >> 5);
    writeReg(0x65, (color & 0x001f));

    /* Draw! */
    writeReg(0x90, 0x80);

    /* Wait for the command to finish */
    while (readReg(0x90) & 0x80);
}

void DmTftRa8875::drawVerticalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
    drawLine(x, y, x, y+length, color);
}

void DmTftRa8875::drawHorizontalLine(uint16_t x, uint16_t y, uint16_t length, uint16_t color)
{
    drawLine(x, y, x+length, y, color);
}

void DmTftRa8875::circle(int16_t x, int16_t y, int16_t r, uint16_t color, bool filled)
{
    setAddress(0, 0, _width-1, _height-1);

    /* Set x */
    writeReg(0x99, x);
    writeReg(0x9A, x >> 8);

    /* Set y */
    writeReg(0x9B, y);
    writeReg(0x9C, y >> 8);

    /* Set Radius */
    writeReg(0x9D, r);

    /* Set Color */
    writeReg(0x63, (color & 0xf800) >> 11);
    writeReg(0x64, (color & 0x07e0) >> 5);
    writeReg(0x65, (color & 0x001f));

    /* Draw! */
    if (filled) {
        writeReg(0x90, 0x40 | 0x20);
    } else {
        writeReg(0x90, 0x40 | 0x00);
    }

    /* Wait for the command to finish */
    while (readReg(0x90) & 0x40);
}

void DmTftRa8875::drawCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    circle(x0, y0, r, color, false);
}
void DmTftRa8875::fillCircle(uint16_t x0, uint16_t y0, uint16_t r, uint16_t color)
{
    circle(x0, y0, r, color, true);
}

void DmTftRa8875::triangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2, uint16_t color, bool filled)
{
    setAddress(0, 0, _width-1, _height-1);

    /* Set Point 0 */
    writeReg(0x91, x0);
    writeReg(0x92, (x0 >> 8));
    writeReg(0x93, y0);
    writeReg(0x94, (y0 >> 8));

    /* Set Point 1 */
    writeReg(0x95, x1);
    writeReg(0x96, (x1 >> 8));
    writeReg(0x97, y1);
    writeReg(0x98, (y1 >> 8));

    /* Set Point 2 */
    writeReg(0xA9, x2);
    writeReg(0xAA, (x2 >> 8));
    writeReg(0xAB, y2);
    writeReg(0xAC, (y2 >> 8));

    /* Set Color */
    writeReg(0x63, (color & 0xf800) >> 11);
    writeReg(0x64, (color & 0x07e0) >> 5);
    writeReg(0x65, (color & 0x001f));

    /* Draw! */
    if (filled) {
        writeReg(0x90, 0xA1);
    } else {
        writeReg(0x90, 0x81);
    }

    /* Wait for the command to finish */
    while (readReg(0x90) & 0x80);
}

void DmTftRa8875::drawTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    triangle(x0, y0, x1, y1, x2, y2, color, false);
}

void DmTftRa8875::fillTriangle(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    triangle(x0, y0, x1, y1, x2, y2, color, true);
}

void DmTftRa8875::ellipse(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint16_t color, bool filled)
{
    setAddress(0, 0, _width-1, _height-1);

  /* Set Center Point */
    writeReg(0xA5, x0);
    writeReg(0xA6, (x0 >> 8));
    writeReg(0xA7, y0);
    writeReg(0xA8, (y0 >> 8));
    
  /* Set Long and Short Axis */
    writeReg(0xA1, longAxis);
    writeReg(0xA2, (longAxis >> 8));
    writeReg(0xA3, shortAxis);
    writeReg(0xA4, (shortAxis >> 8));

    /* Set Color */
    writeReg(0x63, (color & 0xf800) >> 11);
    writeReg(0x64, (color & 0x07e0) >> 5);
    writeReg(0x65, (color & 0x001f));

    /* Draw! */
    if (filled) {
        writeReg(0xA0, 0xC0);
    } else {
        writeReg(0xA0, 0x80);
    }

    /* Wait for the command to finish */
    while (readReg(0xA0) & 0x80);
 
}

void DmTftRa8875::drawEllipse(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint16_t color)
{
    ellipse(x0, y0, longAxis, shortAxis, color, false);
}
 
void DmTftRa8875::fillEllipse(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint16_t color)
{
    ellipse(x0, y0, longAxis, shortAxis, color, true);
}

void DmTftRa8875::curve(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color, bool filled)
{
    setAddress(0, 0, _width-1, _height-1);

  /* Set Center Point */
    writeReg(0xA5, x0);
    writeReg(0xA6, (x0 >> 8));
    writeReg(0xA7, y0);
    writeReg(0xA8, (y0 >> 8));
    
  /* Set Long and Short Axis */
    writeReg(0xA1, longAxis);
    writeReg(0xA2, (longAxis >> 8));
    writeReg(0xA3, shortAxis);
    writeReg(0xA4, (shortAxis >> 8));

    /* Set Color */
    writeReg(0x63, (color & 0xf800) >> 11);
    writeReg(0x64, (color & 0x07e0) >> 5);
    writeReg(0x65, (color & 0x001f));

    /* Draw! */
    if (filled) {
        writeReg(0xA0, 0xD0 | (curvePart & 0x03));
    } else {
        writeReg(0xA0, 0x90 | (curvePart & 0x03));
    }

    /* Wait for the command to finish */
    while (readReg(0xA0) & 0x80);
    
}

void DmTftRa8875::drawCurve(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color)
{
  curve(x0, y0, longAxis, shortAxis, curvePart, color, false);
}  

void DmTftRa8875::fillCurve(int16_t x0, int16_t y0, int16_t longAxis, int16_t shortAxis, uint8_t curvePart, uint16_t color)
{
  curve(x0, y0, longAxis, shortAxis, curvePart, color, true);
}      

void DmTftRa8875::roundrectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t r1, uint16_t r2, uint16_t color, bool filled)
{
    setAddress(0, 0, _width-1, _height-1);

    /* Set x0 */
    writeReg(0x91, x0);
    writeReg(0x92, x0 >> 8);

    /* Set y0 */
    writeReg(0x93, y0);
    writeReg(0x94, y0 >> 8);

    /* Set x1 */
    writeReg(0x95, x1);
    writeReg(0x96, x1 >> 8);

    /* Set y1 */
    writeReg(0x97, y1);
    writeReg(0x98, y1 >> 8);
    
    /* Set Radius */
    writeReg(0xA1, r1);
    writeReg(0xA2, r1 >> 8);
    writeReg(0xA3, r2);
    writeReg(0xA4, r2 >> 8);    

    /* Set Color */
    writeReg(0x63, (color & 0xf800) >> 11);
    writeReg(0x64, (color & 0x07e0) >> 5);
    writeReg(0x65, (color & 0x001f));

    /* Draw! */
    if (filled) {
        writeReg(0xA0, 0xE0);
    } else {
        writeReg(0xA0, 0xA0);
    }

    /* Wait for the command to finish */
    while (readReg(0xA0) & 0x80);
}

void DmTftRa8875::drawRoundRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t r1, uint16_t r2, uint16_t color)
{
    roundrectangle(x0, y0, x1, y1, r1, r2, color, false);
}
void DmTftRa8875::fillRoundRectangle(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t r1, uint16_t r2, uint16_t color)
{
    roundrectangle(x0, y0, x1, y1, r1, r2, color, true);
}
/*********************************************************************************************************
  END FILE
*********************************************************************************************************/

