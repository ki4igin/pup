#ifndef __CHECK_SUM_H
#define __CHECK_SUM_H

#include <stdint.h>

uint8_t checksum_oper_calc(uint8_t *buf, uint32_t size);
uint32_t is_valid_checksum_oper(uint8_t *buf, uint32_t size);
uint32_t is_valid_checksum_kama(uint8_t *buf, uint32_t size);

#endif
