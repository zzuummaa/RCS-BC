/*
 * protocol.h
 *
 *  Created on: 1 апр. 2017 г.
 *      Author: Cntgfy
 */

#ifndef CANSAT_TRS_PROTOCOL_H_
#define CANSAT_TRS_PROTOCOL_H_

#include <stdint.h>

typedef struct {
	bool isNeedResponsePack;
} trs_request;

#define RETURN_PACK_TRUE	0x01
#define RETURN_PACK_FALSE	0x00

int parseReqest(trs_request* req, uint8_t* pack);

#endif /* CANSAT_TRS_PROTOCOL_H_ */
