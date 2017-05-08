/*
 * main.c
 *
 *  Created on: 7 мая 2017 г.
 *      Author: Cntgfy
 */

#include "adapter.h"
extern "C" {
#include "rfm23bp.h"
}

#include "shm/redis.h"
#include "structs.h"

const DATA_TYPE types[] = {TYPE_TERMO};

void setup_rfm();

void data_parse(uint8_t* pack_data, int pack_size, resp_s* resp) {
	resp->type = *(uint8_t* )pack_data;
	resp->sec  = *(int32_t*)(pack_data + 2);
	resp->ms   = *(uint16_t*)(pack_data + 7);
	resp->data =  (char*)(pack_data + 9);
	resp->dataSize = pack_size - 9;
}

void data_set(resp_s* resp, uint8_t* pack_data, int* pack_size) {
	pack_data[0] = resp->type;
	pack_data[1] = ':';

	*( (int*)(pack_data+2) ) = resp->sec;

	pack_data[6] = ':';

	*((uint16_t*)(pack_data+7)) = resp->ms;

	resp->data = (char*)(pack_data+9);

	*pack_size = 9 + resp->dataSize;
}

int main() {
	setup_rfm();

	redisDataService* dserv = new redisDataService();
	dserv->connect();

	int types_count = sizeof(types) / sizeof(types[0]);

	char buffs[types_count][64 + 1];
	resp_s resps[types_count];
	resp_s old_resps[types_count];

	uint8_t pack_data[64];

	for (int i = 0; i < types_count; i++) {
		resps[i].data = buffs[i];
	}

	while (1) {
		for (int i = 0; i < types_count; i++) {
			usleep(50000);
			if ( !dserv->getLast_(types[i], &resps[i]) ) {
				continue;
			}

			if (resps[i].type == old_resps[i].type
			&&  resps[i].sec  == old_resps[i].sec
			&&  resps[i].ms   == old_resps[i].ms) {
				continue;
			}

			old_resps[i].type = resps[i].type;
			old_resps[i].sec = resps[i].sec;
			old_resps[i].ms = resps[i].ms;

			int pack_size;
			data_set(&resps[i], pack_data, &pack_size);

			if (pack_size > 64) {
				printf("Pack size to large\n");
				exit(1);
			}

			if ( rfm23bp_transmite(pack_data, pack_size) == 0 ) {
				printf("Transmit failure\n");
				sleep(1);
				continue;
			}

			pack_data[64] = '\0';
			printf("Transmit packSize=%d, type=%d success\n", pack_size, resps[i].type);

			usleep(500000);
		}
	}

	char data[65] = "Hello!\0";

	if ( rfm23bp_transmite((uint8_t*)data, 64) != 0 ) {
		//printf("Message received: '%s'\n", data);
		printf("Transmit success\n");
	}

	return 0;
}

void setup_rfm() {
	rfm23bp_init(&params);
	rfm23bp_mode_spi(RFM23_SLAVE_SELECT_PIN);
	rfm23bp_ready();
	rfm23bp_set_default_radio_settings();
	rfm23bp_set_power_level(level_4);

	rfm23bp_set_register(RFM23BP_INTERRUPT_ENABLE_1,0x06);//0x02
	rfm23bp_set_register(RFM23BP_INTERRUPT_ENABLE_2,0x00);

	printf("INTERRUPT_STATUS_ENABLE_1: %d\n", rfm23bp_read_register(RFM23BP_INTERRUPT_ENABLE_1));

	rfm23bp_set_register(RFM23BP_GPIO0_CONFIGURATION, 0x12) ; // TX state
	rfm23bp_set_register(RFM23BP_GPIO1_CONFIGURATION, 0x15) ; // RX state

	rfm23bp_set_register(RFM23BP_INTERRUPT_STATUS_1, 0x00);

	uint8_t dev_type = rfm23bp_read_register(RFM23BP_DEVICE_TYPE);
	uint8_t dev_version = rfm23bp_read_register(RFM23BP_DEVICE_VERSION);
	printf("%d\n", dev_type);
	printf("%d\n", dev_version);
}
