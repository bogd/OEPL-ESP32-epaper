#ifndef _DEV_CONFIG_H_
#define _DEV_CONFIG_H_

#include <Arduino.h>
#include <SPI.h>

#define GPIO_PIN_SET   1
#define GPIO_PIN_RESET 0

#define delay_ms(__xms) vTaskDelay(__xms/portTICK_PERIOD_MS)

void Module_Init(void);
void SPI_WriteByte(uint8_t data);
void SPI_Write_nByte(uint8_t *pData, uint32_t len);

#endif
