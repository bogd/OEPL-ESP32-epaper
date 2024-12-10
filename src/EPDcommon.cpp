#include "EPDcommon.h"

void GPIO_Config(void) {
    pinMode(EPD_BUSY_PIN,  INPUT);
    pinMode(EPD_RST_PIN , OUTPUT);
    pinMode(EPD_DC_PIN  , OUTPUT);
    
    pinMode(EPD_SCK_PIN, OUTPUT);
    pinMode(EPD_MOSI_PIN, OUTPUT);
    pinMode(EPD_CS_PIN , OUTPUT);

    digitalWrite(EPD_CS_PIN , HIGH);
    digitalWrite(EPD_SCK_PIN, LOW);
}

void Module_Init(void) {
	GPIO_Config();
	//SPI.begin(EPD_SCK_PIN, -1, EPD_MOSI_PIN, EPD_CS_PIN);
	//SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
}

void SPI_WriteByte(uint8_t data)
{
	// SPI.beginTransaction(spi_settings);
	// SPI.transfer(data);
	// SPI.endTransaction();

    // bit-banged SPI. For some reason, 'real' spi doesn't work with the spectra display on the s2 mini

	digitalWrite(EPD_CS_PIN, GPIO_PIN_RESET);

	for (int i = 0; i < 8; i++)
    {
        if ((data & 0x80) == 0) digitalWrite(EPD_MOSI_PIN, GPIO_PIN_RESET); 
        else                    digitalWrite(EPD_MOSI_PIN, GPIO_PIN_SET);

        data <<= 1;
        digitalWrite(EPD_SCK_PIN, GPIO_PIN_SET);     
        digitalWrite(EPD_SCK_PIN, GPIO_PIN_RESET);
    }

    digitalWrite(EPD_CS_PIN, GPIO_PIN_SET);
}

/*
void SPI_WriteByte(uint8_t data) {
	SPI.transfer(data);
}
*/

void SPI_Write_nByte(uint8_t *pData, uint32_t len) {
	for (int i = 0; i < len; i++)
		SPI_WriteByte(pData[i]);
}
