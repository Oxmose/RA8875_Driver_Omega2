/**********************************************************************************************
 Copyright (c) 2014 DisplayModule. All rights reserved.

 Redistribution and use of this source code, part of this source code or any compiled binary
 based on this source code is permitted as long as the above copyright notice and following
 disclaimer is retained.

 DISCLAIMER:
 THIS SOFTWARE IS SUPPLIED "AS IS" WITHOUT ANY WARRANTIES AND SUPPORT. DISPLAYMODULE ASSUMES
 NO RESPONSIBILITY OR LIABILITY FOR THE USE OF THE SOFTWARE.
 
 The GFX_TEST function is based on UTFT_Demo (http://www.henningkarlsen.com/electronics/)
 ********************************************************************************************/

/******************************************************************************
 * Includes
 *****************************************************************************/

#include "DmTftRA8875.h"
#include "dm_platform.h"
#include <cmath>
#include <iostream>

#define wait_ms(ms) usleep(ms * 1000)
#define wait(s) wait_ms(1000 * s)
/******************************************************************************
 * Typedefs and defines
 *****************************************************************************/

/* Note that there are restrictions on which platforms that can use printf
   in combinations with the DmTftLibrary. Some platforms (e.g. LPC1549 LPCXpresso)
   use the same pins for USBRX/USBTX and display control. Printing will
   cause the display to not work. Read more about this on the display's notebook
   page. */

/* Displays without adapter */
#define DM_PIN_SPI_MOSI   SPI_MOSI_PIN
#define DM_PIN_SPI_MISO   SPI_MISO_PIN
#define DM_PIN_SPI_SCLK   SPI_CLK_PIN
#define DM_PIN_CS_TFT     SPI_CS_PIN

/******************************************************************************
 * Local variables
 *****************************************************************************/
DmTftRa8875  tft;  /* DM_TFT50_111 */


/******************************************************************************
 * Global variables
 *****************************************************************************/
unsigned int m_z=12434,m_w=33254;
/******************************************************************************
 * Local functions
 *****************************************************************************/
unsigned int rnd()
{
    m_z = 36969 * (m_z & 65535) + (m_z >>16);
    m_w = 18000 * (m_w & 65535) + (m_w >>16);
    return ((m_z <<16) + m_w);
}

void GFX_TEST()
{
    uint16_t x = 0;
    uint16_t y = 0;
    uint16_t w = tft.width();
    uint16_t h = tft.height();
    int buf[798];

    int x0, x1;
    int y0, y1;
    int r;
    uint16_t color;

    tft.clearScreen(BLACK);
    tft.fillRectangle(0, 0, 799, 19, RED);
    tft.fillRectangle(0, 460, 799, 479, GRAY2);
    tft.setFontColor(RED, WHITE);
    tft.drawStringCentered(0, 0, 800, 20, "*** Ra8875 demo ***");
    tft.setFontColor(GRAY2, YELLOW);
    tft.drawStringCentered(0, 460, 800, 20, "www.displaymodule.com");

    tft.drawRectangle(0, 20, 799, 459, BLUE);
    tft.drawLine(399, 21, 399, 458, BLUE);
    tft.drawLine(1, 239, 798, 239, BLUE);
    for (int i=9; i<790; i+=10)
        tft.drawVerticalLine(i, 234, 10, BLUE);
    for (int i=29; i<450; i+=10)
        tft.drawHorizontalLine(395, i, 10, BLUE);

    tft.setFontColor(BLACK, RED);
    tft.drawString(5, 25, "Sin");

    for (int i=1; i<798; i++) {
        tft.drawPoint(i,239+(sin(((i*1.13)*3.14)/90)*95));
        wait_ms(1);
    }
    wait(1);
    tft.setFontColor(BLACK, YELLOW);
    tft.drawString(5, 45, "Cos");

    for (int i=1; i<798; i++) {
        tft.drawPoint(i,239+(cos(((i*1.13)*3.14)/90)*95));
        wait_ms(1);
    }
    wait(1.0);
    tft.setFontColor(BLACK, GREEN);
    tft.drawString(5, 65, "Tan");

    for (int i=1; i<798; i++) {
        tft.drawPoint(i,239+(tan(((i*1.13)*3.14)/90)));
        wait_ms(1);
    }

    tft.fillRectangle(1, 21, 798, 458, BLACK);
    tft.drawLine(399, 22, 399, 458, BLUE);
    tft.drawLine(1, 239, 798, 239, BLUE);

    // Draw a moving sinewave
    x=1;
    for (int i=1; i<(798*20); i++) {
        x++;
        if (x==799)
            x=1;
        if (i>799) {
            if ((x==399)||(buf[x-1]==239))
                tft.setFontColor(BLACK, BLUE);
            else
                tft.setFontColor(BLACK, BLACK);
            tft.drawPoint(x,buf[x-1]);
        }
        tft.setFontColor(BLACK, 0x07FF);
        y=239+(sin(((i*1.13)*3.14)/90)*(90-(i / 100)));
        tft.drawPoint(x,y);
        wait_ms(1);
        buf[x-1]=y;
    }


    wait(1.0);
    tft.fillRectangle(1, 21, 798, 458, BLACK);
    // Draw some filled rectangles
    for (int i=1; i<6; i++) {
        wait_ms(20);
        switch (i) {
            case 1:
                tft.fillRectangle(70+(i*50), 30+(i*50), 220+(i*50), 180+(i*50), 0xF81F);
                break;
            case 2:
                tft.fillRectangle(70+(i*50), 30+(i*50), 220+(i*50), 180+(i*50), RED);
                break;
            case 3:
                tft.fillRectangle(70+(i*50), 30+(i*50), 220+(i*50), 180+(i*50),GREEN);
                break;
            case 4:
                tft.fillRectangle(70+(i*50), 30+(i*50), 220+(i*50), 180+(i*50), BLUE);
                break;
            case 5:
                tft.fillRectangle(70+(i*50), 30+(i*50), 220+(i*50), 180+(i*50),YELLOW);
                break;
        }

    }

    wait(1.0);
    tft.fillRectangle(1, 21, 798, 458, BLACK);
    // Draw some filled, rounded rectangles
    for (int i=1; i<6; i++) {
        wait_ms(20);
        switch (i) {
            case 1:
                tft.fillRoundRectangle(570-(i*50), 30+(i*50),720-(i*50), 180+(i*50), 10, 10, 0xF81F);
                break;
            case 2:
                tft.fillRoundRectangle(570-(i*50), 30+(i*50), 720-(i*50), 180+(i*50), 10, 10, RED);
                break;
            case 3:
                tft.fillRoundRectangle(570-(i*50), 30+(i*50), 720-(i*50), 180+(i*50), 10, 10, GREEN);
                break;
            case 4:
                tft.fillRoundRectangle(570-(i*50), 30+(i*50), 720-(i*50), 180+(i*50), 10, 10, BLUE);
                break;
            case 5:
                tft.fillRoundRectangle(570-(i*50), 30+(i*50), 720-(i*50), 180+(i*50), 10, 10, YELLOW);
                break;
        }
    }

    wait(1.0);
    tft.fillRectangle(1, 21, 798, 458, BLACK);
    // Draw some filled, circles
    for (int i=1; i<6; i++) {
        wait_ms(20);
        switch (i) {
            case 1:
                tft.fillCircle(70+(i*50), 70+(i*50), 75, 0xF81F);
                break;
            case 2:
                tft.fillCircle(70+(i*50), 70+(i*50), 75, RED);
                break;
            case 3:
                tft.fillCircle(70+(i*50), 70+(i*50), 75, GREEN);
                break;
            case 4:
                tft.fillCircle(70+(i*50), 70+(i*50), 75, BLUE);
                break;
            case 5:
                tft.fillCircle(70+(i*50), 70+(i*50), 75, YELLOW);
                break;
        }
    }

    wait(1.0);
    tft.fillRectangle(1, 21, 798, 458, BLACK);
    // Draw some filled, ellipses
    for (int i=1; i<6; i++) {
        wait_ms(20);
        switch (i) {
            case 1:
                tft.fillEllipse(700-(i*50), 70+(i*50), 75, 40, 0xF81F);
                break;
            case 2:
                tft.fillEllipse(700-(i*50), 70+(i*50), 75, 40, RED);
                break;
            case 3:
                tft.fillEllipse(700-(i*50), 70+(i*50), 75, 40, GREEN);
                break;
            case 4:
                tft.fillEllipse(700-(i*50), 70+(i*50), 75, 40, BLUE);
                break;
            case 5:
                tft.fillEllipse(700-(i*50), 70+(i*50), 75, 40, YELLOW);
                break;
        }
    }

    wait(1.0);
    tft.fillRectangle(1, 21, 798, 458, BLACK);
    // Draw some filled, triangles
    for (int i=1; i<6; i++) {
        wait_ms(20);
        switch (i) {
            case 1:
                tft.fillTriangle(399, 50+(i*50), 249, 150+(i*50), 549, 150+(i*50), 0xF81F);
                break;
            case 2:
                tft.fillTriangle(399, 50+(i*50), 249, 150+(i*50), 549, 150+(i*50), RED);
                break;
            case 3:
                tft.fillTriangle(399, 50+(i*50), 249, 150+(i*50), 549, 150+(i*50), GREEN);
                break;
            case 4:
                tft.fillTriangle(399, 50+(i*50), 249, 150+(i*50), 549, 150+(i*50), BLUE);
                break;
            case 5:
                tft.fillTriangle(399, 50+(i*50), 249, 150+(i*50), 549, 150+(i*50), YELLOW);
                break;
        }
    }

    wait(1.0);
    tft.fillRectangle(1, 21, 798, 458, BLACK);
    // Draw some lines in a pattern

    for (int i=21; i<458; i+=5) {
        tft.drawLine(1, i, (i*1.82)-31, 458, RED);
        wait_ms(20);
    }
    for (int i=458; i>21; i-=5) {
        tft.drawLine(798, i, (i*1.82)-35, 21, RED);
        wait_ms(20);
    }

    for (int i=458; i>21; i-=5) {
        tft.drawLine(1, i, 835-(i*1.82), 21,CYAN);
        wait_ms(20);
    }
    for (int i=21; i<458; i+=5) {
        tft.drawLine(798, i, 831-(i*1.82), 458,CYAN);
        wait_ms(20);
    }

    wait(1.0);
    tft.fillRectangle(1, 21, 798, 458, BLACK);


    // Draw some random circles
    for (int i=0; i<100; i++) {
        color = ((rnd()%255 >> 3) << 11) | ((rnd()%255 >> 2) << 5) | ((rnd()%255 >> 3));
        x0=62+rnd()%650;
        y0=85+rnd()%300;
        r=1+ rnd()%60;
        tft.fillCircle(x0, y0, r, color);
        wait_ms(40);
    }

    wait(1.0);
    tft.fillRectangle(1, 21, 798, 458, BLACK);
    // Draw some random rectangles
    for (int i=0; i<100; i++) {
        color = ((rnd()%255 >> 3) << 11) | ((rnd()%255 >> 2) << 5) | ((rnd()%255 >> 3));
        x0=2+rnd()%796;
        y0=25+rnd()%420;
        x1=2+rnd()%796;
        y1=25+rnd()%420;
        tft.drawRectangle(x0, y0, x1, y1, color);
        wait_ms(40);
    }

    wait(1.0);
    tft.fillRectangle(1, 21, 798, 458, BLACK);
    // Draw some random lines
    for (int i=0; i<100; i++) {
        color = ((rnd()%255 >> 3) << 11) | ((rnd()%255 >> 2) << 5) | ((rnd()%255 >> 3));
        x0=2+rnd()%796;
        y0=25+rnd()%420;
        x1=2+rnd()%796;
        y1=25+rnd()%420;
        tft.drawLine(x0, y0, x1, y1, color);
        wait_ms(40);
    }


    wait(1.0);
    tft.fillRectangle(1, 21, 798, 458, BLACK);
    // Draw some random pixels
    for (int i=0; i<5000; i++) {
        color = ((rnd()%255 >> 3) << 11) | ((rnd()%255 >> 2) << 5) | ((rnd()%255 >> 3));
        tft.setFontColor(BLACK, color);
        x0=2+rnd()%796;
        y0=25+rnd()%420;
        tft.drawPoint(x0, y0);
    }

    wait(1.0);
    tft.fillRectangle(0, 0, 799, 479, BLUE);
    tft.fillRoundRectangle(200, 140, 600, 340, 10, 10, RED);
    wait(0.2);
    tft.setFontColor(RED, WHITE);
    tft.drawStringCentered(200, 140, 400, 200, "Restarting in a few seconds...");
}

/******************************************************************************
 * Main
 *****************************************************************************/


int main()
{
std::cout << "HELLO FROM TEST" << std::endl;
 
    tft.init();
    int x = 0;
    int y = 0;
    int imgWidth = 10;
    int imgHeight = 0;
    #if 0
    /* Init */
    pc.printf("START_REG");
    
    tft.drawStringCentered(200, 140, 400, 200, "WAIT WIDTH");
    pc.scanf("%d", &imgWidth);
    pc.printf("WD_OK");
    
    tft.drawStringCentered(200, 140, 400, 200, "WAIT HEIGHT");
    pc.scanf("%d", &imgHeight);
    pc.printf("HT_OK");
    
    
    tft.drawStringCentered(200, 140, 400, 200, "Drawing...");
    for (int i=0; i <= imgWidth * imgHeight; i++) 
    {
         uint16_t color = 0;
         pc.scanf("%d", &color);
         tft.setFontColor(BLACK, color);
         tft.drawPoint(x, y);
         if(++x >= imgWidth)
         {
            ++y;
            x = 0;
         }
    }
    #endif
    while(1) {
        GFX_TEST();
        wait(3);
    }
}

