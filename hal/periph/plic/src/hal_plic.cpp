#include "hal_plic.h"

Hal_plic::Hal_plic(Plic_IsrVect_TypeDef *pInstance)
    : Hal_periphBase<Plic_IsrVect_TypeDef>(pInstance)
{
}


bool enable(void)
{
    return false;
}

bool disable(void)
{
    return false;
}
