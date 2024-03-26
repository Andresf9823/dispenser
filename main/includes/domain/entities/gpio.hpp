#include <date.h>

typedef struct _gpio_switch_t
{
    uint64_t id;
    bool enable;
    uint32_t timeOutMs;
    date_t lastTimeTurnOn;
    date_t lastTimeTurnOff;
} gpio_switch_t;
