#include <apa102c.h>

APA102C::APA102C(SPI_Handle s, uint16_t c) {
    spi      = s;
    ledCount = c;

    backBuffer    = new FloatLed[ledCount];
    captureBuffer = new FloatLed[ledCount];
    outputBuffer  = new EightBitLed[ledCount+2];

    memcpy(outputBuffer, &frameStart, sizeof(EightBitLed));
    memcpy(outputBuffer+ledCount+1, &frameEnd, sizeof(EightBitLed));

    transaction.rxBuf = NULL;
    transaction.count = (ledCount+2)*sizeof(outputBuffer);
    transaction.txBuf = (void *) outputBuffer;
}

APA102C::~APA102C() {
    delete(backBuffer);
    delete(captureBuffer);
    delete(outputBuffer);
}

void APA102C::setBrightness(float b) {
    brightness = b;
    for(uint16_t i = 0; i < ledCount; i++) {
        (backBuffer+i)->brightness = b;
    }
}

void APA102C::setLed(FloatLed *led, uint16_t ledIndex) {
    memcpy(backBuffer+ledIndex, led, sizeof(FloatLed));
}

void APA102C::setLed(rgbColorFloat *c, uint16_t ledIndex) {
    FloatLed led = {brightness: brightness, color: *c};
    setLed(&led, ledIndex);
}

void APA102C::setAllLeds(rgbColorFloat *c) {
    FloatLed led = {brightness: brightness, color: *c};
    for(uint16_t i = 0; i < ledCount; i++) {
        setLed(&led, i);
    }
}

APA102C::FloatLed *APA102C::getLed(uint16_t ledIndex) {
    return backBuffer+ledIndex;
}

APA102C::EightBitLed APA102C::getEightBitLed(uint16_t ledIndex) {
    FloatLed l = *(backBuffer+ledIndex);
    uint8_t b  = (uint8_t)(l.brightness*31) & 0x1f;
    EightBitLed led = {
         brightness: (uint8_t)(0xe0 | b),
         color: quantize(&l.color)
    };
    return led;
}

void APA102C::capture() {
    memcpy(captureBuffer, backBuffer, ledCount*sizeof(*backBuffer));
}

void APA102C::capture(rgbColorFloat *c) {
    FloatLed led = {brightness: brightness, color: *c};
    for(uint16_t i = 0; i < ledCount; i++) {
        memcpy(captureBuffer+i, &led, sizeof(FloatLed));
    }
}

APA102C::FloatLed *APA102C::getCaptureBuffer() {
    return captureBuffer;
}

void APA102C::backBufferToOutputBuffer() {
    for(uint16_t i = 0; i < ledCount; i++) {
        EightBitLed led = getEightBitLed(i);
        //output BGR - find a better way
        float red = led.color.red;
        led.color.red = led.color.blue;
        led.color.blue = red;
        memcpy(outputBuffer+i+1, &led, sizeof(outputBuffer));
    }
}

void APA102C::output() {
    backBufferToOutputBuffer();
    SPI_transfer(spi, &transaction);
}
