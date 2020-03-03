# APA102C 
A simple driver for APA102C LEDs using the TI SimpleLink platform.

__How to use__ 

0. Set up SPI device using SysConfig or board file according to hardware and SDK.

1. Add the colorUtil library and SimpleLink SDK in your include path.

2. Build and add this library to your project

3. Use:

```
#define CONFIG_SPI_0 [INDEX OF SPI DEVICE FROM STEP 0]
#define LED_COUNT 9

#include <apa102c.h>

*pLeds = new APA102C(CONFIG_SPI_0, LED_COUNT);

// make all leds red

rgbColorFloat color = {1.0f, 0.0f, 0.0f};
pLeds->setAllLeds(color);
pLeds->output();

// or set a single LED green, full brightness:

FloatLed led = {1.0f, 0.0f, 1.0f, 0.0f};
pLeds->setLed(led, 0);
pLeds->output();
```
