#ifndef __hal_gpio_h__
#define __hal_gpio_h__

#include "K1921VG015.h"
#include "hal_periphBase.h"

class Hal_gpio : public Hal_periphBase<GPIO_TypeDef>
{
private:
    uint16_t pin;

public:
    explicit Hal_gpio(GPIO_TypeDef *pInstance, uint16_t pinNum);
    ~Hal_gpio() override = default;

    void write      (bool val);
    bool read       (void);
    void toggle     (void);

    bool enable     (void) override;
    bool disable    (void) override;
};

#endif //__hal_gpio_h__