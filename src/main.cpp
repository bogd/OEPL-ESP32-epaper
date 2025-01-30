#include <Arduino.h>
#include <SPI.h>
#include <WiFi.h>

#include "commstructs.h"
#include "main.h"
#include "udp.h"
#include "EPDcommon.h"
#include "ImageData.h"

//
// *** select your display type in main.h !!
//

#ifdef EPD_401_ACEP
#include "EPD_4in01f.h"
#endif
#ifdef EPD_730_SPECTRA
#include "EPD_7in3e.h"
#endif
#ifdef EPD_750_WAVESHARE_V2
#include "EPD_7in5_V2.h"
#endif

#define WAKEUP_REASON_FIRSTBOOT 0xFC

const char *ssid = "****";
const char *password = "****";

void advertiseTagTask(void *parameter) {
	sendAvail(WAKEUP_REASON_FIRSTBOOT);
	while (true) {
		vTaskDelay(60000 / portTICK_PERIOD_MS);
		sendAvail(0);
		Serial.println("checkin");
	}
}

void drawImage(uint8_t *buffer, uint8_t dataType) {
	Serial.println("init datatype " + String(dataType));

#ifdef EPD_401_ACEP
	EPD_4IN01F_Init();
	Serial.println("display image");
	EPD_4IN01F_Display(buffer);
	Serial.println("sleep");
	EPD_4IN01F_Sleep();
#endif

#ifdef EPD_730_SPECTRA
	EPD_7IN3E_Init();
	Serial.println("display image");
	EPD_7IN3E_Display(buffer);
	Serial.println("sleep");
	EPD_7IN3E_Sleep();
#endif

#ifdef EPD_750_WAVESHARE_V2
	EPD_7IN5_V2_Init();
	Serial.println("display image");
	EPD_7IN5_V2_Display(buffer);
	Serial.println("sleep");
	EPD_7IN5_V2_Sleep();
#endif
}

void setup() {
	Serial.begin(115200);
	Serial.println("ACEP/Spectra/Waveshare demo");

	Serial.printf("Heap size: %u\n", ESP.getHeapSize());
	Serial.printf("Free heap: %u\n", ESP.getFreeHeap());
	Serial.printf("Largest free block: %u\n", ESP.getMaxAllocHeap());

	WiFi.begin(ssid, password);
	while (WiFi.status() != WL_CONNECTED) {
		Serial.print(".");
		delay(500);
	}
	Serial.println("wifi connected");

	Module_Init();

	/* Demo only, to make sure the display code works */
	Serial.println("Entering demo code");
	EPD_7IN5_V2_Init();
	EPD_7IN5_V2_Display(epd_bitmap_help_me);
	EPD_7IN5_V2_Sleep();
	Serial.println("Demo done, resuming");

	init_udp();
	xTaskCreate(advertiseTagTask, "Tag alive", 6000, NULL, 2, NULL);
}

void loop() {
	if (WiFi.status() != WL_CONNECTED) {
		Serial.println("WiFi disconnected. Attempting to reconnect...");

		WiFi.disconnect();
		WiFi.reconnect();

		int attempts = 0;
		while (WiFi.status() != WL_CONNECTED && attempts < 10) {
			Serial.print(".");
			delay(500);
			attempts++;
		}

		if (WiFi.status() == WL_CONNECTED) {
			Serial.println("\nReconnected to WiFi");
			init_udp();
		} else {
			Serial.println("\nFailed to reconnect");
			delay(5000);
		}
	}
	vTaskDelay(10000 / portTICK_PERIOD_MS);
}
