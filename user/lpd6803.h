#ifndef _LPD6803_H
#define _LPD6803_H

#include "ets_sys.h"

void ICACHE_FLASH_ATTR lpd6803_init();
void ICACHE_FLASH_ATTR lpd6803_strip(uint8_t * data, uint16_t len);

#endif
