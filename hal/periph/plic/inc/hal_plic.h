#ifndef __hal_plic_h__
#define __hal_plic_h__

#include "K1921VG015.h"
#include "hal_periphBase.h"

class Hal_plic: public Hal_periphBase<Plic_IsrVect_TypeDef>
{
private:
    
public:
    explicit Hal_plic(Plic_IsrVect_TypeDef *pInstance);
    ~Hal_plic() override = default;

    bool enable         (void) override;
    bool disable        (void) override;
    
};



#endif //__hal_plic_h__