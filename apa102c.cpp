#include <apa102c.h>


APA102C::APA102C(SPI_Handle s, uint16_t numLeds) {
    spi = s;
    setNumLeds(numLeds);
    backBuffer = new FloatLed[numLeds];
    captureBuffer = new FloatLed[numLeds];
    outputBuffer = new EightBitLed[frameLength];
    memcpy(outputBuffer, &frameStart, sizeof(EightBitLed));
    memcpy(outputBuffer+frameLength-1, &frameEnd, sizeof(EightBitLed));
}

void APA102C::setBrightness(float b) {
    brightness = b;
}

void APA102C::setNumLeds(uint16_t num) {
    numLeds = num;
    frameLength = numLeds+2;
}

void APA102C::setLed(FloatLed *led, uint16_t ledIndex) {
    memcpy(backBuffer+ledIndex, led, sizeof(FloatLed));
}

void APA102C::setLed(rgbColorFloat *c, uint16_t ledIndex) {
    FloatLed led = {brightness: brightness, color: *c};
    memcpy(backBuffer+ledIndex, &led, sizeof(FloatLed));
}

void APA102C::setAllLeds(rgbColorFloat *c) {
    for(uint16_t i = 0; i < numLeds; i++) {
        setLed(c, i);
    }
}

APA102C::FloatLed APA102C::getLed(uint16_t ledIndex) {
    FloatLed c = *(backBuffer+ledIndex);
    return c;
}

APA102C::EightBitLed APA102C::getEightBitLed(uint16_t ledIndex) {
    FloatLed l = *(backBuffer+ledIndex);
    uint8_t b = (uint8_t)(l.brightness*31) & 0x1f;
    EightBitLed led = {
         brightness: (uint8_t)(0xe0 | b),
         color: quantize(&l.color)
    };
    return led;
}

void APA102C::capture() {
    memcpy(captureBuffer, backBuffer, (numLeds*sizeof(*backBuffer)));
}

APA102C::FloatLed *APA102C::getCaptureBuffer() {
    return captureBuffer;
}

void APA102C::backBufferToOutputBuffer() {
    for(uint16_t i = 0; i < numLeds; i++) {
        //worth writing directly to outputbuffer?
        EightBitLed led = getEightBitLed(i);
        //output BGR
        float red = led.color.red;
        led.color.red = led.color.blue;
        led.color.blue = red;
        memcpy(outputBuffer+i+1, &led, sizeof(outputBuffer));
    }
}



void APA102C::output() {
    backBufferToOutputBuffer();
    SPI_Transaction transaction;
    transaction.txBuf = (void *) outputBuffer;
    transaction.rxBuf = NULL;
    transaction.count = frameLength*4;
    SPI_transfer(spi, &transaction);
}
