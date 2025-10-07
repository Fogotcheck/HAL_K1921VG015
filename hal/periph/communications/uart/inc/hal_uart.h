#ifndef __hal_uart_h__
#define __hal_uart_h__

#include <functional>

#include "K1921VG015.h"
#include "hal_periphBase.h"
#include "hal_gpio.h"

class Hal_uart : public Hal_communBase<UART_TypeDef>
{
public:
    struct hwParam
    {
        RCU_UARTCLKCFG_CLKSEL_Enum clkType      = RCU_UARTCLKCFG_CLKSEL_HSE;
        uint32_t valClkRef                      = 0;
        uint32_t baudRate                       = 115200;
        uint32_t RCU_DIVEN   :1;
        uint32_t RCU_DIVN    :6;
        
    } param;
    typedef enum{
        NULL_MODE,
        POLLING_MODE,
        ISR_MODE,
        DMA_MODE,
        USER_MODE,
    }mode_t;

    int8_t getUartNum(void);
    void calkCoef(void);
    void setMode(mode_t mode);

    bool transmitPolling(uint8_t *data, uint16_t len, void* ctx);
    bool transmitISR    (uint8_t *data, uint16_t len, void* ctx);
    bool transmitDMA    (uint8_t *data, uint16_t len, void* ctx);

    bool receivePolling (uint8_t *data, uint16_t len, void* ctx);
    bool receiveISR     (uint8_t *data, uint16_t len, void* ctx);
    bool receiveDMA     (uint8_t *data, uint16_t len, void* ctx);    

    bool transmit       (uint8_t *data, uint16_t len, void* ctx) override;
    bool receive        (uint8_t *buf,  uint16_t len, void* ctx) override;

    bool enable (void)  override;
    bool disable(void)  override;

    Hal_uart(UART_TypeDef *pInstance);
    ~Hal_uart() override = default;

private:
    uint32_t baudIcoef = 0;
    uint32_t baudFcoef = 0;
    mode_t   mode = NULL_MODE;
    
    std::function<bool(uint8_t*, uint16_t, void* ctx)> transmitFunc         = nullptr;
    std::function<bool(uint8_t*, uint16_t, void* ctx)> receiveFunc          = nullptr;
    std::function<void(bool res, uint8_t*, uint16_t, void* ctx)> callbackTX = nullptr;
    std::function<void(bool res, uint8_t*, uint16_t, void* ctx)> callbackRX = nullptr;
};

#endif //__hal_uart_h__