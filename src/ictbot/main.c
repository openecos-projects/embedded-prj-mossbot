#include "driver/i2c_soft.h"
#include "driver/timer.h"
#include "driver/uart.h"
#include "module/lu9685.h"

#define SERVO_MOVE_ACTION 0

void main() {
    Timer_Init();
    I2C_Soft_Init();
    UART_Init(9600);

#if SERVO_MOVE_ACTION
    uint8_t uart_arr[4]  = {0};
    uint8_t uart_arr_cnt =  0;
    LU9685Struct servo_arr[] = {
        { 9, 0},
        {10, 0},
        {11, 0},
        {12, 0},
        {13, 0}
    };
    uint8_t servo_arr_len = LENGTH(servo_arr);
    uint8_t servo_action  = 0;
#else
    uint8_t uart_arr[5]  = {0};
    uint8_t uart_arr_cnt =  0;
    uint8_t servo_val_1  =  0;
    uint8_t servo_val_2  =  0;
#endif

    while (1) {
        while (((UART_1_REG_LSR & 0x080) >> 7) == 1);
        uint8_t uart_data = (uint8_t)UART_1_REG_TRX;
#if SERVO_MOVE_ACTION
        uint8_t uart_flag = UART_RecvDataAction(uart_arr,
                                               &uart_arr_cnt,
                                                uart_data,
                                               &servo_action);
#else
        uint8_t uart_flag = UART_ReceDataAngle(uart_arr,
                                              &uart_arr_cnt,
                                               uart_data,
                                              &servo_val_1,
                                              &servo_val_2);
#endif
        if (uart_flag == UART_STATUS_SUCCESS) {
            UART_SendData(uart_flag);
#if SERVO_MOVE_ACTION
            LU9685_SetAction(servo_arr, servo_arr_len, servo_action);
#else
            LU9685_SetAngle(servo_val_1, servo_val_2);
#endif
        }
        else if (uart_flag == UART_STATUS_ERROR) {
            UART_SendData(uart_flag);
        }
        else {
        }
    }
}
