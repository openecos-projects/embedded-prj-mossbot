#include "uart.h"

void UART_Init(uint32_t baud) {
    printf("[uart] init...\n");
    GPIO_0_REG_IOFCFG = (uint32_t)0x3;
    GPIO_0_REG_PINMUX = (uint32_t)0;

    UART_1_REG_DIV = (uint32_t)(100000000 / baud);  // 100x10^6 / 115200
    UART_1_REG_FCR = (uint32_t)0b1111;              // clear tx and rx fifo
    UART_1_REG_FCR = (uint32_t)0b1100;
    UART_1_REG_LCR = (uint32_t)0b00011111;          // 8N1, en all irq
    // printf("REG_DIV: %x REG_LCR: %x\n", UART_1_REG_DIV, UART_1_REG_LCR);
    printf("[uart] init done!\n");
}

uint8_t UART_RecvData(uint8_t *uart_arr_p,
                      uint8_t *uart_arr_cnt_p,
                      uint8_t  uart_data,
                      uint8_t *servo_action_p) {
    printf("[uart] data %d is 0x%x\n", *uart_arr_cnt_p, uart_data);

    uart_arr_p[*uart_arr_cnt_p] = uart_data;
    (*uart_arr_cnt_p)++;

    PRINT_ARR_PTR(uart_arr_p, 4, "uart");

    if (*uart_arr_cnt_p == 1 || *uart_arr_cnt_p == 2) {
        if (uart_data != 0xFF) {
            memset(uart_arr_p, 0, 4);
            *uart_arr_cnt_p = 0;
            printf("[uart] header is invalid!\n");
        }
        else {
        }
    }
    else if (*uart_arr_cnt_p == 4) {
        uint8_t uart_checksum = uart_arr_p[0] + uart_arr_p[1] + uart_arr_p[2];
        uint8_t uart_checkbit = uart_arr_p[3];
        uint8_t uart_action   = uart_arr_p[2];
        printf("[uart] checksum is %x + %x + %x = %x\n",
               uart_arr_p[0], uart_arr_p[1], uart_arr_p[2], uart_checksum);
        printf("[uart] checkbit is %x\n", uart_checkbit);
        memset(uart_arr_p, 0, 4);

        *uart_arr_cnt_p = 0;
        if (uart_checksum == uart_checkbit) {
            *servo_action_p = uart_action;
            printf("[uart] checksum is ok!\n");
            return UART_STATUS_SUCCESS;
        }
        else {
            printf("[uart] checksum is invalid!\n");
            return UART_STATUS_ERROR;
        }
    }
    else {
    }

    return UART_STATUS_PASS;
}

void UART_SendData(uint8_t uart_flag) {
    printf("[uart] send response...\n");
    uint32_t uart_data = (uart_flag == UART_STATUS_SUCCESS) ? 0x00 : 0xFF;
    char *uart_lf = (uart_flag == UART_STATUS_SUCCESS) ? "" : "\n";
    while (((UART_1_REG_LSR & 0x100) >> 8) == 1);
    UART_1_REG_TRX = uart_data;
    printf("[uart] send response 0x%x done!\n%s", uart_data, uart_lf);
}
