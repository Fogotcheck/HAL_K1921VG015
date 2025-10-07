#include "hal_uart.h"

Hal_uart::Hal_uart(UART_TypeDef *pInstance)
    : Hal_communBase<UART_TypeDef>(pInstance)
{
}

bool Hal_uart::transmit(uint8_t *data, uint16_t len, void* ctx)
{
    if (transmitFunc)
        return std::invoke(transmitFunc, data, len, ctx);
    
    return false;
}

bool Hal_uart::receive(uint8_t *buf, uint16_t len, void* ctx)
{
    if (receiveFunc)
        return std::invoke(receiveFunc, buf, len, ctx);
    
    return false;
}

bool Hal_uart::enable(void)
{
    if (instance == nullptr)
        return false;

    calkCoef();

    RCU->CGCFGAPB_bit.UART0EN = 1;
    RCU->RSTDISAPB_bit.UART0EN = 1;

    int8_t uartNum = getUartNum();
    if (uartNum < 0)
        return false;

    RCU->UARTCLKCFG[uartNum].UARTCLKCFG_bit.CLKSEL = param.clkType;
    RCU->UARTCLKCFG[uartNum].UARTCLKCFG_bit.DIVEN = param.RCU_DIVEN;
    RCU->UARTCLKCFG[uartNum].UARTCLKCFG_bit.DIVN = param.RCU_DIVN;
    RCU->UARTCLKCFG[uartNum].UARTCLKCFG_bit.RSTDIS = 1;
    RCU->UARTCLKCFG[uartNum].UARTCLKCFG_bit.CLKEN = 1;

    instance->IBRD = baudIcoef;
    instance->FBRD = baudFcoef;

    return true;
}

bool Hal_uart::disable(void)
{
    return false;
}

int8_t Hal_uart::getUartNum(void)
{
    uint32_t address = (uint32_t)instance;
    int8_t num = -1;

    switch (address)
    {
    case UART0_BASE:
        num = 0;
        break;
    case UART1_BASE:
        num = 1;
        break;
    case UART2_BASE:
        num = 2;
        break;
    case UART3_BASE:
        num = 3;
        break;
    case UART4_BASE:
        num = 4;
        break;
    default:
        break;
    }

    return num;
}

void Hal_uart::calkCoef(void)
{
    baudIcoef = param.valClkRef / (16 * param.baudRate);
    baudFcoef = ((param.valClkRef / (16.0f * param.baudRate) - baudIcoef) * 64 + 0.5f);
}

void Hal_uart::setMode(mode_t newMode)
{
    mode = newMode;
    switch (newMode)
    {
    case POLLING_MODE:
        transmitFunc = [this](uint8_t* data, uint16_t len, void *ctx) { return this->transmitPolling(data, len, ctx); };
        receiveFunc  = [this](uint8_t* data, uint16_t len, void *ctx) { return this->receivePolling (data, len, ctx); };
        break;

    default:
        transmitFunc = nullptr;
        receiveFunc  = nullptr;
        break;
    }
}

bool Hal_uart::transmitPolling(uint8_t *data, uint16_t len, void* ctx)
{
    if (data == nullptr || len == 0)
        return false;

    for (uint16_t i = 0; i < len; i++) {
        while (instance->FR & UART_FR_TXFF_Msk) {}

        instance->DR = data[i];
    }

    while (!(instance->FR & UART_FR_TXFE_Msk)) {}
    while (instance->FR & UART_FR_BUSY_Msk) {}

    if (callbackTX)
        std::invoke(callbackTX, true, data, len, ctx);

    return true;
}

bool Hal_uart::receivePolling(uint8_t *data, uint16_t len, void* ctx)
{
    if (data == nullptr || len == 0)
        return false;

    for (uint16_t i = 0; i < len; i++) {
        while (instance->FR & UART_FR_RXFE_Msk) {}
        
        data[i] = (uint8_t)(instance->DR & 0xFF);
    }

    if (callbackRX)
        std::invoke(callbackRX, true, data, len, ctx);

    return true;
}