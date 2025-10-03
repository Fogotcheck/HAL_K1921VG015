#include <stdio.h>
#include <stdlib.h>

#include <utility>
#include <climits>
#include <random>
#include "arch.h"
#include "csr.h"

#include "hal_rcu.h"
#include "hal_gpio.h"

Hal_rcu *rcu = nullptr;

void Error_handler(void);
template <typename T, typename... Args>
void Create_obj(T *&obj, Args &&...args);

int main()
{
    Create_obj(rcu, RCU);

    Hal_gpio rcuGpio(GPIOC, 7);
    if(!rcu->initMSP(rcuGpio))
        Error_handler();
    rcuGpio.config.altFuncNumSet.PIN7   = 3;
    rcuGpio.config.altFuncSet.PIN7      = 1;
    rcuGpio.configure(rcuGpio.config.altFuncNumSet);
    rcuGpio.configure(rcuGpio.config.altFuncSet);

    rcu->setClkOutCFG(  (RCU_CLKOUTCFG_CLKSEL_PLL0 << RCU_CLKOUTCFG_CLKSEL_Pos) |
                        (1 << RCU_CLKOUTCFG_DIVN_Pos                          ) |
                        (1 << RCU_CLKOUTCFG_DIVEN_Pos                         ) |
                        RCU_CLKOUTCFG_RSTDIS_Msk                                |
                        RCU_CLKOUTCFG_CLKEN_Msk);
    
    if (!rcu->selectHSE(16000000))
        Error_handler();

    rcu->setPllCFG0(    (15 << RCU_PLLSYSCFG0_PD1B_Pos      )                   |
                        (3  << RCU_PLLSYSCFG0_PD1A_Pos      )                   |
                        (7  << RCU_PLLSYSCFG0_PD0B_Pos      )                   |
                        (3  << RCU_PLLSYSCFG0_PD0A_Pos      )                   |
                        (1  << RCU_PLLSYSCFG0_REFDIV_Pos    )                   |
                        (0  << RCU_PLLSYSCFG0_FOUTEN_Pos    )                   |
                        (0  << RCU_PLLSYSCFG0_DSMEN_Pos     )                   |
                        (0  << RCU_PLLSYSCFG0_DACEN_Pos     )                   |
                        (3  << RCU_PLLSYSCFG0_BYP_Pos       )                   |
                        (1  << RCU_PLLSYSCFG0_PLLEN_Pos     ));
    rcu->setPllCFG1(0);
    rcu->setPllCFG2(100);
    if (!rcu->selectPllOut())
        Error_handler();
    
    if (!rcu->setTypeSysClk(RCU_SYSCLKCFG_SRC_SYSPLL0CLK))
        Error_handler();
    
    if (!rcu->init())
        Error_handler();

    Hal_gpio leds[] = {
        Hal_gpio(GPIOA, 12),
        Hal_gpio(GPIOA, 13),
        Hal_gpio(GPIOA, 14),
        Hal_gpio(GPIOA, 15),
    };

    if(! rcu->initMSP(leds[0]))
        Error_handler();

    leds[0].config.outEnSet.PIN12    =1;
    leds[0].config.stOutSet.PIN12    =1;
    leds[1].config.outEnSet.PIN13    =1;
    leds[1].config.stOutSet.PIN13    =1;
    leds[2].config.outEnSet.PIN14    =1;
    leds[2].config.stOutSet.PIN14    =1;
    leds[3].config.outEnSet.PIN15    =1;
    leds[3].config.stOutSet.PIN15    =1;

    for (uint8_t i = 0; i < sizeof(leds) / sizeof(leds[0]); ++i)
    {
        leds[i].configure();

        if (!leds[i].read())
            Error_handler();
    }

    std::random_device rd;
    std::mt19937 rng(rd());
    std::uniform_int_distribution<uint8_t> dist(0, 3);
    
    while (1)
    {   
        leds[dist(rng)].toggle();
        for (volatile int delayj = 0; delayj < 500000; ++delayj) { }
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