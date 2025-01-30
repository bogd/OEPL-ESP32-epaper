#include <Arduino.h>


// *** select your display type here
// #define EPD_401_ACEP
// #define EPD_730_SPECTRA
#define EPD_750_WAVESHARE_V2
// ***************************



#ifdef EPD_401_ACEP
#define HWTYPE 0xC1 // ACEP
#define IMGWIDTH 640
#define IMGHEIGHT 400
#endif

#ifdef EPD_730_SPECTRA
// board with psram needed
#define HWTYPE 0xC2 // Spectra
#define IMGWIDTH 800
#define IMGHEIGHT 480
#endif

#ifdef EPD_750_WAVESHARE_V2
// board with psram needed
#define HWTYPE 0x36 // Waveshare 7.5 - reusing 36 (BWR display), should actually get a new HWID
#define IMGWIDTH 800
#define IMGHEIGHT 480
#endif


void drawImage(uint8_t *buffer, uint8_t dataType);
