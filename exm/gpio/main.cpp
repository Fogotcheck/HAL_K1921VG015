#include <utility>
#include <climits>
#include <random>

#include "hal_rcu.h"
#include "hal_gpio.h"
#include "arch.h"
#include "csr.h"

Hal_rcu *rcu = nullptr;

void Error_handler(void);
template <typename T, typename... Args>
void Create_obj(T *&obj, Args &&...args);

bool rcu_init(Hal_rcu *rcu);

int main()
{
    Create_obj(rcu, RCU);
    if (!rcu_init(rcu))
        Error_handler();

    rcu->instance->CGCFGAHB_bit.GPIOAEN     = 1;
    rcu->instance->RSTDISAHB_bit.GPIOAEN    = 1;
    Hal_gpio leds[] = {
        Hal_gpio(GPIOA, 12),
        Hal_gpio(GPIOA, 13),
        Hal_gpio(GPIOA, 14),
        Hal_gpio(GPIOA, 15),
    };

    leds[0].instance->OUTENSET_bit.PIN12    = 1;
    leds[1].instance->OUTENSET_bit.PIN13    = 1;
    leds[2].instance->OUTENSET_bit.PIN14    = 1;
    leds[3].instance->OUTENSET_bit.PIN15    = 1;

    leds[0].instance->DATAOUTSET_bit.PIN12  = 1;
    leds[1].instance->DATAOUTSET_bit.PIN13  = 1;
    leds[2].instance->DATAOUTSET_bit.PIN14  = 1;
    leds[3].instance->DATAOUTSET_bit.PIN15  = 1;

    for (uint8_t i = 0; i < sizeof(leds) / sizeof(leds[0]); ++i)
    {
        if (!leds[i].read())
            Error_handler();
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 3);

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
    if(!rcuGpio.enable())
        return false;

    rcuGpio.instance->ALTFUNCNUM_bit.PIN7   = 3;
    rcuGpio.instance->ALTFUNCSET_bit.PIN7   = 1;

    pRcu->instance->CLKOUTCFG = (RCU_CLKOUTCFG_CLKSEL_PLL0 << RCU_CLKOUTCFG_CLKSEL_Pos)     |
                                (1 << RCU_CLKOUTCFG_DIVN_Pos )                              |
                                (1 << RCU_CLKOUTCFG_DIVEN_Pos)                              |
                                RCU_CLKOUTCFG_RSTDIS_Msk                                    |
                                RCU_CLKOUTCFG_CLKEN_Msk;
    if (!pRcu->selectHSE())
        return false;

    pRcu->instance->PLLSYSCFG0 = (15 << RCU_PLLSYSCFG0_PD1B_Pos)                            |
                                 (3 << RCU_PLLSYSCFG0_PD1A_Pos)                             |
                                 (7 << RCU_PLLSYSCFG0_PD0B_Pos)                             |
                                 (3 << RCU_PLLSYSCFG0_PD0A_Pos)                             |
                                 (1 << RCU_PLLSYSCFG0_REFDIV_Pos)                           |
                                 (0 << RCU_PLLSYSCFG0_FOUTEN_Pos)                           |
                                 (0 << RCU_PLLSYSCFG0_DSMEN_Pos)                            |
                                 (0 << RCU_PLLSYSCFG0_DACEN_Pos)                            |
                                 (3 << RCU_PLLSYSCFG0_BYP_Pos)                              |
                                 (1 << RCU_PLLSYSCFG0_PLLEN_Pos);
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

    return pRcu->enable();
}