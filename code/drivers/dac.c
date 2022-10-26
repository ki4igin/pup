#include "dac.h"

#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_dac.h"

void dac_all_init(void)
{
    /* DAC Configuration */
    /* Reset all DAC settings */
    DAC_DeInit();
    /* DAC channel1 Configuration */
    DAC1_Init(DAC1_AVCC);
    /* DAC channel1 enable */
    DAC1_Cmd(ENABLE);

    /* DAC channel1 Configuration */
    DAC2_Init(DAC2_AVCC);
    /* DAC channel1 enable */
    DAC2_Cmd(ENABLE);
}
