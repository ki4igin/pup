#include "paralaks.h"
#include "ParalaxCalc_fixpt.h"
#include "leds.h"

// R_MAX не более (2^17 - 1), вшито в функцию ParalaxCalc_fixpt
#define R_MAX 100000
#define R_MIN 200

struct kama_pos kama_pos = {.x = 29, .y = 17, .z = 11};

void ParalaksCalc(SphCoord_t *coordIn, SphCoord_t *coordOut)
{
    // led_trans_on();
    uint32_t temp_r = coordIn->r;
    if (temp_r >= R_MAX) {
        coordOut->az = coordIn->az;
        coordOut->el = coordIn->el;
        coordOut->r = coordIn->r;
        led_trans_off();
        return;
    } else if ((temp_r < R_MIN) || (temp_r & (1 << 22))){
        temp_r = R_MIN;
    }

    uint32_t degAz = coordIn->az << 17;
    int32_t degEl = coordIn->el << 18;
    // r не более (2^17 - 1), вшито в функцию ParalaxCalc_fixpt
    uint32_t r = temp_r << 15;
    ParalaxCalc_fixpt(
        degAz,
        degEl,
        r,
        (kama_pos.x << 24),
        (kama_pos.y << 24),
        (kama_pos.z << 24),
        &coordOut->az,
        &coordOut->el,
        &coordOut->r);

    coordOut->az >>= 17;
    coordOut->el >>= 18;
    coordOut->r >>= 14;

    // led_trans_off();
}

void ParalaksInit(void)
{
    ParalaxCalc_fixpt_initialize();
}
