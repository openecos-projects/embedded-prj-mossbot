#ifndef LU9685_H
#define LU9685_H

#include <klib-macros.h>
#include <ecos.h>
#include "../driver/i2c_soft.h"
#include "../driver/timer.h"
#include "../util/debug.h"

#define SET_SERVO_POS(arr, v0, v1, v2, v3, v4) do {    \
    arr[0].val = v0;                                   \
    arr[1].val = v1;                                   \
    arr[2].val = v2;                                   \
    arr[3].val = v3;                                   \
    arr[4].val = v4;                                   \
    for (uint8_t i = 0; i < 5; i++) {                  \
        LU9685_SetAngleSingle(arr[i].num, arr[i].val); \
    }                                                  \
    Timer_DelayMs(300);                                \
} while(0)

typedef struct {
    uint8_t num;
    uint8_t val;
} LU9685Struct;

void LU9685_Init(LU9685Struct *servo_arr_p, uint8_t *servo_arr_len_p);
void LU9685_Reset();
void LU9685_SetAngleSingle(uint8_t servo_idx, uint8_t servo_val);
void LU9685_SetAngleMulti(LU9685Struct *servo_arr_p, uint8_t servo_arr_len);
void LU9685_SetAction(LU9685Struct *servo_arr_p, uint8_t servo_arr_len,
                                                 uint8_t servo_action);

#endif
