#include "uart.h"

#include "MDR32F9Qx_config.h"
#include "MDR32F9Qx_uart.h"

void uart1_init(void)
{
    UART_InitTypeDef UART_InitStructure = {0};

    UART_DeInit(MDR_UART1);
    UART_BRGInit(MDR_UART1, UART_HCLKdiv1);

    /* Initialize UART_InitStructure */
    UART_InitStructure.UART_BaudRate = 115200;
    UART_InitStructure.UART_WordLength = UART_WordLength8b;
    UART_InitStructure.UART_StopBits = UART_StopBits1;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE
                                                | UART_HardwareFlowControl_TXE;
    UART_Init(MDR_UART1, &UART_InitStructure);
    UART_ITConfig(MDR_UART1, UART_IT_RX, ENABLE);
    UART_Cmd(MDR_UART1, ENABLE);
}

void uart2_init(void)
{
    UART_InitTypeDef UART_InitStructure = {0};

    UART_DeInit(MDR_UART2);
    UART_BRGInit(MDR_UART2, UART_HCLKdiv1);

    /* Initialize UART_InitStructure */
    UART_InitStructure.UART_BaudRate = 115200;
    UART_InitStructure.UART_WordLength = UART_WordLength8b;
    UART_InitStructure.UART_StopBits = UART_StopBits2;
    UART_InitStructure.UART_Parity = UART_Parity_No;
    UART_InitStructure.UART_FIFOMode = UART_FIFO_OFF;
    UART_InitStructure.UART_HardwareFlowControl = UART_HardwareFlowControl_RXE
                                                | UART_HardwareFlowControl_TXE;
    UART_Init(MDR_UART2, &UART_InitStructure);
    UART_ITConfig(MDR_UART2, UART_IT_RX, ENABLE);
    UART_Cmd(MDR_UART2, ENABLE);

    NVIC_EnableIRQ(UART2_IRQn);
}
