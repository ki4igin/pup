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
#include "ParalaxCalc_fixpt.h"
#include "leds.h"
#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "dac.h"
#include "timer.h"
#include "sin_signal.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

// Zero_Ref - shift bitwin zeros VT-sensor and drive-sensor
#define Sensor_Shift 450

int16_t Zero_Ref = Sensor_Shift;

PORT_InitTypeDef PORT_InitStructure;
TIMER_CntInitTypeDef sTIM_CntInit;

UART_InitTypeDef UART_InitStructure;

uint8_t ReciveByte = 0x00;

uint32_t Period_Massiv[2048]; // for debug

/* Variables for DAC ---------------------------------------------------------*/
int16_t Sin_high = 0; /// angle = 91,3 grad
int16_t Sin_low = 4095;
int16_t Cos_high = 71;
int16_t Cos_low = 0;

volatile int16_t next_angle = 460; //// ???? =913  ???
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
uint32_t period_mcs = 0;

/* Variables for UART ---------------------------------------------------------*/
#define RX_BUFFER_SIZE 6
uint8_t RxMassiv[RX_BUFFER_SIZE];
volatile uint8_t RxCnt = 0;
uint8_t flag_rx_ready = 0;
uint8_t flag_tx_ready = 0;
uint8_t flag_rx_cu = 0;
uint8_t ParseEnable = 0;
uint8_t RxEnable = 0;
uint16_t Sum = 0;

uint8_t flag_zapit = 0;
uint8_t flag_not_zapit = 0;
uint8_t cnt_systick = 0;

uint16_t New_offset = 0;

volatile uint8_t RxCnt1 = 0;

uint8_t RxEnable1 = 0;
uint8_t RxMassiv1[26];

uint8_t ReciveByte1 = 0x00;

uint8_t TxMassivSingle[6] = {0x11, 0x00, 0x00, 0x00, 0x00, 0x11};
uint8_t TxMassivComplex[6] = {0x11, 0x00, 0x00, 0x00, 0x01, 0x12};
uint8_t TxMassivComplexKama[6] = {0x11, 0x00, 0x00, 0x00, 0x02, 0x13};
uint8_t TxMassivOffset[6] = {0x13, 0x00, 0x00, 0x30, 0xD4, 0x17};

uint8_t TxMassivNoZap[6] = {0x14, 0x00, 0x00, 0x00, 0x01, 0x15};
uint8_t TxMassivSmallZap[6] = {0x15, 0x00, 0x00, 0x00, 0x00, 0x15};

uint8_t i;

/* Variables for Parallaks ---------------------------------------------------------*/

uint8_t Angle_In_Mode = 0; // if = 1 - Parallaks Mode
uint32_t b_first = 31;     // = 37m
uint32_t um_otn = 10;      // = 3grad
uint32_t az_otn = 1600;    // = 70grad

uint32_t R_first = 0;  // = 3km
uint32_t um_first = 0; // = 40grad
uint32_t az_first = 0; // = 15grad

uint32_t dif = 0;

uint32_t OD = 0;
uint32_t AD2 = 0;
uint32_t AD2_2 = 0;
uint32_t AO = 0;
uint32_t AD = 0;

uint32_t CD = 0;

uint32_t CAD = 0;
uint32_t OAD = 0;

uint32_t AC2 = 0;
uint32_t AC = 0;

uint32_t sin_CAD = 0;
uint32_t sin_OAD = 0;
uint8_t flag_equally = 0;
uint16_t index1 = 0;
uint16_t plus = 0;

uint32_t S = 0;
uint32_t a = 0;
uint32_t b = 0;
uint32_t e = 0;

/*--------------------------------------------------------------*/

typedef enum {
    None = 0x00,
    Up = 0x01,
    Down = 0x10
} Angle_Out_Mode_TypeDef;

#define IS_Angle_Out_Mode(Angle_Out_Mode) (((Angle_Out_Mode) == None) || ((Angle_Out_Mode) == Up) || ((Angle_Out_Mode) == Down))

Angle_Out_Mode_TypeDef Angle_Out_Mode = None;

typedef enum {
    Single = 0,
    Complex = 1,
    Complex_Kama = 2
} ModeState;

#define IS_MODE_STATE(STATE) (((STATE) == Single) || ((STATE) == Complex) || ((STATE) == Complex_Kama))

uint8_t Pre_Complex = 0;
uint8_t Pre_Complex_Kama = 0;
ModeState Mode = Single;

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

void Calc_Ampl(uint16_t next_angle);
void Timer1_Init(void);
void Timer2_Init(void);
void Timer3_Init(void);
void CheckSumPacket(uint8_t count_massiv, uint8_t Massiv[20]);
void Parallaks(void);
void Check_next_angle(void);

static uint32_t cnt_rx_kama = 0;

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

    timer1_init();
    timer2_init();
    timer3_init();
    New_offset = MDR_TIMER3->ARR;

    uart1_init();
    uart2_init();

    dac_all_init();

    /* Init Angle as 46 grad */ //// ??????????????
    Sin_high = 0;
    Sin_low = sin_signal[890];
    Cos_high = sin_signal[10];
    Cos_low = 0;

    /* Set PIN SHDN of RS232 */
    PORT_SetBits(MDR_PORTB, PORT_Pin_11);

    /* Init System Timer */
    SysTick->LOAD = 0xFFFFFF; // 0.2s
    SysTick->CTRL = 0x7;

    static int32_t OAD_TEMP;

    // ParalaxCalc_fixpt_initialize();
    ParalaksInit();
    PORT_SetBits(MDR_PORTB, PORT_Pin_12);
    while (1) {
        if (flag_not_zapit == 1) {
            flag_not_zapit = 0;
            flag_zapit = 0;
            cnt_systick = 0;
            PORT_ResetBits(MDR_PORTC, PORT_Pin_4);
            PORT_ResetBits(MDR_PORTC, PORT_Pin_5);
            PORT_ResetBits(MDR_PORTC, PORT_Pin_6);
            PORT_SetBits(MDR_PORTB, PORT_Pin_12);
            PORT_ResetBits(MDR_PORTB, PORT_Pin_13);
            PORT_ResetBits(MDR_PORTB, PORT_Pin_14);

            counter_ext = 0;
            counter_ext2 = 0;
            Mode = Single;
            NVIC_DisableIRQ(Timer1_IRQn); //
            TIMER_Cmd(MDR_TIMER3, DISABLE);
            MDR_TIMER3->CNT = 0;
            TIMER_Cmd(MDR_TIMER1, DISABLE);
            MDR_TIMER1->CNT = 0;

            i = 0;
            while (i < 6) {
                while (UART_GetFlagStatus(MDR_UART2, UART_FLAG_TXFE) == RESET) {
                }
                UART_SendData(MDR_UART2, TxMassivNoZap[i]);
                i++;
            }
        }

        if (flag_rx_cu == 1) // if recieve celeukazaniya
        {
            /////

            // parallaks

            // PORT_SetBits(MDR_PORTB, PORT_Pin_15);

            CheckSumPacket(26, RxMassiv1);
            flag_rx_cu = 0;
            if (ParseEnable == 1) {
                ParseEnable = 0;

                az_first = (RxMassiv1[13] >> 2);
                az_first = az_first + (RxMassiv1[12] << 5);
                az_first = az_first + ((RxMassiv1[11] & 0x07) << 12); // = 15grad
                //////////////////////////////////
                //			az_first = (az_first*225)>>11;
                ///////////////////////////////////

                if ((RxMassiv1[18] & 0x40) == 0) {
                    um_first = (RxMassiv1[20] >> 2) + (RxMassiv1[19] << 5) + ((RxMassiv1[18] & 0x0F) << 12); // = 40grad
                    //////////////////////////////////
                    //		um_first = (um_first*225)>>11;
                    ///////////////////////////////////
                } else {
                    um_first = (RxMassiv1[20] >> 2) + (RxMassiv1[19] << 5) + ((RxMassiv1[18] & 0x0F) << 12); // = 40grad
                    //////////////////////////////////
                    //			um_first = (um_first*225)>>11;
                    ///////////////////////////////////

                    um_first = 0 - um_first;
                }
                R_first = (RxMassiv1[17]) + (RxMassiv1[16] << 7) + (RxMassiv1[15] << 14) + (RxMassiv1[14] >> 21); // = 3km

                N_Coord_Kama.az = az_first;
                N_Coord_Kama.el = um_first;
                N_Coord_Kama.r = R_first;

                // фильтрация данных камы
                if (isvalid_kama_data(N_Coord_Kama) == 0) {
                    continue;
                }

                ParalaksCalc(&N_Coord_Kama, &N_Coord_MRL);
                //				az_first = (RxMassiv1[1]<<8) + RxMassiv1[2]; // = 15grad
                //				um_first = (RxMassiv1[3]<<8) + RxMassiv1[4]; // = 40grad
                //				R_first = (RxMassiv1[5]<<24) + (RxMassiv1[6]<<16) + (RxMassiv1[7]<<8) + RxMassiv1[8]; // = 3km

                //				CBE = N_Coord_MRL.el;
                OAD = N_Coord_MRL.az;
                //				BC = N_Coord_MRL.r;

                // need translate to degrees & meters

                // WTF ???????????
                // OAD_TEMP = ((OAD*3600) >> 15) - 75;
                // WTF ???????????
                OAD_TEMP = ((OAD * 3600) >> 15);
                if (OAD_TEMP < 0) {
                    OAD_TEMP += 3600;
                }
                OAD = OAD_TEMP;
                //				((OAD*3600) >> 15) - 75;

                //				CBE = (CBE*3600) >> 15;
                Parallaks();
            }
            /////
        } else {
            if (flag_rx_ready == 1) {
                flag_rx_ready = 0;

                CheckSumPacket(6, RxMassiv);
                if (ParseEnable == 1) {
                    ParseEnable = 0;
                    if (RxMassiv[0] == 0x11) {
                        switch (RxMassiv[4]) {
                        case 0x00: // relay switch off
                            PORT_ResetBits(MDR_PORTC, PORT_Pin_4);
                            PORT_ResetBits(MDR_PORTC, PORT_Pin_5);
                            PORT_ResetBits(MDR_PORTC, PORT_Pin_6);
                            PORT_SetBits(MDR_PORTB, PORT_Pin_12);
                            PORT_ResetBits(MDR_PORTB, PORT_Pin_13);
                            PORT_ResetBits(MDR_PORTB, PORT_Pin_14);
                            counter_ext = 0;
                            counter_ext2 = 0;
                            Mode = Single;
                            NVIC_DisableIRQ(Timer1_IRQn); // enable interrupt Timer1
                            TIMER_Cmd(MDR_TIMER1, DISABLE);
                            MDR_TIMER1->CNT = 0;
                            TIMER_Cmd(MDR_TIMER3, DISABLE);
                            MDR_TIMER3->CNT = 0;
                            NVIC_DisableIRQ(UART1_IRQn);
                            break;
                        case 0x01: // angle from operdtor
                            flag_not_zapit = 0;
                            flag_zapit = 0;
                            cnt_systick = 0;
                            PORT_SetBits(MDR_PORTC, PORT_Pin_4);
                            PORT_SetBits(MDR_PORTC, PORT_Pin_5);
                            PORT_SetBits(MDR_PORTC, PORT_Pin_6);
                            PORT_SetBits(MDR_PORTB, PORT_Pin_13);
                            PORT_ResetBits(MDR_PORTB, PORT_Pin_12);
                            PORT_ResetBits(MDR_PORTB, PORT_Pin_14);

                            // Mode = Complex;
                            if (Mode == Single) {
                                Pre_Complex = 1;
                            } else {
                                Mode = Complex;
                            }
                            counter_ext = 0;
                            count_error_ext = 0;
                            counter_ext2 = 0;
                            NVIC_DisableIRQ(UART1_IRQn);
                            break;

                        case 0x02: // angle from KAMA
                            flag_not_zapit = 0;
                            flag_zapit = 0;
                            cnt_systick = 0;
                            PORT_SetBits(MDR_PORTC, PORT_Pin_4);
                            PORT_SetBits(MDR_PORTC, PORT_Pin_5);
                            PORT_SetBits(MDR_PORTC, PORT_Pin_6);

                            PORT_SetBits(MDR_PORTB, PORT_Pin_14);
                            PORT_ResetBits(MDR_PORTB, PORT_Pin_13);
                            PORT_ResetBits(MDR_PORTB, PORT_Pin_12);

                            if (Mode == Single) {
                                Pre_Complex_Kama = 1;
                            } else {
                                Mode = Complex_Kama;
                            }
                            // Mode = Complex_Kama;
                            counter_ext = 0;
                            count_error_ext = 0;
                            counter_ext2 = 0;

                            Angle_In_Mode = 1; // Parallacs mode
                            cnt_rx_kama = 0;
                            NVIC_EnableIRQ(UART1_IRQn);

                            break;
                        }
                    } else if (RxMassiv[0] == 0x12) {
                        /////////////////////////////////////////////////////////////

                        if (Mode == Complex_Kama) {
                        } else {
                            // ParseEnable = 0;
                            last_angle = next_angle;
                            next_angle_mode = last_angle;
                            // next_angle = 0;
                            next_angle = (RxMassiv[3] << 8) + RxMassiv[4];
                            //						n_angle = next_angle;

                            Check_next_angle();
                        }
                    } else if (RxMassiv[0] == 0x13) {
                        //	flag_offset = 1 ;
                        New_offset = (RxMassiv[3] << 8) + RxMassiv[4] + 1;
                        TxMassivOffset[3] = RxMassiv[3];
                        TxMassivOffset[4] = RxMassiv[4];
                        TxMassivOffset[5] = TxMassivOffset[4] + TxMassivOffset[3] + TxMassivOffset[0];
                    } else if (RxMassiv[0] == 0x16) {
                        Zero_Ref = Sensor_Shift + (RxMassiv[1] << 8) + RxMassiv[2];
                        if (Zero_Ref > 3599) {
                            Zero_Ref = Zero_Ref - 3600;
                        }
                        if (Zero_Ref < 0) {
                            Zero_Ref = Zero_Ref + 3600;
                        }
                        Check_next_angle();
                    } else if (RxMassiv[0] == 0x8B) {
                        d_period = (RxMassiv[1] << 8) + RxMassiv[2];
                    }

                    //					else if ( RxMassiv[0] == 0x88) // Change mode to Parallaks
                    //					{
                    //						if (RxMassiv[3] == 1)
                    //						{
                    //							Angle_In_Mode = 1;
                    //							NVIC_EnableIRQ(UART1_IRQn);
                    //						}
                    //						else
                    //						{
                    //							Angle_In_Mode = 0;
                    //							NVIC_DisableIRQ(UART1_IRQn);
                    //						}
                    //
                    //					}
                }
            }
            if (flag_tx_ready == 1) {
                flag_tx_ready = 0;
                count_tx++;
                if (count_tx == 1) {
                    i = 0;
                    while (i < 6) {
                        while (UART_GetFlagStatus(MDR_UART2, UART_FLAG_TXFE) == RESET) {
                        }
                        UART_SendData(MDR_UART2, TxMassivOffset[i]);
                        i++;
                    }
                } else {
                    count_tx = 0;
                    if (Mode == Single) {
                        i = 0;
                        while (i < 6) {
                            while (UART_GetFlagStatus(MDR_UART2, UART_FLAG_TXFE) == RESET) {
                            }
                            UART_SendData(MDR_UART2, TxMassivSingle[i]);
                            i++;
                        }
                    } else if (Mode == Complex) {
                        i = 0;
                        while (i < 6) {
                            while (UART_GetFlagStatus(MDR_UART2, UART_FLAG_TXFE) == RESET) {
                            }
                            UART_SendData(MDR_UART2, TxMassivComplex[i]);
                            i++;
                        }
                    } else {
                        i = 0;
                        while (i < 6) {
                            while (UART_GetFlagStatus(MDR_UART2, UART_FLAG_TXFE) == RESET) {
                            }
                            UART_SendData(MDR_UART2, TxMassivComplexKama[i]);
                            i++;
                        }
                    }
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
            Mode = Complex;
            MDR_TIMER1->CNT = 0;
            TIMER_Cmd(MDR_TIMER1, ENABLE);
            NVIC_EnableIRQ(Timer1_IRQn); // enable interrupt Timer1
        }
    }
    if (Mode == Complex) {
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
            Mode = Complex_Kama;
            MDR_TIMER1->CNT = 0;
            TIMER_Cmd(MDR_TIMER1, ENABLE);
            NVIC_EnableIRQ(Timer1_IRQn); // enable interrupt Timer1
        }
    }
    if (Mode == Complex_Kama) {
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

/*
volatile uint16_t n_angle_calc;
volatile uint16_t n_angle_calc2;
volatile uint16_t n_angle_calc3;
*/
volatile int z;

void Calc_Ampl(uint16_t next_angle_calc)
{
    //	uint8_t count_error = 0;
    // count_error = next_angle_calc/ ;
    // next_angle_calc = next_angle_calc + system_error[]; ///////////// remove system error
    //	n_angle_cnt3++;
    //	n_angle_calc3 = next_angle_calc;
    //	next_angle_calc = next_angle_calc ;//+ 7; ///////////// remove system error
    if ((next_angle == 0) || (next_angle_calc == 1) || (next_angle_calc == 0)) {
        z++;
    }

    if (next_angle_calc >= (3600 - Zero_Ref)) {
        next_angle2 = 3600 - next_angle_calc;
        next_angle2 = Zero_Ref - next_angle2;
        next_Sin_high = 0;
        next_Cos_high = 0;
        next_Sin_low = sin_signal[next_angle2];
        next_Cos_low = sin_signal[900 - next_angle2];
    } else {
        next_angle2 = next_angle_calc + Zero_Ref;
        if (next_angle2 <= 900) {
            next_Sin_high = 0;
            next_Cos_high = 0;
            next_Sin_low = sin_signal[next_angle2];
            next_Cos_low = sin_signal[900 - next_angle2];
        } else if (next_angle2 <= 1800) {
            next_Sin_high = 0;
            next_Cos_high = sin_signal[next_angle2 - 900];
            next_Sin_low = sin_signal[1800 - next_angle2];
            next_Cos_low = 0;
        } else if (next_angle2 <= 2700) {
            next_Sin_high = sin_signal[next_angle2 - 1800];
            next_Cos_high = sin_signal[2700 - next_angle2];
            next_Sin_low = 0;
            next_Cos_low = 0;
        } else {
            next_Sin_high = sin_signal[3600 - next_angle2];
            next_Cos_high = 0;
            next_Sin_low = 0;
            next_Cos_low = sin_signal[next_angle2 - 2700];
        }
    }
    NewData = 1;
    //	last_need_angle = new_need_angle;
    //	new_need_angle = next_Sin_low;
}

void UART2_IRQHandler(void)
{
    if (UART_GetFlagStatus(MDR_UART2, UART_FLAG_RXFF) == SET) {
        ReciveByte = (uint8_t)UART_ReceiveData(MDR_UART2);
        if (RxEnable == 0) {
            if (ReciveByte == 0x88 | ReciveByte == 0x11 | ReciveByte == 0x12 | ReciveByte == 0x8B | ReciveByte == 0x13 | ReciveByte == 0x16) {
                RxEnable = 1;
                RxCnt = 0;
                RxMassiv[RxCnt] = ReciveByte;
                RxCnt++;
            }
        } else {
            RxMassiv[RxCnt] = ReciveByte;
            RxCnt++;
            if (RxCnt == 6) {
                RxEnable = 0;
                flag_rx_ready = 1;
                //				if (RxMassiv[0] == 0x12)
                //				{
                //					PORT_SetBits(MDR_PORTB, PORT_Pin_13);
                //					//flag_new_next = 1;
                //				}
            }
        }
    }
}

void UART1_IRQHandler(void)
{
    if (UART_GetFlagStatus(MDR_UART1, UART_FLAG_RXFF) == SET) {
        ReciveByte1 = (uint8_t)UART_ReceiveData(MDR_UART1);
        if (RxEnable1 == 0) {
            if (ReciveByte1 == 0xEB) {
                RxEnable1 = 1;
                RxCnt1 = 0;
                RxMassiv1[RxCnt1] = ReciveByte1;
                RxCnt1++;
            }
        } else {
            if (RxCnt1 == 25) {
                RxMassiv1[RxCnt1] = ReciveByte1;
                RxCnt1++;
                flag_rx_cu = 1;

                RxEnable1 = 0;
            } else {
                if ((ReciveByte1 & 0x80) == 0) {
                    RxMassiv1[RxCnt1] = ReciveByte1;
                    RxCnt1++;
                } else {
                    RxMassiv1[RxCnt1] = ReciveByte1;
                    RxCnt1++;
                    if (RxCnt1 == 24) {
                        if (RxMassiv1[24] != 0x9C) {
                            RxEnable1 = 0;
                        }
                    }
                    //				RxEnable1 = 0;
                }
            }
        }
    }
}

void Parallaks(void)
{
    last_angle = next_angle;
    next_angle_mode = last_angle;
    // next_angle = 0;
    next_angle = OAD;

    Check_next_angle();
    // MDR_PORTB->RXTX = 0x00;
}

void Check_next_angle()
{
    Calc_Ampl(next_angle);
}

void CheckSumPacket(uint8_t count_massiv, uint8_t Massiv[20])
{
    Sum = 0;
    uint8_t i = 0;
    count_massiv = count_massiv - 1;
    while (i < count_massiv) {
        Sum += Massiv[i];
        i++;
    }
    // Sum = Sum + 0xEB;
    if (flag_rx_cu == 1) {
        while (Sum > 0xFF) {
            Sum = ((Sum & 0xFF00) >> 8) + (Sum & 0x00FF);
        }
        // Sum = Sum & 0x7F;
    }

    if ((Sum & 0x00FF) == Massiv[count_massiv]) {
        ParseEnable = 1;
    }
    //	ParseEnable = 1;
}

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
        //			{
        //					DAC2_SetData (Cos_low);

        //				//	MDR_TIMER3->ARR = Sin_delay<<4; //half haperiod
        //				  TIMER_Cmd(MDR_TIMER3,ENABLE);
        //
        //				//	MDR_TIMER2->ARR =  (period>>1)+13000 + (d_period); //half haperiod
        //				 // TIMER_Cmd(MDR_TIMER2,ENABLE);
        //			}
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

        period_mcs = period >> 1;

        if ((period_mcs > 2400) && (period_mcs < 2800)) // period < 2.2 ms - sin_signala zapitki net :)
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
            if (Angle_Out_Mode != None) {
                count_mode_angle++;
                if (count_mode_angle == 2) {
                    count_mode_angle = 0;
                    if (Angle_Out_Mode == Up) {
                        next_angle_mode++;
                        if (next_angle_mode == next_angle) {
                            Angle_Out_Mode = None;
                        } else if (next_angle_mode == 3600) {
                            next_angle_mode = 0;
                        }
                    }
                    if (Angle_Out_Mode == Down) {
                        next_angle_mode--;
                        if (next_angle_mode == next_angle) {
                            Angle_Out_Mode = None;
                        }

                        else if (next_angle_mode == 0) {
                            next_angle_mode = 3600;
                        }
                    }
                    //				  n_angle_cnt2++;
                    //				  n_angle_calc2 = next_angle_mode;
                    Calc_Ampl(next_angle_mode);
                }
            }
        } else // period < 2.2 ms - sin_signala zapitki net
        {
            count_error_ext++;
            if (count_error_ext > 7) {
                count_error_ext = 0;
                // UART_SendData (MDR_UART2, 0xCC);
                NVIC_DisableIRQ(Timer1_IRQn);
                TIMER_Cmd(MDR_TIMER1, DISABLE);
                MDR_TIMER1->CNT = 0;

                PORT_ResetBits(MDR_PORTC, PORT_Pin_4);
                PORT_ResetBits(MDR_PORTC, PORT_Pin_5);
                PORT_ResetBits(MDR_PORTC, PORT_Pin_6);

                PORT_SetBits(MDR_PORTB, PORT_Pin_12);
                PORT_ResetBits(MDR_PORTB, PORT_Pin_13);
                PORT_ResetBits(MDR_PORTB, PORT_Pin_14);

                Mode = Single;
                NVIC_DisableIRQ(Timer1_IRQn);
                //	NVIC_DisableIRQ(EXT_INT1_IRQn);
                TIMER_Cmd(MDR_TIMER3, DISABLE);
                MDR_TIMER3->CNT = 0;

                TxMassivSmallZap[2] = counter_ext;
                TxMassivSmallZap[1] = (counter_ext >> 8);
                // period_mcs = (period>>1);
                TxMassivSmallZap[4] = period_mcs;
                TxMassivSmallZap[3] = (period_mcs >> 8);
                TxMassivSmallZap[5] = TxMassivSmallZap[0] + TxMassivSmallZap[2] + TxMassivSmallZap[1] + TxMassivSmallZap[3] + TxMassivSmallZap[4];

                i = 0;
                while (i < 6) {
                    while (UART_GetFlagStatus(MDR_UART2, UART_FLAG_TXFE) == RESET) {
                    }
                    UART_SendData(MDR_UART2, TxMassivSmallZap[i]);
                    i++;
                }
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

                    //		MDR_TIMER1->CNT=0;
                    //		MDR_TIMER1->STATUS=0;
                    //		TIMER_Cmd(MDR_TIMER1,ENABLE);//Count of enable ext_int
                    ////
                    ////					  DAC1_SetData (Sin_high);
                    ////		  DAC2_SetData (Cos_high);
                    //
                    if (counter_ext > 0) {
                        // MDR_TIMER2->ARR =  (period>>1)+13000 + (d_period); //half haperiod
                    }
                    // TIMER_Cmd(MDR_TIMER2,ENABLE);
                }

                //////////////////////////////
                // Angle_Out_Mode = 1
                if (Angle_Out_Mode != None) {
                    count_mode_angle++;
                    if (count_mode_angle == 2) {
                        count_mode_angle = 0;
                        if (Angle_Out_Mode == Up) {
                            next_angle_mode++;
                            if (next_angle_mode == next_angle) {
                                Angle_Out_Mode = None;
                            } else if (next_angle_mode == 3600) {
                                next_angle_mode = 0;
                            }
                        }
                        if (Angle_Out_Mode == Down) {
                            next_angle_mode--;
                            if (next_angle_mode == next_angle) {
                                Angle_Out_Mode = None;
                            } else if (next_angle_mode == 0) {
                                next_angle_mode = 3600;
                            }
                        }

                        Calc_Ampl(next_angle_mode);
                    }
                }
            }
        }

        if (counter_ext == 0xFFFF) {
            counter_ext = 1;
        } else {
            counter_ext++;
        }
        // cnt11++;

        //	temp_az=1;

        //	NVIC_DisableIRQ(Timer1_IRQn);
        //	MDR_TIMER3->STATUS=0;
        //	MDR_TIMER3->CH1_CNTRL = 0x8000; //enable Chanel1 in capture mode
        // MDR_TIMER3->IE = 0x00000020;
        //	MDR_TIMER3->STATUS=0;
        //	NVIC_EnableIRQ(Timer3_IRQn);// enable interrupt from imp sdviga
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
