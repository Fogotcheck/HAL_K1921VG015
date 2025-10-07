#include "hal_gpio.h"

Hal_gpio::Hal_gpio(GPIO_TypeDef *pInstance, uint16_t pinNum)
    : Hal_periphBase<GPIO_TypeDef>(pInstance), pin(pinNum)
{
}

bool Hal_gpio::enable(void)
{
    uint32_t address = (uint32_t)instance;

    switch (address)
    {
    case GPIOA_BASE:
        RCU->CGCFGAHB_bit.GPIOAEN   = 1;
        RCU->RSTDISAHB_bit.GPIOAEN  = 1;
        break;
    case GPIOB_BASE:
        RCU->CGCFGAHB_bit.GPIOBEN   = 1;
        RCU->RSTDISAHB_bit.GPIOBEN  = 1;
        break;
    case GPIOC_BASE:
        RCU->CGCFGAHB_bit.GPIOCEN   = 1;
        RCU->RSTDISAHB_bit.GPIOCEN  = 1;
        break;
    default:
        return false;
    }
    return true;
}

bool Hal_gpio::disable(void)
{
    return false;
}

void Hal_gpio::write(bool val)
{
    if (val)
        instance->DATAOUTSET = (1 << pin);
    else
        instance->DATAOUTCLR = (1 << pin);
}

bool Hal_gpio::read(void)
{
    return (instance->DATA & (1 << pin)) != 0;
}

void Hal_gpio::toggle(void)
{
    instance->DATAOUTTGL = (1 << pin);
}
