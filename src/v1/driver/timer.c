#include "timer.h"

void Timer_Init() {
    printf("[timer] init...\n");
    timer_init(1, 100000);
    printf("[timer] init done!\n");
}

void Timer_DelayMs(uint32_t val) {
    TIMER_0_REG_CTRL = (uint32_t)0xD;
    for (int i = 0; i < val; ++i) {
        while (TIMER_0_REG_STAT == 0);
    }
    TIMER_0_REG_CTRL = (uint32_t)0xD;
}
