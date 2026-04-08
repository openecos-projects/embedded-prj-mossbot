#ifndef UART_H
#define UART_H

#include <klib.h>
#include <ecos.h>
#include "../util/debug.h"

#define UART_1_BASE_ADDR    0x10001000
#define UART_1_REG_LCR      *((volatile uint32_t *)(UART_1_BASE_ADDR + 0))
#define UART_1_REG_DIV      *((volatile uint32_t *)(UART_1_BASE_ADDR + 4))
#define UART_1_REG_TRX      *((volatile uint32_t *)(UART_1_BASE_ADDR + 8))
#define UART_1_REG_FCR      *((volatile uint32_t *)(UART_1_BASE_ADDR + 12))
#define UART_1_REG_LSR      *((volatile uint32_t *)(UART_1_BASE_ADDR + 16))

#define UART_STATUS_PASS    0
#define UART_STATUS_SUCCESS 1
#define UART_STATUS_ERROR   2

void UART_Init(uint32_t baud);
uint8_t UART_RecvDataAction(uint8_t *uart_arr_p,
                            uint8_t *uart_arr_cnt_p,
                            uint8_t  uart_data,
                            uint8_t *servo_action_p);
uint8_t UART_ReceDataAngle(uint8_t *uart_arr_p,
                           uint8_t *uart_arr_cnt_p,
                           uint8_t  uart_data,
                           uint8_t *servo_val_1_p,
                           uint8_t *servo_val_2_p);
void UART_SendData(uint8_t uart_flag);

#endif
