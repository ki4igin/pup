#include "rcc.h"
#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_rst_clk.h"


void rcc_init(void)
{
    RST_CLK_DeInit();

    RST_CLK_HSEconfig(RST_CLK_HSE_ON);
    while (RST_CLK_HSEstatus() != SUCCESS) {
    }
    /* Select HSE clock as CPU_PLL input clock source */
    /* Set PLL multiplier to 7                        */
    RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul10); // 80MHz
    /* Enable CPU_PLL */
    RST_CLK_CPU_PLLcmd(ENABLE);
    while (RST_CLK_CPU_PLLstatus() != SUCCESS) {
    }
    /* Set CPU_C3_prescaler to 2 */
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);
    /* Set CPU_C2_SEL to CPU_PLL output instead of CPU_C1 clock */
    RST_CLK_CPU_PLLuse(ENABLE);

    /* Select CPU_C3 clock on the CPU clock MUX */
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);

    /* Enable peripheral clocks --------------------------------------------------*/
    RST_CLK_PCLKcmd(RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_TIMER1
                        | RST_CLK_PCLK_PORTE | RST_CLK_PCLK_PORTB
                        | RST_CLK_PCLK_PORTF | RST_CLK_PCLK_DAC
                        | RST_CLK_PCLK_UART1 | RST_CLK_PCLK_UART2
                        | RST_CLK_PCLK_PORTD | RST_CLK_PCLK_TIMER2
                        | RST_CLK_PCLK_TIMER3 | RST_CLK_PCLK_PORTC,
                    ENABLE);
}
