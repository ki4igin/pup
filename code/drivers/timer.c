#include "timer.h"

#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_timer.h"


// Timer for offset sin and cos signal
void timer1_init()
{
    TIMER_DeInit(MDR_TIMER1);
    MDR_RST_CLK->TIM_CLOCK = 0x07000000;
    MDR_TIMER1->CNTRL = 0x00000000;
    MDR_TIMER1->CNT = 0x00000000;
    MDR_TIMER1->PSG = 0x00000027; //= 80/40 =2MHZ
    MDR_TIMER1->ARR = 0x000016A8; // 5800 = 2,8ms // zapitka - 384MHz= 2,604ms
    MDR_TIMER1->IE = 0x00000080;  // enable interrupt - capture chan. 3
    TIMER_BRGInit(MDR_TIMER1, TIMER_HCLKdiv1);
    MDR_TIMER1->CH3_CNTRL = 0x8000; // enable Chanel1 in capture mode
}

// Timer for count first half-period
void timer2_init()
{
    TIMER_DeInit(MDR_TIMER2);
    MDR_TIMER2->CNT = 0x00000000;
    MDR_TIMER2->PSG = 0x00000004; //= 80/5 =16MHZ
    MDR_TIMER2->ARR = 0x00005160; //=20832( tau = 1,302ms)

    TIMER_BRGInit(MDR_TIMER2, TIMER_HCLKdiv1);
    MDR_TIMER2->IE = 0x2;
    NVIC_EnableIRQ(Timer2_IRQn);
}


// Capture - 1
void timer3_init()
{
    TIMER_DeInit(MDR_TIMER3);
    MDR_TIMER3->CNT = 0x00000000;
    MDR_TIMER3->PSG = 0x00000003; //= 80/4 =20MHZ
    MDR_TIMER3->ARR = 0x000030D4; //=12500( tau = 0,625ms)    
    TIMER_BRGInit(MDR_TIMER3, TIMER_HCLKdiv1);
    MDR_TIMER3->IE = 0x2;
    NVIC_EnableIRQ(Timer3_IRQn);
}

