#include "user_config.h"
#include "ets_sys.h"
#include "osapi.h"
#include "gpio.h"
#include "os_type.h"
#include "lpd6803.h"
#include "tpm2net.h"

void ICACHE_FLASH_ATTR user_init() {
	system_timer_reinit();

	wifi_set_opmode( 2 ); 
	wifi_station_dhcpc_start();

	lpd6803_init();
	tpm2net_init();
}
