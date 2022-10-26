// Includes --------------------------------------------------------------------
#include "paralaks.h"
#include "ParalaxCalc_fixpt.h"
#include "leds.h"
// Private Typedef -------------------------------------------------------------

// Private Macro ---------------------------------------------------------------
const uint32_t base_x = 29;
const uint32_t base_y = 17;
const uint32_t base_z = 11;
// Private Variables -----------------------------------------------------------

// Private Function prototypes -------------------------------------------------

// Functions -------------------------------------------------------------------
void ParalaksCalc(SphCoord_t* coordIn, SphCoord_t* coordOut)
{
    // led_trans_on();
    if (coordIn->r > (1UL << 17) - 1)
    {
        coordOut->az = coordIn->az;
        coordOut->el = coordIn->el;
        coordOut->r = coordIn->r;
        led_trans_off();  
        return;
    }
    
    uint32_t degAz = coordIn->az << 17;
    int32_t  degEl = coordIn->el << 18;
    uint32_t r     = coordIn->r << 15;

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
