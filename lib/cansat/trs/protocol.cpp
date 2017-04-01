/*
 * protocol.cpp
 *
 *  Created on: 1 апр. 2017 г.
 *      Author: Cntgfy
 */
#include "protocol.h"

int parseReqest(trs_request* req, uint8_t* pack) {
	if (pack[0] != 0x00) {
		return 0;
	}

	if (pack[1] != 0x3A) {
		return 0;
	}

	switch (pack[2]) {
	case 0x00: req->isNeedResponsePack = false; break;
	case 0x01: req->isNeedResponsePack = true;  break;
	default  : return 0;
	}

	return 1;
}

