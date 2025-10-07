#ifndef __hal_periphBase_h__
#define __hal_periphBase_h__

#include <cstdint>

template <typename PeriphInstance>
class Hal_periphBase
{
public:
    volatile PeriphInstance *instance = nullptr;

    explicit Hal_periphBase(PeriphInstance *pInstance)
        : instance(pInstance)
    {
    }

    virtual ~Hal_periphBase()   = default;

    virtual bool enable(void)   = 0;
    virtual bool disable(void)  = 0;
};

template <typename PeriphInstance>
class Hal_communBase
{
public:
    volatile PeriphInstance *instance = nullptr;

    explicit Hal_communBase(PeriphInstance *pInstance)
        : instance(pInstance)
    {
    }

    virtual ~Hal_communBase()   = default;

    virtual bool enable(void)   = 0;
    virtual bool disable(void)  = 0;

    virtual bool transmit   (uint8_t *data, uint16_t len, void* ctx) = 0;
    virtual bool receive    (uint8_t *buf,  uint16_t len, void* ctx) = 0;
};

#endif // __hal_periphBase_h__
