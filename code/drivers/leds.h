#ifndef __LEDS_H
#define __LEDS_H

#include "MDR32F9Qx_port.h"

#define LEDS_PORT         MDR_PORTB
#define LED_TRANSPERT_PIN PORT_Pin_15
#define LED_GREEN_PIN     PORT_Pin_14
#define LED_YELLOW_PIN    PORT_Pin_13
#define LED_RED_PIN       PORT_Pin_12
#define LED_ALL_PIN       (LED_RED_PIN | LED_TRANSPERT_PIN | LED_GREEN_PIN | LED_YELLOW_PIN)

inline static void led_on(uint32_t pin)
{
    PORT_SetBits(LEDS_PORT, pin);
}

inline static void led_off(uint32_t pin)
{
    PORT_ResetBits(LEDS_PORT, pin);
}

inline static void led_all_on(void)
{
    led_on(LED_ALL_PIN);
}

inline static void led_all_off(void)
{
    led_off(LED_ALL_PIN);
}

inline static void led_red_on(void)
{
    led_on(LED_RED_PIN);
}

inline static void led_yellow_on(void)
{
    led_on(LED_YELLOW_PIN);
}

inline static void led_trans_on(void)
{
    led_on(LED_TRANSPERT_PIN);
}

inline static void led_green_on(void)
{
    led_on(LED_GREEN_PIN);
}

inline static void led_red_only_on(void)
{
    led_all_off();
    led_on(LED_RED_PIN);
}

inline static void led_yellow_only_on(void)
{
    led_all_off();
    led_on(LED_YELLOW_PIN);
}

inline static void led_trans_only_on(void)
{
    led_all_off();
    led_on(LED_TRANSPERT_PIN);
}

inline static void led_green_only_on(void)
{
    led_all_off();
    led_on(LED_GREEN_PIN);
}

inline static void led_red_off(void)
{
    led_off(LED_RED_PIN);
}

inline static void led_yellow_off(void)
{
    led_off(LED_YELLOW_PIN);
}

inline static void led_green_off(void)
{
    led_off(LED_GREEN_PIN);
}

inline static void led_trans_off(void)
{
    led_off(LED_TRANSPERT_PIN);
}

#endif
