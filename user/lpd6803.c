#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "lpd6803.h"

static uint16_t lpd6803_pixels[maxLEDs];
static uint16_t numLEDs = maxLEDs;

static int lpd6803_SendMode;
static uint32_t lpd6803_BitCount;
static uint16_t lpd6803_LedIndex;
static uint32_t lpd6803_lastdata = 0;

static os_timer_t lpd6803_timer;

void ICACHE_FLASH_ATTR lpd6803_LedOut() {
	switch (lpd6803_SendMode) {
		case LPD6803_DONE:
			break;

		case LPD6803_DATA:
			if ((1 << (15 - lpd6803_BitCount)) & lpd6803_pixels[lpd6803_LedIndex]) {
				if (!lpd6803_lastdata) {
					GPIO_OUTPUT_SET(0, 1);
					lpd6803_lastdata = 1;
				}
			} else {
				if (lpd6803_lastdata) {
					GPIO_OUTPUT_SET(0, 0);
					lpd6803_lastdata = 0;
				}
			}
			lpd6803_BitCount++;

			if (lpd6803_BitCount == 16) {
				lpd6803_LedIndex++;
					if (lpd6803_LedIndex < numLEDs) {
						lpd6803_BitCount = 0;
					} else {
						GPIO_OUTPUT_SET(0, 0);
						lpd6803_lastdata = 0;
						lpd6803_SendMode = LPD6803_DONE;
					}
			}
			break;

		case LPD6803_HEADER:
			if (lpd6803_BitCount < 32) {
				GPIO_OUTPUT_SET(0, 0);
				lpd6803_lastdata = 0;
				lpd6803_BitCount++;
				if (lpd6803_BitCount == 32) {
					lpd6803_LedIndex = 0;
					lpd6803_BitCount = 0;
					lpd6803_SendMode = LPD6803_DATA;
				}
			}
			break;

		case LPD6803_START:
			lpd6803_LedIndex = 0;
			lpd6803_BitCount = 0;
			lpd6803_SendMode = LPD6803_HEADER;
			break;
	}

	GPIO_OUTPUT_SET(2, 1);
	GPIO_OUTPUT_SET(2, 0);
}

void ICACHE_FLASH_ATTR lpd6803_setPixelColor(uint16_t n, uint8_t r, uint8_t g, uint8_t b) {
	uint16_t data;

	if (n > numLEDs)
		return;

	data = (int) ((g / 255.0f) * 31);
	data <<= 5;
	data |= (int) ((r / 255.0f) * 31);
	data <<= 5;
	data |= (int) ((b / 255.0f) * 31);
	data |= 0x8000;

	lpd6803_pixels[n] = data;
}

void ICACHE_FLASH_ATTR lpd6803_setAllPixelColor(uint8_t r, uint8_t g, uint8_t b) {
	uint16_t i;

	for (i = 0; i < numLEDs; i++) {
		lpd6803_setPixelColor(i, r, g, b);
	}
}

void ICACHE_FLASH_ATTR lpd6803_init() {
	gpio_init();
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
	GPIO_OUTPUT_SET(2, 0);

	PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
	GPIO_OUTPUT_SET(0, 0);

	lpd6803_setAllPixelColor(0, 0, 0);

	os_timer_disarm(&lpd6803_timer);
	os_timer_setfn(&lpd6803_timer, (os_timer_func_t *) lpd6803_LedOut, NULL);
	os_timer_arm_us(&lpd6803_timer, 40, 1);

	lpd6803_show();
}

void ICACHE_FLASH_ATTR lpd6803_show(void) {
	lpd6803_SendMode = LPD6803_START;
}

void ICACHE_FLASH_ATTR lpd6803_strip(uint8_t * buffer, uint16_t length) {
	uint16_t i;

	if (lpd6803_SendMode != LPD6803_DONE) {
		return;
	}

	numLEDs = length/3;

	for( i = 0; i < numLEDs; i++ ) {
		uint16_t cled = i * 3;
		lpd6803_setPixelColor(i, buffer[cled], buffer[cled+1], buffer[cled+2]);
	}
	lpd6803_show();
}

