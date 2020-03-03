/*
 * APA102C.h
 *
 *  Created on: Feb 24, 2020
 *      Author: mh
 */

#include <ColorUtil.h>
#include <ti/drivers/SPI.h>

#ifndef APA102C_H_
#define APA102C_H_

#define BRIGHTNESS_MASK    0xe0
#define INITIAL_BRIGHTNESS 1.0f

using namespace colorUtil;

class APA102C {
public:

    typedef struct FloatLed {
        float brightness;
        rgbColorFloat color;
    } FloatLed;

    typedef struct EightBitLed {
        uint8_t brightness;
        rgbColor color;
    } EightBitLed;

    APA102C(uint_least8_t spiIndex, uint16_t ledCount);
    virtual ~APA102C();

    void setBrightness(float brightness);

    void setLed(rgbColorFloat *color, uint16_t ledIndex);
    void setLed(FloatLed *led, uint16_t ledIndex);
    void setAllLeds(FloatLed *led);
    void setAllLeds(rgbColorFloat *color);

    FloatLed *getLed(uint16_t ledIndex);
    EightBitLed getEightBitLed(uint16_t ledIndex);
    rgbColorFloat getLedColor(void);

    void capture();
    void capture(rgbColorFloat *c);
    FloatLed *getCaptureBuffer();
    void output();

private:
    SPI_Handle      spi;
    SPI_Transaction transaction;
    uint16_t        ledCount;
    uint16_t        frameLength;
    FloatLed       *backBuffer;
    FloatLed       *captureBuffer;
    EightBitLed    *outputBuffer;
    EightBitLed     frameStart[4] = {0x00, 0x00, 0x00, 0x00};
    EightBitLed     frameEnd[4]   = {0xff, 0xff, 0xff, 0xff};
    float           brightness    = INITIAL_BRIGHTNESS;

    void backBufferToOutputBuffer();
};

#endif
