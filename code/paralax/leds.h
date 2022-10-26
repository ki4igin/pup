#ifndef __LEDS_H
#define __LEDS_H

#include "MDR32F9Qx_port.h"

#define LEDS_PORT MDR_PORTB
#define LED_TRANSPERT_PIN PORT_Pin_15
#define LED_GREEN_PIN PORT_Pin_14
#define LED_YELLOW_PIN PORT_Pin_13
#define LED_RED_PIN PORT_Pin_12
#define LED_ALL_PIN (LED_RED_PIN | LED_TRANSPERT_PIN | LED_GREEN_PIN | LED_YELLOW_PIN)

static inline void led_on(uint32_t pin) {
    PORT_SetBits(LEDS_PORT, pin);
}

static inline void led_off(uint32_t pin) {
    PORT_ResetBits(LEDS_PORT, pin);
}

static inline void led_all_on() {
    led_on(LED_ALL_PIN);
}

static inline void led_all_off() {
    led_off(LED_ALL_PIN);
}

static inline void led_red_on() {
    led_on(LED_RED_PIN);
}

static inline void led_yellow_on() {
    led_on(LED_YELLOW_PIN);
}

static inline void led_trans_on() {
    led_on(LED_TRANSPERT_PIN);
}

static inline void led_green_on() {
    led_on(LED_GREEN_PIN);
}

static inline void led_red_off() {
    led_off(LED_RED_PIN);
}

static inline void led_yellow_off() {
    led_off(LED_YELLOW_PIN);
}

static inline void led_green_off() {
    led_off(LED_GREEN_PIN);
}

static inline void led_trans_off() {
    led_off(LED_TRANSPERT_PIN);
}

#endif
