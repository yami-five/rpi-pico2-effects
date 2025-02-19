#include "pico/stdlib.h"

void blink()
{
    gpio_put(25,1);
    sleep_ms(500);
    gpio_put(25,0);
    sleep_ms(500);
}