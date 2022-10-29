// Includes --------------------------------------------------------------------
#include "check_sum.h"

#include <stdint.h>

uint32_t checksum_oper_calc(uint8_t *buf, uint32_t size)
{
    uint32_t sum = 0;
		uint32_t i;
    for (i = 0; i < (size - 1); i++) {
        sum += *buf++;
    }
    return (sum & 0xFF);
}


uint32_t is_valid_checksum_oper(uint8_t *buf, uint32_t size)
{
    return ((uint8_t)checksum_oper_calc(buf, size) == buf[size - 1]);
}

uint32_t is_valid_checksum_kama(uint8_t *buf, uint32_t size)
{
    uint32_t sum = 0;
		uint32_t i;
    for (i = 0; i < (size - 1); i++) {
        sum += *buf++;
    }
    while (sum > 0xFF) {
        sum = (sum >> 8) + (sum & 0xFF);
    }

    return ((uint8_t)sum == *buf);
}
