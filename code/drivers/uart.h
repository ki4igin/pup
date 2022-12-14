#ifndef __UART_H
#define __UART_H

#include "MDR32F9Qx_uart.h"

void uart1_init(void);
void uart2_init(void);

void uart_send_buf(MDR_UART_TypeDef *uart, uint8_t *buf, uint32_t size);
uint32_t uart_receive_buf(MDR_UART_TypeDef *uart, uint8_t *buf, uint32_t size,
                          uint32_t timeout);
#endif
