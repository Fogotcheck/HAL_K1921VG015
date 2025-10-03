#include "hal_gpio.h"

Hal_gpio::Hal_gpio(GPIO_TypeDef *pPort, uint16_t pinNum)
    : port(pPort), pin(pinNum)
{
}

Hal_gpio::~Hal_gpio()
{
}

void Hal_gpio::configure(void)
{
    configure(config.stOutSet);
    configure(config.outClr);
    configure(config.pullModeSet);
    configure(config.outModeSet);
    configure(config.outEnSet);
    configure(config.outEnClrSet);
    configure(config.altFuncSet);
    configure(config.altFuncClrSet);
    configure(config.altFuncNumSet);
    configure(config.syncSet);
    configure(config.syncClr);
    configure(config.qualSet);
    configure(config.qualClr);
    configure(config.qualModeSet);
    configure(config.qualModeClr);
    configure(config.qualSampleSet);
    configure(config.intEnSet);
    configure(config.intEnClr);
    configure(config.intTypeSet);
    configure(config.intTypeClr);
    configure(config.intPolSet);
    configure(config.intPolClr);
    configure(config.intEdgeSet);
    configure(config.intEdgeClr);
    configure(config.DmaReqSet);
    configure(config.DmaReqClr);
    configure(config.AdcSocSet);
    configure(config.AdcSocClr);
    configure(config.LockKeySet);
}

void Hal_gpio::configure(_GPIO_DATAOUTSET_bits outSet)
{
    port->DATAOUTSET = *(uint32_t *)&outSet; // write-only SET регистр - просто записать
}

void Hal_gpio::configure(_GPIO_DATAOUTCLR_bits outClr)
{
    port->DATAOUTCLR = *(uint32_t *)&outClr; // write-only CLR регистр - просто записать
}

void Hal_gpio::configure(_GPIO_PULLMODE_bits pullModeSet)
{
    port->PULLMODE |= *(uint32_t *)&pullModeSet; // обычный регистр - чтение/маскирование/запись
}

void Hal_gpio::configure(_GPIO_OUTMODE_bits outModeSet)
{
    port->OUTMODE |= *(uint32_t *)&outModeSet;
}

void Hal_gpio::configure(_GPIO_OUTENSET_bits outEnSet)
{
    port->OUTENSET |= *(uint32_t *)&outEnSet;
}

void Hal_gpio::configure(_GPIO_OUTENCLR_bits outEnClrSet)
{
    port->OUTENCLR = *(uint32_t *)&outEnClrSet;
}

void Hal_gpio::configure(_GPIO_ALTFUNCSET_bits altFuncSet)
{
    port->ALTFUNCSET |= *(uint32_t *)&altFuncSet;
}

void Hal_gpio::configure(_GPIO_ALTFUNCCLR_bits altFuncClrSet)
{
    port->ALTFUNCCLR = *(uint32_t *)&altFuncClrSet;
}

void Hal_gpio::configure(_GPIO_ALTFUNCNUM_bits altFuncNumSet)
{
    port->ALTFUNCNUM |= *(uint32_t *)&altFuncNumSet;
}

void Hal_gpio::configure(_GPIO_SYNCSET_bits syncSet)
{
    port->SYNCSET |= *(uint32_t *)&syncSet;
}

void Hal_gpio::configure(_GPIO_SYNCCLR_bits syncClr)
{
    port->SYNCCLR = *(uint32_t *)&syncClr;
}

void Hal_gpio::configure(_GPIO_QUALSET_bits qualSet)
{
    port->QUALSET |= *(uint32_t *)&qualSet;
}

void Hal_gpio::configure(_GPIO_QUALCLR_bits qualClr)
{
    port->QUALCLR = *(uint32_t *)&qualClr;
}

void Hal_gpio::configure(_GPIO_QUALMODESET_bits qualModeSet)
{
    port->QUALMODESET |= *(uint32_t *)&qualModeSet;
}

void Hal_gpio::configure(_GPIO_QUALMODECLR_bits qualModeClr)
{
    port->QUALMODECLR = *(uint32_t *)&qualModeClr;
}

void Hal_gpio::configure(_GPIO_QUALSAMPLE_bits qualSampleSet)
{
    port->QUALSAMPLE |= *(uint32_t *)&qualSampleSet; // обычный регистр
}

void Hal_gpio::configure(_GPIO_INTENSET_bits intEnSet)
{
    port->INTENSET |= *(uint32_t *)&intEnSet;
}

void Hal_gpio::configure(_GPIO_INTENCLR_bits intEnClr)
{
    port->INTENCLR = *(uint32_t *)&intEnClr;
}

void Hal_gpio::configure(_GPIO_INTTYPESET_bits intTypeSet)
{
    port->INTTYPESET |= *(uint32_t *)&intTypeSet;
}

void Hal_gpio::configure(_GPIO_INTTYPECLR_bits intTypeClr)
{
    port->INTTYPECLR = *(uint32_t *)&intTypeClr;
}

void Hal_gpio::configure(_GPIO_INTPOLSET_bits intPolSet)
{
    port->INTPOLSET |= *(uint32_t *)&intPolSet;
}

void Hal_gpio::configure(_GPIO_INTPOLCLR_bits intPolClr)
{
    port->INTPOLCLR = *(uint32_t *)&intPolClr;
}

void Hal_gpio::configure(_GPIO_INTEDGESET_bits intEdgeSet)
{
    port->INTEDGESET |= *(uint32_t *)&intEdgeSet;
}

void Hal_gpio::configure(_GPIO_INTEDGECLR_bits intEdgeClr)
{
    port->INTEDGECLR = *(uint32_t *)&intEdgeClr;
}

void Hal_gpio::configure(_GPIO_DMAREQSET_bits DmaReqSet)
{
    port->DMAREQSET |= *(uint32_t *)&DmaReqSet;
}

void Hal_gpio::configure(_GPIO_DMAREQCLR_bits DmaReqClr)
{
    port->DMAREQCLR = *(uint32_t *)&DmaReqClr;
}

void Hal_gpio::configure(_GPIO_ADCSOCSET_bits AdcSocSet)
{
    port->ADCSOCSET |= *(uint32_t *)&AdcSocSet;
}

void Hal_gpio::configure(_GPIO_ADCSOCCLR_bits AdcSocClr)
{
    port->ADCSOCCLR = *(uint32_t *)&AdcSocClr;
}

void Hal_gpio::configure(_GPIO_LOCKKEY_bits LockKeySet)
{
    port->LOCKKEY = *(uint32_t *)&LockKeySet; 
}

void Hal_gpio::write(bool val)
{
    if (val)
        port->DATAOUTSET = (1 << pin);
    else
        port->DATAOUTCLR = (1 << pin);
}

bool Hal_gpio::read(void)
{
    return (port->DATA & (1 << pin)) != 0;
}

void Hal_gpio::toggle(void)
{
    GPIOA->DATAOUTTGL = (1 << pin);
}

volatile GPIO_TypeDef * Hal_gpio::getPort(void)
{
    return port;
}