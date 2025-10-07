#ifndef __hal_rcu_h__
#define __hal_rcu_h__

#include "K1921VG015.h"
#include "hal_periphBase.h"

#define HSICLK_VAL 1000000
#define LSICLK_VAL 32768

class Hal_rcu : public Hal_periphBase<RCU_TypeDef>
{
private:
    uint32_t coreClock      = 0;
    uint32_t Pll0Clock      = 0;
    uint32_t Pll1Clock      = 0;
    uint32_t valHSEClk      = 0;
    uint32_t typeSysClk     = 0;

    uint32_t pll_refclk     = 0;
    uint32_t pll_refdiv     = 0;
    uint32_t pll_frac       = 0;
    uint32_t pll_fbdiv      = 0;
    uint32_t pll_pd0a       = 0;
    uint32_t pll_pd0b       = 0;
    uint32_t pll_pd1a       = 0;
    uint32_t pll_pd1b       = 0;

public:
    explicit Hal_rcu(RCU_TypeDef *pInstance);
    ~Hal_rcu() override = default;

    bool selectHSE      (void);
    bool selectPLL     (void);
    void sysClkUpdate   (void);
    void setHSECLK_VAL  (uint32_t newHSEClk);

    uint32_t getCoreClk (void);
    uint32_t getPll0Clk (void);
    uint32_t getPll1Clk (void);

    bool enable         (void) override;
    bool disable        (void) override;
};

#endif //__hal_rcu_h__
