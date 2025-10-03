#ifndef __hal_gpio_h__
#define __hal_gpio_h__

#include "K1921VG015.h"

class Hal_gpio
{
private:
    volatile GPIO_TypeDef *port = nullptr;
    uint16_t pin = 0;

public:
    struct hal_gpioConfig
    {
        _GPIO_DATAOUTSET_bits         stOutSet         ;
        _GPIO_DATAOUTCLR_bits         outClr         ;
        _GPIO_PULLMODE_bits           pullModeSet    ;
        _GPIO_OUTMODE_bits            outModeSet     ;
        _GPIO_OUTENSET_bits           outEnSet       ;
        _GPIO_OUTENCLR_bits           outEnClrSet    ;
        _GPIO_ALTFUNCSET_bits         altFuncSet     ;
        _GPIO_ALTFUNCCLR_bits         altFuncClrSet  ;
        _GPIO_ALTFUNCNUM_bits         altFuncNumSet  ;
        _GPIO_SYNCSET_bits            syncSet        ;
        _GPIO_SYNCCLR_bits            syncClr        ;
        _GPIO_QUALSET_bits            qualSet        ;
        _GPIO_QUALCLR_bits            qualClr        ;
        _GPIO_QUALMODESET_bits        qualModeSet    ;
        _GPIO_QUALMODECLR_bits        qualModeClr    ;
        _GPIO_QUALSAMPLE_bits         qualSampleSet  ;
        _GPIO_INTENSET_bits           intEnSet       ;
        _GPIO_INTENCLR_bits           intEnClr       ;
        _GPIO_INTTYPESET_bits         intTypeSet     ;
        _GPIO_INTTYPECLR_bits         intTypeClr     ;
        _GPIO_INTPOLSET_bits          intPolSet      ;
        _GPIO_INTPOLCLR_bits          intPolClr      ;
        _GPIO_INTEDGESET_bits         intEdgeSet     ;
        _GPIO_INTEDGECLR_bits         intEdgeClr     ;
        _GPIO_DMAREQSET_bits          DmaReqSet      ;
        _GPIO_DMAREQCLR_bits          DmaReqClr      ;
        _GPIO_ADCSOCSET_bits          AdcSocSet      ;
        _GPIO_ADCSOCCLR_bits          AdcSocClr      ;
        _GPIO_LOCKKEY_bits            LockKeySet     ;
    } config = {};
    
    void configure(void);
    void configure(_GPIO_DATAOUTSET_bits         outSet         );
    void configure(_GPIO_DATAOUTCLR_bits         outClr         );
    void configure(_GPIO_PULLMODE_bits           pullModeSet    );
    void configure(_GPIO_OUTMODE_bits            outModeSet     );
    void configure(_GPIO_OUTENSET_bits           outEnSet       );
    void configure(_GPIO_OUTENCLR_bits           outEnClrSet    );
    void configure(_GPIO_ALTFUNCSET_bits         altFuncSet     );
    void configure(_GPIO_ALTFUNCCLR_bits         altFuncClrSet  );
    void configure(_GPIO_ALTFUNCNUM_bits         altFuncNumSet  );
    void configure(_GPIO_SYNCSET_bits            syncSet        );
    void configure(_GPIO_SYNCCLR_bits            syncClr        );
    void configure(_GPIO_QUALSET_bits            qualSet        );
    void configure(_GPIO_QUALCLR_bits            qualClr        );
    void configure(_GPIO_QUALMODESET_bits        qualModeSet    );
    void configure(_GPIO_QUALMODECLR_bits        qualModeClr    );
    void configure(_GPIO_QUALSAMPLE_bits         qualSampleSet  );
    void configure(_GPIO_INTENSET_bits           intEnSet       );
    void configure(_GPIO_INTENCLR_bits           intEnClr       );
    void configure(_GPIO_INTTYPESET_bits         intTypeSet     );
    void configure(_GPIO_INTTYPECLR_bits         intTypeClr     );
    void configure(_GPIO_INTPOLSET_bits          intPolSet      );
    void configure(_GPIO_INTPOLCLR_bits          intPolClr      );
    void configure(_GPIO_INTEDGESET_bits         intEdgeSet     );
    void configure(_GPIO_INTEDGECLR_bits         intEdgeClr     );
    void configure(_GPIO_DMAREQSET_bits          DmaReqSet      );
    void configure(_GPIO_DMAREQCLR_bits          DmaReqClr      );
    void configure(_GPIO_ADCSOCSET_bits          AdcSocSet      );
    void configure(_GPIO_ADCSOCCLR_bits          AdcSocClr      );
    void configure(_GPIO_LOCKKEY_bits            LockKeySet     );

    void write(bool val);
    bool read(void);
    void toggle(void);

    volatile GPIO_TypeDef *getPort(void);

    Hal_gpio(GPIO_TypeDef *pPort, uint16_t pinNum);
    ~Hal_gpio();
};

#endif //__hal_gpio_h__