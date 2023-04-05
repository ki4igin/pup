#ifndef __PARALAKS_H
#define __PARALAKS_H

#include "MDR32F9Qx_config.h"

typedef struct 
{
    uint32_t az;
    int32_t el;
    uint32_t r;
} SphCoord_t;

extern struct kama_pos {
    uint32_t x;
    uint32_t y;
    uint32_t z;
} kama_pos;


void ParalaksInit(void);
void ParalaksCalc(SphCoord_t* coordIn, SphCoord_t* coordOut);

#endif
