#include "paralaks.h"
#include "ParalaxCalc_fixpt.h"
#include "leds.h"

// R_MAX не более (2^17 - 1), вшито в функцию ParalaxCalc_fixpt
#define R_MAX 100000
#define R_MIN 200

const uint32_t base_x = 29;
const uint32_t base_y = 17;
const uint32_t base_z = 11;

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
        (base_x << 24),
        (base_y << 24),
        (base_z << 24),
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
