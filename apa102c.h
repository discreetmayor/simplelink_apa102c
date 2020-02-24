#include <stdint.h>
#include <string.h>
#include <ColorUtil.h>
#include <ti/drivers/SPI.h>

#ifndef APA102C_H_
#define APA102C_H_

#define BRIGHTNESS_MASK 0xe0
#define INITIAL_BRIGHTNESS 0.16

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

    APA102C(SPI_Handle spi, uint16_t numLeds);

    void setNumLeds(uint16_t numLeds);
    void setBrightness(float brightness);

    void setLed(rgbColorFloat *color, uint16_t ledIndex);
    void setLed(FloatLed *led, uint16_t ledIndex);

    void setAllLeds(FloatLed *led);
    void setAllLeds(rgbColorFloat *color);

    FloatLed getLed(uint16_t ledIndex);
    EightBitLed getEightBitLed(uint16_t ledIndex);

    rgbColorFloat getLedColor(void);

    void capture();
    FloatLed* getCaptureBuffer();
    void output();

private:
    SPI_Handle      spi;
    SPI_Transaction transaction;
    uint16_t        numLeds;
    uint16_t        frameLength;
    FloatLed       *backBuffer;
    FloatLed       *captureBuffer;
    EightBitLed    *outputBuffer;
    EightBitLed     frameStart[4] = {0x00, 0x00, 0x00, 0x00};
    EightBitLed     frameEnd[4]   = {0xff, 0xff, 0xff, 0xff};
    float           brightness = INITIAL_BRIGHTNESS;


    void backBufferToOutputBuffer();
};

#endif
