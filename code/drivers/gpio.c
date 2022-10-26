#include "gpio.h"
#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_port.h"

void gpio_init(void)
{
    PORT_InitTypeDef PORT_InitStructure = {0};
    
    /* Reset PORTE settings */
    PORT_DeInit(MDR_PORTE);
    PORT_DeInit(MDR_PORTF);
    PORT_DeInit(MDR_PORTB);
    // PORT_DeInit(MDR_PORTA);
    PORT_DeInit(MDR_PORTC);

    // PD15 выход для детектирования платы, подтянуть к земле,
    // Если единица то плата угла места
    PORT_InitStructure.PORT_Pin = PORT_Pin_15;
    PORT_InitStructure.PORT_OE = PORT_OE_IN;
    PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_ON;
    PORT_Init(MDR_PORTD, &PORT_InitStructure);

    /* Configure DAC pin: DAC1_OUT, DAC2_OUT*/
    /* Configure PORTE pin 0, 9 */
    PORT_InitStructure.PORT_Pin = PORT_Pin_0 | PORT_Pin_9;
    PORT_InitStructure.PORT_OE = PORT_OE_OUT;
    PORT_InitStructure.PORT_MODE = PORT_MODE_ANALOG;

    PORT_Init(MDR_PORTE, &PORT_InitStructure);

    /* Configure TIMER1 pins: CH3 */
    /* Configure PORTD pins 12 */
    PORT_InitStructure.PORT_Pin = (PORT_Pin_12);
    PORT_InitStructure.PORT_OE = PORT_OE_IN;
    PORT_InitStructure.PORT_FUNC = PORT_FUNC_MAIN;
    PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;
    PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_ON;
    PORT_Init(MDR_PORTD, &PORT_InitStructure);

    //	/* Configure EXT_INT pin: EXT_INT1*/
    //  /* Configure PORTA pin 0 */
    //	PORT_InitStructure.PORT_Pin   = PORT_Pin_0 ;
    //  PORT_InitStructure.PORT_OE    = PORT_OE_IN;
    //  PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    //	PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
    //	PORT_InitStructure.PORT_FUNC 	=	PORT_FUNC_ALTER;
    //	PORT_InitStructure.PORT_PULL_DOWN = PORT_PULL_DOWN_ON;
    //
    //  PORT_Init(MDR_PORTA, &PORT_InitStructure);

    /* Configure LED pin and UART_EN*/
    /* Configure PORTB pin 11-15 */
    PORT_InitStructure.PORT_Pin = PORT_Pin_11 | PORT_Pin_12 | PORT_Pin_13 | PORT_Pin_14 | PORT_Pin_15;
    PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_OE = PORT_OE_OUT;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
    PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;

    PORT_Init(MDR_PORTB, &PORT_InitStructure);

    /* Configure Switch pin*/
    /* Configure PORTC pin 4-6 */
    PORT_InitStructure.PORT_Pin = PORT_Pin_4 | PORT_Pin_5 | PORT_Pin_6;
    PORT_InitStructure.PORT_FUNC = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_OE = PORT_OE_OUT;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;
    PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;

    PORT_Init(MDR_PORTC, &PORT_InitStructure);

    /* Configure UART pin*/
    PORT_InitStructure.PORT_FUNC = PORT_FUNC_ALTER;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_MAXFAST;

    PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;

    /* Configure PORTB pins 5 (UART1_TX) as output */
    PORT_InitStructure.PORT_OE = PORT_OE_OUT;
    PORT_InitStructure.PORT_Pin = PORT_Pin_5;
    PORT_Init(MDR_PORTB, &PORT_InitStructure);

    /* Configure PORTB pins 6 (UART1_RX) as input */
    PORT_InitStructure.PORT_OE = PORT_OE_IN;
    PORT_InitStructure.PORT_Pin = PORT_Pin_6;
    PORT_Init(MDR_PORTB, &PORT_InitStructure);

    PORT_InitStructure.PORT_FUNC = PORT_FUNC_OVERRID;

    /* Configure PORTF pins 1 (UART2_TX) as output */
    PORT_InitStructure.PORT_OE = PORT_OE_OUT;
    PORT_InitStructure.PORT_Pin = PORT_Pin_1;
    PORT_Init(MDR_PORTF, &PORT_InitStructure);

    /* Configure PORTF pins 0 (UART2_RX) as input */
    PORT_InitStructure.PORT_OE = PORT_OE_IN;
    PORT_InitStructure.PORT_Pin = PORT_Pin_0;
    PORT_Init(MDR_PORTF, &PORT_InitStructure);
}
