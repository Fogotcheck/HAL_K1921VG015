#include <utility>

#include "hal_rcu.h"
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

    volatile int temp = 0;

    while (1)
    {
        temp++;
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

    pRcu->instance->CGCFGAHB_bit.GPIOCEN    = 1;
    pRcu->instance->RSTDISAHB_bit.GPIOCEN   = 1;
    /** @todo rm after gpio */
    GPIOC->ALTFUNCNUM_bit.PIN7  = 3;
    GPIOC->ALTFUNCSET_bit.PIN7  = 1;
    pRcu->instance->CLKOUTCFG   = (RCU_CLKOUTCFG_CLKSEL_PLL0 << RCU_CLKOUTCFG_CLKSEL_Pos)   |
                                (1 << RCU_CLKOUTCFG_DIVN_Pos)                               |
                                (1 << RCU_CLKOUTCFG_DIVEN_Pos)                              |
                                RCU_CLKOUTCFG_RSTDIS_Msk                                    | 
                                RCU_CLKOUTCFG_CLKEN_Msk;
    if (!pRcu->selectHSE())
        return false;

    pRcu->instance->PLLSYSCFG0 = (15 <<  RCU_PLLSYSCFG0_PD1B_Pos)                           |
                                 (3 <<   RCU_PLLSYSCFG0_PD1A_Pos)                           |
                                 (7 <<   RCU_PLLSYSCFG0_PD0B_Pos)                           |
                                 (3 <<   RCU_PLLSYSCFG0_PD0A_Pos)                           |
                                 (1 <<   RCU_PLLSYSCFG0_REFDIV_Pos)                         |
                                 (0 <<   RCU_PLLSYSCFG0_FOUTEN_Pos)                         |
                                 (0 <<   RCU_PLLSYSCFG0_DSMEN_Pos)                          |
                                 (0 <<   RCU_PLLSYSCFG0_DACEN_Pos)                          |
                                 (3 <<   RCU_PLLSYSCFG0_BYP_Pos)                            |
                                 (1 <<   RCU_PLLSYSCFG0_PLLEN_Pos);                      
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