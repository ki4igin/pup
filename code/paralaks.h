#ifndef __PARALAKS_H
#define __PARALAKS_H

// Includes --------------------------------------------------------------------
#include "MDR32F9Qx_config.h"
// Typedef ---------------------------------------------------------------------
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

// Macro -----------------------------------------------------------------------

// Variables -------------------------------------------------------------------

// Function prototypes ---------------------------------------------------------
void ParalaksInit(void);
void ParalaksCalc(SphCoord_t* coordIn, SphCoord_t* coordOut);
// Inline Functions ------------------------------------------------------------

#endif
