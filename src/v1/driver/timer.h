#ifndef TIMER_H
#define TIMER_H

#include <klib.h>
#include <ecos.h>

void Timer_Init();
void Timer_DelayMs(uint32_t val);

#endif
