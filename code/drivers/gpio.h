#ifndef __GPIO_H
#define __GPIO_H

#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_port.h"

#define RELAYS_PORT MDR_PORTC
#define RELAY1_PIN  PORT_Pin_4
#define RELAY2_PIN  PORT_Pin_5
#define RELAY3_PIN  PORT_Pin_6

void gpio_init(void);

inline static void relays_off(void)
{
    PORT_ResetBits(MDR_PORTC, PORT_Pin_4);
    PORT_ResetBits(MDR_PORTC, PORT_Pin_5);
    PORT_ResetBits(MDR_PORTC, PORT_Pin_6);
}

inline static void relays_on(void)
{
    PORT_SetBits(MDR_PORTC, PORT_Pin_4);
    PORT_SetBits(MDR_PORTC, PORT_Pin_5);
    PORT_SetBits(MDR_PORTC, PORT_Pin_6);
}

#endif
