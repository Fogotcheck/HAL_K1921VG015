#include "hal_rcu.h"

Hal_rcu::Hal_rcu(RCU_TypeDef *rcu_base)
    : inst(rcu_base)
{
}

Hal_rcu::~Hal_rcu(void)
{
}

bool Hal_rcu::init(void)
{
    inst->SYSCLKCFG = (typeSysClk << RCU_SYSCLKCFG_SRC_Pos);

    uint32_t timeout_counter = 0;

    while ((inst->CLKSTAT_bit.SRC != inst->SYSCLKCFG_bit.SRC))
    {
        if (timeout_counter >= 1000)
            return false;
        timeout_counter++;
    }

    update();
    return true;
}

bool Hal_rcu::initMSP(Hal_gpio &gpio)
{
    auto pPort = gpio.getPort();
    if (pPort==GPIOA) {
        inst->CGCFGAHB_bit. GPIOAEN     = 1;
        inst->RSTDISAHB_bit.GPIOAEN     = 1;
    } else
    if (pPort==GPIOB) {
        inst->CGCFGAHB_bit. GPIOBEN     = 1;
        inst->RSTDISAHB_bit.GPIOBEN     = 1;
    } else
    if (pPort==GPIOC) {
        inst->CGCFGAHB_bit.GPIOCEN      = 1;
        inst->RSTDISAHB_bit.GPIOCEN     = 1;
    }else
        return false;
    
    return true;
}

bool Hal_rcu::setTypeSysClk(RCU_SYSCLKCFG_SRC_Enum type)
{
    switch (type)
    {
    case RCU_SYSCLKCFG_SRC_HSICLK:
        break;
    case RCU_SYSCLKCFG_SRC_HSECLK:
        break;
    case RCU_SYSCLKCFG_SRC_SYSPLL0CLK:
        break;
    case RCU_SYSCLKCFG_SRC_LSICLK:
        break;
    default:
        return false;
    }

    typeSysClk = type;
    return true;
}

bool Hal_rcu::selectHSE(uint32_t newValHSEClk)
{
    inst->SYSCLKCFG = (RCU_SYSCLKCFG_SRC_HSECLK << RCU_SYSCLKCFG_SRC_Pos);

    uint32_t timeout_counter = 0;
    while ((RCU->CLKSTAT_bit.SRC != RCU->SYSCLKCFG_bit.SRC))
    {
        if (timeout_counter >= 1000)
            return false;
        timeout_counter++;
    }

    valHSEClk = newValHSEClk;
    return true;
}

bool Hal_rcu::selectPllOut(void)
{
    inst->PLLSYSCFG0_bit.FOUTEN = 1;

    uint32_t timeout_counter = 0;

    while ((inst->PLLSYSSTAT_bit.LOCK) != 1)
    {
        if (timeout_counter >= 1000)
            return false;

        timeout_counter++;
    };
    inst->PLLSYSCFG0_bit.BYP = 2;

    /** @todo rm after flash */
    FLASH->CTRL_bit.LAT = 3;
    FLASH->CTRL_bit.CEN = 1;

    return true;
}

void Hal_rcu::setClkOutCFG(uint32_t ClkOutCFG)
{
    inst->CLKOUTCFG = ClkOutCFG;
}

void Hal_rcu::setPllCFG0(uint32_t PllSysCFG0)
{
    inst->PLLSYSCFG0 = PllSysCFG0;
}

void Hal_rcu::setPllCFG1(uint32_t PllSysCFG1)
{
    inst->PLLSYSCFG1 = PllSysCFG1;
}

void Hal_rcu::setPllCFG2(uint32_t PllSysCFG2)
{
    inst->PLLSYSCFG2 = PllSysCFG2;
}

void Hal_rcu::setCGCFGAHB(uint32_t CGCFGAHB)
{
    inst->CGCFGAHB = CGCFGAHB;
}

void Hal_rcu::setCGCFGAPB(uint32_t CGCFGAPB)
{
    inst->CGCFGAPB = CGCFGAPB;
}

void Hal_rcu::setSecCNT0(uint32_t SecCNT0)
{
    inst->SECCNT0 = SecCNT0;
}

void Hal_rcu::setSecCNT1(uint32_t SecCNT1)
{
    inst->SECCNT1 = SecCNT1;
}

void Hal_rcu::setRstSys(uint32_t RstSys)
{
    inst->RSTSYS = RstSys;
}

void Hal_rcu::update(void)
{
    typeSysClk   = inst->CLKSTAT_bit.SRC;
    pll_refclk   =       valHSEClk;
    pll_fbdiv    = inst->PLLSYSCFG2_bit.FBDIV;
    pll_refdiv   = inst->PLLSYSCFG0_bit.REFDIV;
    pll_pd0a     = inst->PLLSYSCFG0_bit.PD0A;
    pll_pd0b     = inst->PLLSYSCFG0_bit.PD0B;
    pll_pd1a     = inst->PLLSYSCFG0_bit.PD1A;
    pll_pd1b     = inst->PLLSYSCFG0_bit.PD1B;

    if (inst->PLLSYSCFG0_bit.DSMEN)
        pll_frac = inst->PLLSYSCFG1_bit.FRAC;
    else
        pll_frac = 0;

    Pll0Clock = (pll_refclk * (pll_fbdiv + pll_frac / (1 << 24))) / (pll_refdiv * (1 + pll_pd0a) * (1 + pll_pd0b));
    Pll1Clock = (pll_refclk * (pll_fbdiv + pll_frac / (1 << 24))) / (pll_refdiv * (1 + pll_pd1a) * (1 + pll_pd1b));
    
    switch (typeSysClk)
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

volatile RCU_TypeDef *Hal_rcu::getInst(void)
{
    return inst;
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

uint32_t Hal_rcu::getValHSEClk(void)
{
    return valHSEClk;
}

RCU_SYSCLKCFG_SRC_Enum Hal_rcu::getTypeSysClk(void)
{
    return (RCU_SYSCLKCFG_SRC_Enum)typeSysClk;
}
