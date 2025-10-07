#include <sys/errno.h>
#include <string.h>
#include <unistd.h>

#include "memasm.h"

extern char _tdata_start[], _tdata_end[], _tbss_start[], _tbss_end[];

extern const uint8_t __data_source_start;
extern uint8_t __data_target_start;
extern uint8_t __data_target_end;

extern const uint8_t __sdata_source_start;
extern uint8_t __sdata_target_start;
extern uint8_t __sdata_target_end;

extern const uint8_t __init_array_source_start;
extern void (*__init_array_target_start[])(void);
extern void (*__init_array_target_end[])(void);

extern const uint8_t __fini_array_source_start;
extern uint8_t __fini_array_target_start;
extern uint8_t __fini_array_target_end;

extern void plf_init_relocate(void) __attribute__((weak));

extern char __bss_start[], __bss_end[];

typedef void (*init_func_t)(void);

void __init plf_init_noreloc(void)
{
}

void __init plf_init_generic(void)
{
    memset(__bss_start, 0, (size_t)(__bss_end - __bss_start));

    memcpy((void *)&__data_target_start,
           (const void *)&__data_source_start,
           (&__data_target_end - &__data_target_start));

    memcpy((void *)&__sdata_target_start,
           (const void *)&__sdata_source_start,
           (&__sdata_target_end - &__sdata_target_start));

    memcpy((void *)&__init_array_target_start,
           (const void *)&__init_array_source_start,
           ((uint8_t *)__init_array_target_end - (uint8_t *)__init_array_target_start));

    memcpy((void *)&__fini_array_target_start,
           (const void *)&__fini_array_source_start,
           (&__fini_array_target_end - &__fini_array_target_start));

    size_t count = __init_array_target_end - __init_array_target_start;
    for (size_t i = 0; i < count; i++)
    {
        __init_array_target_start[i]();
    }
}

void plf_init(void) __attribute__((weak, alias("plf_init_generic")));
