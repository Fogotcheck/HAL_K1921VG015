#ifndef __hal_rcu_h__
#define __hal_rcu_h__

#include "hal_rcu_def.h"
#include "hal_gpio.h"

class Hal_rcu
{
private:
    volatile RCU_TypeDef *inst = nullptr;

    uint32_t coreClock   = 0;
    uint32_t Pll0Clock   = 0;
    uint32_t Pll1Clock   = 0;
    uint32_t valHSEClk   = 0;
    uint32_t typeSysClk  = 0;

    uint32_t pll_refclk  = 0;
    uint32_t pll_refdiv  = 0;
    uint32_t pll_frac    = 0;
    uint32_t pll_fbdiv   = 0;
    uint32_t pll_pd0a    = 0;
    uint32_t pll_pd0b    = 0;
    uint32_t pll_pd1a    = 0;
    uint32_t pll_pd1b    = 0;

public:
    bool init           (void);
    void update         (void);
    bool selectPllOut   (void);
    bool initMSP        (Hal_gpio       &gpio           );
    bool selectHSE      (uint32_t       newValHSEClk    );
    
    bool setTypeSysClk  (RCU_SYSCLKCFG_SRC_Enum type    );
    void setClkOutCFG   (uint32_t           ClkOutCFG   );
    void setPllCFG0     (uint32_t           PllSysCFG0  );
    void setPllCFG1     (uint32_t           PllSysCFG1  );
    void setPllCFG2     (uint32_t           PllSysCFG2  );
    void setCGCFGAHB    (uint32_t           CGCFGAHB    );
    void setCGCFGAPB    (uint32_t           CGCFGAPB    );
    void setSecCNT0     (uint32_t           SecCNT0     );
    void setSecCNT1     (uint32_t           SecCNT1     );
    void setRstSys      (uint32_t           RstSys      );

    volatile RCU_TypeDef *  getInst         (void);
    uint32_t                getCoreClk      (void);
    uint32_t                getPll0Clk      (void);
    uint32_t                getPll1Clk      (void);
    uint32_t                getValHSEClk    (void);
    RCU_SYSCLKCFG_SRC_Enum  getTypeSysClk   (void);

    Hal_rcu(RCU_TypeDef *rcu_base);
    ~Hal_rcu(void);
};

#endif //__hal_rcu_h__
