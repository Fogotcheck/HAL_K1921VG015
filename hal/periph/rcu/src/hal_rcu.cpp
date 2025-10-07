#include "hal_rcu.h"

Hal_rcu::Hal_rcu(RCU_TypeDef *pInstance)
    : Hal_periphBase<RCU_TypeDef>(pInstance)
{
}

bool Hal_rcu::enable(void)
{
    if (instance == nullptr)
        return false;

    instance->SYSCLKCFG = (typeSysClk << RCU_SYSCLKCFG_SRC_Pos);

    uint32_t timeout_counter = 0;
    while ((instance->CLKSTAT_bit.SRC != RCU->SYSCLKCFG_bit.SRC))
    {
        timeout_counter++;
        if (timeout_counter >= 1000)
            return false;
    }

    sysClkUpdate();
    return true;
}

bool Hal_rcu::disable(void)
{
    return false;
}

bool Hal_rcu::selectHSE(void)
{
    if (instance == nullptr)
        return false;

    instance->SYSCLKCFG = (RCU_SYSCLKCFG_SRC_HSECLK << RCU_SYSCLKCFG_SRC_Pos);

    uint32_t timeout_counter = 0;
    while ((instance->CLKSTAT_bit.SRC != RCU->SYSCLKCFG_bit.SRC))
    {
        timeout_counter++;
        if (timeout_counter >= 1000)
            return false;
    }

    return true;
}

bool Hal_rcu::selectPLL(void)
{
    if (instance == nullptr)
        return false;

    uint32_t timeout_counter = 0;
    while ((instance->PLLSYSSTAT_bit.LOCK) != 1)
    {
        timeout_counter++;
        if (timeout_counter >= 1000)
            return false;
    };

    typeSysClk = RCU_SYSCLKCFG_SRC_SYSPLL0CLK;

    return true;
}

void Hal_rcu::setHSECLK_VAL(uint32_t newHSEClk)
{
    valHSEClk = newHSEClk;
}

void Hal_rcu::sysClkUpdate(void)
{
    if (instance == nullptr)
        return;

    uint32_t current_sysclk;

    current_sysclk = instance->CLKSTAT_bit.SRC;
    pll_refclk = valHSEClk;
    pll_fbdiv = instance->PLLSYSCFG2_bit.FBDIV;
    pll_refdiv = instance->PLLSYSCFG0_bit.REFDIV;
    pll_pd0a = instance->PLLSYSCFG0_bit.PD0A;
    pll_pd0b = instance->PLLSYSCFG0_bit.PD0B;
    pll_pd1a = instance->PLLSYSCFG0_bit.PD1A;
    pll_pd1b = instance->PLLSYSCFG0_bit.PD1B;
    if (instance->PLLSYSCFG0_bit.DSMEN)
        pll_frac = instance->PLLSYSCFG1_bit.FRAC;
    else
        pll_frac = 0;

    Pll0Clock = (pll_refclk * (pll_fbdiv + pll_frac / (1 << 24))) / (pll_refdiv * (1 + pll_pd0a) * (1 + pll_pd0b));
    Pll1Clock = (pll_refclk * (pll_fbdiv + pll_frac / (1 << 24))) / (pll_refdiv * (1 + pll_pd1a) * (1 + pll_pd1b));
    switch (current_sysclk)
    {
    case RCU_CLKSTAT_SRC_HSICLK:
        coreClock = HSICLK_VAL;
        break;
    case RCU_CLKSTAT_SRC_HSECLK:
        coreClock = valHSEClk;
        break;
    case RCU_CLKSTAT_SRC_SYSPLL0CLK:
        coreClock = Pll0Clock;
        break;
    case RCU_CLKSTAT_SRC_LSICLK:
        coreClock = LSICLK_VAL;
        break;
    }
}

uint32_t Hal_rcu::getCoreClk(void)
{
    return coreClock;
}

uint32_t Hal_rcu::getPll0Clk(void)
{
    return Pll0Clock;
}

uint32_t Hal_rcu::getPll1Clk(void)
{
    return Pll1Clock;
}