/////  AZIMUT///////////////

/* Includes ------------------------------------------------------------------*/
// #include "MDR32F9Qx_board.h"
#include "MDR32F9Qx_config.h"
#include "MDR32Fx.h"
#include "MDR32F9Qx_uart.h"
#include "MDR32F9Qx_port.h"
#include "MDR32F9Qx_rst_clk.h"
#include "MDR32F9Qx_dac.h"
#include "MDR32F9Qx_timer.h"
// #include "MDR32F9Qx_it.h"
#include "paralaks.h"
#include "leds.h"
#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "dac.h"
#include "timer.h"
#include "sin_signal.h"
#include "cor_offset_amp.h"
#include "check_sum.h"
#include "version.h"

int16_t cor_oper = 0;
int16_t cor_kama = 0;

uint8_t ReciveByte = 0x00;

uint32_t Period_Massiv[2048]; // for debug

/* Variables for DAC ---------------------------------------------------------*/
int16_t Sin_high = 0; /// angle = 91,3 grad
int16_t Sin_low = 4095;
int16_t Cos_high = 71;
int16_t Cos_low = 0;

int16_t current_deg_oper = 460;
int16_t current_deg_kama = 0;
volatile int16_t last_angle = 0;
volatile int16_t next_angle2 = 0;
volatile int16_t next_angle_mode = 0;

volatile int16_t next_Sin_high = 0;
volatile int16_t next_Cos_high = 0;
volatile int16_t next_Sin_low = 0;
volatile int16_t next_Cos_low = 0;

/* Variables for offset sin,cos ---------------------------------------------------------*/
uint16_t Sin_delay = 0;

uint8_t NewData = 0;

uint16_t period = 0;
uint32_t period_us = 0;

/* Variables for UART ---------------------------------------------------------*/
#define RX_OPER_SIZE 6
#define TX_OPER_SIZE 6
#define RX_KAMA_SIZE 26

struct rx_buf {
    uint32_t cnt;
    uint8_t data[];
};

struct rx_buf rx_buf_oper = {
    .cnt = 0,
    .data = {[0 ... RX_OPER_SIZE - 1] = 0}};

struct rx_buf rx_buf_kama = {
    .cnt = 0,
    .data = {[0 ... RX_KAMA_SIZE - 1] = 0}};

uint8_t flag_rx_ready = 0;
uint8_t flag_tx_ready = 0;
uint8_t flag_rx_cu = 0;

uint8_t flag_zapit = 0;
uint8_t flag_not_zapit = 0;
uint8_t cnt_systick = 0;

uint16_t New_offset = 0;

uint8_t i;

/*--------------------------------------------------------------*/

typedef enum {
    None = 0x00,
    Up = 0x01,
    Down = 0x10
} Angle_Out_Mode_TypeDef;

#define IS_Angle_Out_Mode(Angle_Out_Mode) (((Angle_Out_Mode) == None) || ((Angle_Out_Mode) == Up) || ((Angle_Out_Mode) == Down))

Angle_Out_Mode_TypeDef Angle_Out_Mode = None;

enum mode {
    MODE_OFF = 0,
    MODE_OPER = 1,
    MODE_KAMA = 2
} mode = MODE_OFF;

#define IS_MODE_STATE(STATE) (((STATE) == MODE_OFF) || ((STATE) == MODE_OPER) || ((STATE) == MODE_KAMA))

uint8_t Pre_Complex = 0;
uint8_t Pre_Complex_Kama = 0;

uint8_t count_mode_angle = 0;
uint32_t counter_ext = 0;
uint32_t counter_ext2 = 0;

uint16_t count_tim1 = 0;
uint16_t count_tim2 = 0;
uint16_t count_tim3 = 0;

uint16_t count_error_ext = 0;

uint8_t count_tx = 0;

uint16_t d_period = 20832; // = 1,302 ms

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

void Calc_Ampl(int32_t deg, int32_t cor);
void Timer1_Init(void);
void Timer2_Init(void);
void Timer3_Init(void);
void Parallaks(void);

enum cmd {
    CMD_MODE = 0x1,
    CMD_DEG = 0x2,
    CMD_SHIFT = 0x3,
    CMD_NO_ZAP = 0x4,
    CMD_ZAP_SMALL = 0x5,
    CMD_COR_OPER = 0x6,
    CMD_COR_KAMA = 0x7,
    CMD_VER = 0xF,
};

static enum pup {
    PUP_AZ = 0,
    PUP_EL = 1
} pup;

const uint8_t pup_id[] = {
    [PUP_AZ] = 0x1,
    [PUP_EL] = 0x2,
};

static uint32_t cnt_rx_kama = 0;

// static uint8_t get_cmd_id(enum pup p, enum cmd c)
//{
//     uint8_t id = ((p + 1) << 4) | (c & 0xF);
//     return id;
// }

static void send_cmd(enum cmd cmd, uint32_t arg)
{
    uint8_t tx_buf[TX_OPER_SIZE] = {0};
    tx_buf[0] = (pup_id[pup] << 4) | cmd;
    *(uint32_t *)&tx_buf[1] = __REV(arg);
    tx_buf[TX_OPER_SIZE - 1] = checksum_oper_calc(tx_buf, TX_OPER_SIZE);
    uart_send_buf(MDR_UART2, tx_buf, TX_OPER_SIZE);
}

static uint32_t isvalid_kama_data(SphCoord_t coord)
{
    static SphCoord_t coord_old;
    if (cnt_rx_kama++ < 3) {
        coord_old = coord;
        return 1;
    }

    // Умножаем углы на два для автоматического переполнения
    int16_t delta_az = (int16_t)(coord_old.az * 2 - coord.az * 2);
    // 256 * 180 / 2^14 = 2.8125 град
    if ((delta_az > 256 * 2) || (delta_az < -256 * 2)) {
        return 0;
    }

    int16_t delta_el = (int16_t)(coord_old.el - coord.el);
    // 256 * 180 / 2^14 = 2.8125 град
    if ((delta_el > 256) || (delta_el < -256)) {
        return 0;
    }

    coord_old = coord;
    return 1;
}

SphCoord_t N_Coord_Kama, N_Coord_MRL;

#ifdef __CC_ARM
int main(void)
#else
void main(void)
#endif
{
    /* Disable all interrupt */
    NVIC->ICPR[0] = 0xFFFFFFFF;
    NVIC->ICER[0] = 0xFFFFFFFF;

    rcc_init();
    gpio_init();

    relays_off();
    led_red_on();

    pup = PORT_ReadInputDataBit(MDR_PORTD, PORT_Pin_15) ? PUP_EL : PUP_AZ;

    timer1_init();
    timer2_init();
    timer3_init();
    New_offset = MDR_TIMER3->ARR;

    uart1_init();
    uart2_init();

    dac_all_init();

    /* Init Angle as 46 grad */ //// ??????????????
    Calc_Ampl(460, 0);

    /* Set PIN SHDN of RS232 */
    PORT_SetBits(MDR_PORTB, PORT_Pin_11);

    /* Init System Timer */
    SysTick->LOAD = 0xFFFFFF; // 0.2s
    SysTick->CTRL = 0x7;

    // ParalaxCalc_fixpt_initialize();
    ParalaksInit();

    while (1) {
        if (flag_not_zapit == 1) {
            flag_not_zapit = 0;
            flag_zapit = 0;
            cnt_systick = 0;
            relays_off();
            led_red_only_on();

            counter_ext = 0;
            counter_ext2 = 0;
            mode = MODE_OFF;
            NVIC_DisableIRQ(Timer1_IRQn); //
            TIMER_Cmd(MDR_TIMER3, DISABLE);
            MDR_TIMER3->CNT = 0;
            TIMER_Cmd(MDR_TIMER1, DISABLE);
            MDR_TIMER1->CNT = 0;

            send_cmd(CMD_NO_ZAP, 1);
        }

        if (flag_rx_cu) // if recieve celeukazaniya
        {
            flag_rx_cu = 0;

            if (is_valid_checksum_kama((uint8_t *)&rx_buf_kama.data, RX_KAMA_SIZE)) {
                uint8_t *data = (uint8_t *)&rx_buf_kama.data;

                uint32_t az_first = (data[13] >> 2);
                az_first = az_first + (data[12] << 5);
                az_first = az_first + ((data[11] & 0x07) << 12); // = 15grad
                //////////////////////////////////
                //			az_first = (az_first*225)>>11;
                ///////////////////////////////////

                int32_t um_first = (data[20] >> 2) + (data[19] << 5) + ((data[18] & 0x0F) << 12);
                if (data[18] & 0x40) {
                    um_first = 0 - um_first;
                }
                uint32_t R_first = (data[17]) + (data[16] << 7) + (data[15] << 14) + (data[14] >> 21); // = 3km

                N_Coord_Kama.az = az_first;
                N_Coord_Kama.el = um_first;
                N_Coord_Kama.r = R_first;

                // фильтрация данных камы
                if (isvalid_kama_data(N_Coord_Kama) == 0) {
                    continue;
                }

                ParalaksCalc(&N_Coord_Kama, &N_Coord_MRL);

                int32_t deg_temp = 0;

                if (pup == PUP_AZ) {
                    deg_temp = N_Coord_MRL.az;
                } else {
                    deg_temp = N_Coord_MRL.el;
                }

                deg_temp = ((deg_temp * 3600) >> 15);

                current_deg_kama = deg_temp;
                Calc_Ampl(deg_temp, cor_kama);
            }
            /////
        } else {
            if (flag_rx_ready == 1) {
                flag_rx_ready = 0;

                uint8_t *rx_data = (uint8_t *)rx_buf_oper.data;

                if (is_valid_checksum_oper(rx_data, RX_OPER_SIZE)) {
                    uint8_t cmd = rx_data[0] & 0xF;
                    if (cmd == CMD_MODE) {
                        switch (rx_data[4]) {
                        case MODE_OFF:
                            relays_off();
                            led_red_only_on();
                            counter_ext = 0;
                            counter_ext2 = 0;
                            mode = MODE_OFF;
                            NVIC_DisableIRQ(Timer1_IRQn); // enable interrupt Timer1
                            TIMER_Cmd(MDR_TIMER1, DISABLE);
                            MDR_TIMER1->CNT = 0;
                            TIMER_Cmd(MDR_TIMER3, DISABLE);
                            MDR_TIMER3->CNT = 0;
                            NVIC_DisableIRQ(UART1_IRQn);
                            break;
                        case MODE_OPER:
                            flag_not_zapit = 0;
                            flag_zapit = 0;
                            cnt_systick = 0;
                            relays_on();
                            led_yellow_only_on();

                            // Mode = MODE_OPER;
                            if (mode == MODE_OFF) {
                                Pre_Complex = 1;
                            } else {
                                mode = MODE_OPER;
                            }
                            counter_ext = 0;
                            count_error_ext = 0;
                            counter_ext2 = 0;
                            NVIC_DisableIRQ(UART1_IRQn);
                            break;

                        case MODE_KAMA: // angle from KAMA
                            flag_not_zapit = 0;
                            flag_zapit = 0;
                            cnt_systick = 0;
                            relays_on();
                            led_green_only_on();

                            if (mode == MODE_OFF) {
                                Pre_Complex_Kama = 1;
                            } else {
                                mode = MODE_KAMA;
                            }
                            // Mode = MODE_KAMA;
                            counter_ext = 0;
                            count_error_ext = 0;
                            counter_ext2 = 0;
                            cnt_rx_kama = 0;
                            NVIC_EnableIRQ(UART1_IRQn);

                            break;
                        }
                    } else if (cmd == CMD_DEG) {
                        if (mode == MODE_KAMA) {
                        } else {
                            current_deg_oper = (rx_data[3] << 8) + rx_data[4];
                            Calc_Ampl(current_deg_oper, cor_oper);
                        }
                    } else if (cmd == CMD_SHIFT) {
                        //	flag_offset = 1 ;
                        New_offset = (rx_data[3] << 8) + rx_data[4] + 1;
                    } else if (cmd == CMD_COR_OPER) {
                        cor_oper = (rx_data[1] << 8) + rx_data[2];
                        Calc_Ampl(current_deg_oper, cor_oper);
                    } else if (cmd == CMD_COR_KAMA) {
                        cor_oper = (rx_data[1] << 8) + rx_data[2];
                        Calc_Ampl(current_deg_kama, cor_kama);
                    } else if (cmd == CMD_VER) {
                        send_cmd(CMD_VER, VERSION);
                    }
                    // WTF ????????????????????????????????????????????????????
                    else if (cmd == 0x8B) {
                        d_period = (rx_data[1] << 8) + rx_data[2];
                    }
                    // ???????????????????????????????????????????????????????
                }
            }
            if (flag_tx_ready == 1) {
                flag_tx_ready = 0;
                count_tx++;
                if (count_tx == 1) {
                    send_cmd(CMD_SHIFT, New_offset);
                    // uart_send_buf(MDR_UART2, TxMassivOffset, TX_OPER_SIZE);
                } else {
                    count_tx = 0;
                    send_cmd(CMD_MODE, mode);
                    // uint8_t tx_buf[TX_OPER_SIZE] = {0};
                    // tx_buf[0] = (pup_id[pup] << 4) | CMD_MODE;
                    // tx_buf[4] = mode;
                    // tx_buf[TX_OPER_SIZE - 1] = checksum_oper_calc(tx_buf, TX_OPER_SIZE);
                    // uart_send_buf(MDR_UART2, tx_buf, TX_OPER_SIZE);
                }
            }
        }
    }
}

void SysTick_Handler(void)
{
    if (Pre_Complex == 1) {
        cnt_systick++;
        if (cnt_systick == 2) // wait switch on relay
        {
            Pre_Complex = 0;
            cnt_systick = 0;
            mode = MODE_OPER;
            MDR_TIMER1->CNT = 0;
            TIMER_Cmd(MDR_TIMER1, ENABLE);
            NVIC_EnableIRQ(Timer1_IRQn); // enable interrupt Timer1
        }
    }
    if (mode == MODE_OPER) {
        if (flag_zapit == 0) {
            cnt_systick++;
            if (cnt_systick == 2) {
                flag_not_zapit = 1;
                cnt_systick = 0;
            }
        }
    }
    if (Pre_Complex_Kama == 1) {
        cnt_systick++;
        if (cnt_systick == 2) // wait switch on relay
        {
            Pre_Complex_Kama = 0;
            cnt_systick = 0;
            mode = MODE_KAMA;
            MDR_TIMER1->CNT = 0;
            TIMER_Cmd(MDR_TIMER1, ENABLE);
            NVIC_EnableIRQ(Timer1_IRQn); // enable interrupt Timer1
        }
    }
    if (mode == MODE_KAMA) {
        if (flag_zapit == 0) {
            cnt_systick++;
            if (cnt_systick == 2) {
                flag_not_zapit = 1;
                cnt_systick = 0;
            }
        }
    }
    flag_zapit = 0;
    flag_tx_ready = 1;
}

void Calc_Ampl(int32_t deg, int32_t cor)
{
    // shift bitwin zeros VT-sensor and drive-sensor
    const int32_t sensor_shift[] = {
        [PUP_AZ] = -1350,
        [PUP_EL] = 0};

    uint32_t ind_cor_array = deg < 0 ? 3600 + deg : deg;

    deg += cor + sensor_shift[pup] - cor_offset_amp[pup][ind_cor_array];
    //		deg += cor + sensor_shift[pup];
    if (deg > 1800) {
        deg = deg - 3600;
    } else if (deg <= -1800) {
        deg = deg + 3600;
    }

    if (deg < -900) {
        next_Sin_low = sin_signal[1800 + deg];
        next_Cos_low = sin_signal[-900 - deg];
        next_Sin_high = 0;
        next_Cos_high = 0;
    } else if (deg < 0) {
        next_Sin_low = sin_signal[-deg];
        next_Cos_low = 0;
        next_Sin_high = 0;
        next_Cos_high = sin_signal[900 + deg];
    } else if (deg < 900) {
        next_Sin_low = 0;
        next_Cos_low = 0;
        next_Sin_high = sin_signal[deg];
        next_Cos_high = sin_signal[900 - deg];
    } else {
        next_Sin_low = 0;
        next_Cos_low = sin_signal[-900 + deg];
        next_Sin_high = sin_signal[1800 - deg];
        next_Cos_high = 0;
    }
    NewData = 1;
}

void UART2_IRQHandler(void)
{
    if (UART_GetFlagStatus(MDR_UART2, UART_FLAG_RXFF) == SET) {
        uint8_t rx_byte = (uint8_t)UART_ReceiveData(MDR_UART2);

        if (rx_buf_oper.cnt == 0) {
            if (((rx_byte >> 4) != pup_id[pup])) {
                return;
            }
        }

        rx_buf_oper.data[rx_buf_oper.cnt++] = rx_byte;

        if (rx_buf_oper.cnt == RX_OPER_SIZE) {
            rx_buf_oper.cnt = 0;
            flag_rx_ready = 1;
        }
    }
}

void UART1_IRQHandler(void)
{
    if (UART_GetFlagStatus(MDR_UART1, UART_FLAG_RXFF) == SET) {
        uint8_t rx_byte = (uint8_t)UART_ReceiveData(MDR_UART1);

        if (rx_byte == 0xEB) {
            rx_buf_kama.cnt = 0;
        }

        rx_buf_kama.data[rx_buf_kama.cnt++] = rx_byte;

        if ((rx_buf_kama.cnt == (RX_KAMA_SIZE - 1)) && rx_byte != 0x9C) {
            rx_buf_kama.cnt = 0;
        }

        if (rx_buf_kama.cnt == RX_KAMA_SIZE) {
            flag_rx_cu = 1;
        }
    }
}

// void Parallaks(void)
// {
//     last_angle = current_deg_oper;
//     next_angle_mode = last_angle;
//     // current_deg_oper = 0;
//     current_deg_oper = OAD;

//     Check_next_angle();
//     // MDR_PORTB->RXTX = 0x00;
// }

// void Check_next_angle()
// {
//     Calc_Ampl(current_deg_oper);
// }

volatile unsigned int SinOutL;
volatile unsigned int CosOutL;
volatile unsigned int SinOutH;
volatile unsigned int CosOutH;

void Timer2_IRQHandler(void)
{
    count_tim2++;
    if (Sin_delay == 0) {
        // Set low output level for DAC
        DAC1_SetData(Sin_low);
        DAC2_SetData(Cos_low);

        SinOutL = Sin_low;
        CosOutL = Cos_low;
    } else {
    }

    MDR_TIMER2->STATUS = 0;
    TIMER_Cmd(MDR_TIMER2, DISABLE);
    MDR_TIMER2->CNT = 0x00000000;
}

void Timer1_IRQHandler(void)
{
    if (TIMER_GetFlagStatus(MDR_TIMER1, TIMER_STATUS_CCR_CAP_CH3) == SET) {
        TIMER_ClearFlag(MDR_TIMER1, TIMER_STATUS_CCR_CAP_CH3);
        flag_zapit = 1;
        period = MDR_TIMER1->CNT;
        MDR_TIMER1->CNT = 0;
        Period_Massiv[counter_ext2] = period;

        period_us = period >> 1;

        if ((period_us > 2400) && (period_us < 2800)) // period < 2.2 ms - sin_signala zapitki net :)
        {
            count_error_ext = 0;
            if (NewData == 1) {
                // PORT_ResetBits(MDR_PORTB, PORT_Pin_13);
                NewData = 0;
                Sin_high = next_Sin_high;
                Cos_high = next_Cos_high;
                Sin_low = next_Sin_low;
                Cos_low = next_Cos_low;
            }

            if (Sin_delay == 0) {
                MDR_TIMER3->CNT = 0;
                MDR_TIMER3->ARR = New_offset;
                MDR_TIMER3->STATUS = 0;
                //	NVIC_EnableIRQ(Timer3_IRQn);
                TIMER_Cmd(MDR_TIMER3, ENABLE);

                if (counter_ext > 0) {
                    // MDR_TIMER2->ARR =  (period>>1)+13000 + (d_period); //half haperiod
                }
                // TIMER_Cmd(MDR_TIMER2,ENABLE);
            }
            // if (Angle_Out_Mode != None) {
            //     count_mode_angle++;
            //     if (count_mode_angle == 2) {
            //         count_mode_angle = 0;
            //         if (Angle_Out_Mode == Up) {
            //             next_angle_mode++;
            //             if (next_angle_mode == current_deg_oper) {
            //                 Angle_Out_Mode = None;
            //             } else if (next_angle_mode == 3600) {
            //                 next_angle_mode = 0;
            //             }
            //         }
            //         if (Angle_Out_Mode == Down) {
            //             next_angle_mode--;
            //             if (next_angle_mode == current_deg_oper) {
            //                 Angle_Out_Mode = None;
            //             }

            //             else if (next_angle_mode == 0) {
            //                 next_angle_mode = 3600;
            //             }
            //         }
            //         //				  n_angle_cnt2++;
            //         //				  n_angle_calc2 = next_angle_mode;
            //         Calc_Ampl(next_angle_mode);
            //     }
            // }
        } else // period < 2.2 ms - sin_signala zapitki net
        {
            count_error_ext++;
            if (count_error_ext > 7) {
                count_error_ext = 0;
                // UART_SendData (MDR_UART2, 0xCC);
                NVIC_DisableIRQ(Timer1_IRQn);
                TIMER_Cmd(MDR_TIMER1, DISABLE);
                MDR_TIMER1->CNT = 0;

                relays_off();
                led_red_only_on();

                mode = MODE_OFF;
                NVIC_DisableIRQ(Timer1_IRQn);
                //	NVIC_DisableIRQ(EXT_INT1_IRQn);
                TIMER_Cmd(MDR_TIMER3, DISABLE);
                MDR_TIMER3->CNT = 0;
                send_cmd(CMD_NO_ZAP, period_us);
                counter_ext = 0;
            } else {
                // count_error_ext = 0;
                if (NewData == 1) {
                    NewData = 0;
                    Sin_high = next_Sin_high;
                    Cos_high = next_Cos_high;
                    Sin_low = next_Sin_low;
                    Cos_low = next_Cos_low;
                }

                if (Sin_delay == 0) {
                    MDR_TIMER3->CNT = 0;
                    MDR_TIMER3->STATUS = 0;
                    //	NVIC_EnableIRQ(Timer3_IRQn);
                    TIMER_Cmd(MDR_TIMER3, ENABLE);
                    if (counter_ext > 0) {
                    }
                }
            }
        }

        if (counter_ext == 0xFFFF) {
            counter_ext = 1;
        } else {
            counter_ext++;
        }
    }
    if (counter_ext2 == 2047) {
        counter_ext2 = 1;
    } else {
        counter_ext2++;
    }
}

void Timer3_IRQHandler(void)
{
    TIMER_Cmd(MDR_TIMER3, DISABLE);
    MDR_TIMER3->CNT = 0;
    MDR_TIMER3->STATUS = 0;
    //	NVIC_DisableIRQ(Timer3_IRQn);

    // Set high output level for DAC
    DAC1_SetData(Sin_high);
    DAC2_SetData(Cos_high);

    SinOutH = Sin_high;
    CosOutH = Cos_high;

    MDR_TIMER2->ARR = d_period; // half haperiod
    TIMER_Cmd(MDR_TIMER2, ENABLE);

    count_tim3++;
}

/**
 * @brief  Reports the source file ID, the source line number
 *         and expression text (if USE_ASSERT_INFO == 2) where
 *         the assert_param error has occurred.
 * @param  file_id: pointer to the source file name
 * @param  line: assert_param error line source number
 * @param  expr:
 * @retval None
 */

#if (USE_ASSERT_INFO == 1)
void assert_failed(uint32_t file_id, uint32_t line)
{
    /* User can add his own implementation to report the source file ID and line number.
       Ex: printf("Wrong parameters value: file Id %d on line %d\r\n", file_id, line) */

    /* Infinite loop */
    while (1) {
    }
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint32_t file_id, uint32_t line, const uint8_t *expr);
{
    /* User can add his own implementation to report the source file ID, line number and
       expression text.
       Ex: printf("Wrong parameters value (%s): file Id %d on line %d\r\n", expr, file_id, line) */

    /* Infinite loop */
    while (1) {
    }
}
#endif /* USE_ASSERT_INFO */

/** @} */ /* End of group DAC_DMA_SineWave_91 */

/** @} */ /* End of group __MDR32F9Q1_EVAL */

/** @} */ /* End of group __MDR32F9Qx_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2011 Milandr *********/

/* END OF FILE main.c */
