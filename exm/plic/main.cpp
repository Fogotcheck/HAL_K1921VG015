#include <utility>
#include <climits>
#include <random>
#include <string.h>

#include "hal_rcu.h"
#include "hal_gpio.h"
#include "hal_uart.h"
#include "arch.h"
#include "csr.h"

Hal_rcu *rcu = nullptr;
Hal_uart *uart_0 = nullptr;

void Error_handler(void);
template <typename T, typename... Args>
void Create_obj(T *&obj, Args &&...args);
bool uart_0_init(Hal_uart *pUart);

bool rcu_init(Hal_rcu *rcu);

int main()
{
    Create_obj(rcu, RCU);
    if (!rcu_init(rcu))
        Error_handler();

    rcu->instance->CGCFGAHB_bit.GPIOAEN  = 1;
    rcu->instance->RSTDISAHB_bit.GPIOAEN = 1;
    Hal_gpio leds[] = {
        Hal_gpio(GPIOA, 12),
        Hal_gpio(GPIOA, 13),
        Hal_gpio(GPIOA, 14),
        Hal_gpio(GPIOA, 15),
    };

    leds[0].instance->OUTENSET_bit.PIN12 = 1;
    leds[1].instance->OUTENSET_bit.PIN13 = 1;
    leds[2].instance->OUTENSET_bit.PIN14 = 1;
    leds[3].instance->OUTENSET_bit.PIN15 = 1;

    Create_obj(uart_0, UART0);
    if (!uart_0_init(uart_0))
        Error_handler();

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 3);

    printf("UID[0]::0x%08X\tUID[1]::0x%08X\tUID[2]::0x%08X\tUID[3]::0x%08X\r\n",
            (unsigned int)PMUSYS->UID[0],
            (unsigned int)PMUSYS->UID[1],
            (unsigned int)PMUSYS->UID[2],
            (unsigned int)PMUSYS->UID[3]);

    while (1)
    {
        leds[dist(rng)].toggle();
        for (volatile int delayj = 0; delayj < 500000; ++delayj)
        {
        }
    }
}

void Error_handler(void)
{
    clear_csr(mie, MIE_MEXTERNAL);
    clear_csr(mstatus, MSTATUS_MIE);
    while (1)
    {
    }
}

template <typename T, typename... Args>
void Create_obj(T *&obj, Args &&...args)
{
    obj = new T(std::forward<Args>(args)...);
    if (!obj)
        Error_handler();
}

bool rcu_init(Hal_rcu *pRcu)
{
    if ((pRcu == nullptr) || (pRcu->instance == nullptr))
        return false;

    Hal_gpio rcuGpio(GPIOC, 7);
    if (!rcuGpio.enable())
        return false;

    rcuGpio.instance->ALTFUNCNUM_bit.PIN7 = 3;
    rcuGpio.instance->ALTFUNCSET_bit.PIN7 = 1;

    pRcu->instance->CLKOUTCFG = (RCU_CLKOUTCFG_CLKSEL_PLL0 << RCU_CLKOUTCFG_CLKSEL_Pos)     |
                                (1 << RCU_CLKOUTCFG_DIVN_Pos)                               |
                                (1 << RCU_CLKOUTCFG_DIVEN_Pos)                              |
                                RCU_CLKOUTCFG_RSTDIS_Msk                                    |
                                RCU_CLKOUTCFG_CLKEN_Msk;
    if (!pRcu->selectHSE())
        return false;

    pRcu->instance->PLLSYSCFG0 = (15 << RCU_PLLSYSCFG0_PD1B_Pos)                            |
                                 ( 3 << RCU_PLLSYSCFG0_PD1A_Pos)                            |
                                 ( 7 << RCU_PLLSYSCFG0_PD0B_Pos)                            |
                                 ( 3 << RCU_PLLSYSCFG0_PD0A_Pos)                            |
                                 ( 1 << RCU_PLLSYSCFG0_REFDIV_Pos)                          |
                                 ( 0 << RCU_PLLSYSCFG0_FOUTEN_Pos)                          |
                                 ( 0 << RCU_PLLSYSCFG0_DSMEN_Pos)                           |
                                 ( 0 << RCU_PLLSYSCFG0_DACEN_Pos)                           |
                                 ( 3 << RCU_PLLSYSCFG0_BYP_Pos)                             |
                                 ( 1 << RCU_PLLSYSCFG0_PLLEN_Pos);
    pRcu->instance->PLLSYSCFG1 = 0;
    pRcu->instance->PLLSYSCFG2 = 100;

    pRcu->setHSECLK_VAL(16000000);

    pRcu->instance->PLLSYSCFG0_bit.FOUTEN = 3;
    if (!pRcu->selectPLL())
        return false;

    pRcu->instance->PLLSYSCFG0_bit.BYP = 0;
    /** @todo rm after flash */
    FLASH->CTRL_bit.LAT = 3;
    FLASH->CTRL_bit.CEN = 1;

    if (!pRcu->enable())
        return false;

    return true;
}

bool uart_0_init(Hal_uart *pUart)
{
    Hal_gpio uartRX(GPIOA, 0);
    Hal_gpio uartTX(GPIOA, 1);

    if ((!uartRX.enable()) ||
        (!uartTX.enable()))
        return false;

    uartRX.instance->ALTFUNCNUM_bit.PIN0 = 1;
    uartTX.instance->ALTFUNCNUM_bit.PIN1 = 1;
    uartRX.instance->ALTFUNCSET_bit.PIN0 = 1;
    uartTX.instance->ALTFUNCSET_bit.PIN1 = 1;

    pUart->param.clkType = RCU_UARTCLKCFG_CLKSEL_PLL1;
    pUart->param.baudRate = 115200;
    pUart->param.valClkRef = rcu->getPll1Clk();

    pUart->param.RCU_DIVEN = 0;
    pUart->param.RCU_DIVN = 0;
    if (!pUart->enable())
        return false;

    pUart->instance->LCRH_bit.FEN   = 1;
    pUart->instance->LCRH_bit.WLEN  = 3;
    pUart->instance->IFLS           = 0;
    pUart->instance->CR_bit.UARTEN  = 1;
    pUart->instance->CR_bit.RXE     = 1;
    pUart->instance->CR_bit.TXE     = 1;

    pUart->setMode(Hal_uart::POLLING_MODE);
    return true;
}

#ifdef __cplusplus
extern "C"
{
#endif
    int __io_putchar(int ch)
    {
        while (UART0->FR_bit.BUSY)
        {
        };
        UART0->DR = ch;
        return ch;
    }

    int __io_getchar()
    {
        while (UART0->FR_bit.RXFE)
        {
        };
        return (int)UART0->DR_bit.DATA;
    }

#ifdef __cplusplus
}
#endif