#include "uart.h"

void UART_Init(uint32_t baud) {
    printf("[uart] init...\n");
    // GPIO_0_0 is UART_1_RX, GPIO_0_1 is UART_1_TX.
    GPIO_0_REG_IOFCFG = (uint32_t)0x3;
    GPIO_0_REG_PINMUX = (uint32_t)0;
    // Set baud rate
    // [15:0] is DIV: clock divide value.
    UART_1_REG_DIV = (uint32_t)(100000000 / baud);
    // Disable transmit and receive fifo, and set receive fifo threshold is 14.
    // [4:3] is RX_TRG_LEVL: receive trigger threshold.
    //     RX_TRG_LEVL = 2'b00: 1 receive fifo element threshold.
    //     RX_TRG_LEVL = 2'b01: 2 receive fifo elements threshold.
    //     RX_TRG_LEVL = 2'b10: 8 receive fifo elements threshold.
    //     RX_TRG_LEVL = 2'b11: 14 receive fifo elements threshold.
    // [1:1] is TF_CLR: transmit fifo clear.
    //     TF_CLR = 1'b0: transmit fifo writable.
    //     TF_CLR = 1'b1: transmit fifo clear.
    // [0:0] is RF_CLR: receive fifo clear.
    //     RF_CLR = 1'b0: receive fifo readable.
    //     RF_CLR = 1'b1: receive fifo clear.
    UART_1_REG_FCR = (uint32_t)0b1111;
    // Enable transmit and receive fifo.
    UART_1_REG_FCR = (uint32_t)0b1100;
    // Enable receive, transmit and parity error interrupt, and set data word
    // length to 8-bit.
    // [4:3] is WLS: world length.
    //     WLS = 2'b00: 5-bit data word length.
    //     WLS = 2'b01: 6-bit data word length.
    //     WLS = 2'b10: 7-bit data word length.
    //     WLS = 2'b11: 8-bit data word length.
    // [2:2] is PEIE: parity error interrupt enable.
    //     PEIE = 1'b0: parity error interrupt disable.
    //     PEIE = 1'b1: otherwise.
    // [1:1] is TXIE: transmit interrupt enable.
    //     TXIE = 1'b0: transmit interrupt disable.
    //     TXIE = 1'b1: otherwise.
    // [0:0] is RXIE: receive interrupt enable.
    //     RXIE = 1'b0: receive interrupt disable.
    //     RXIE = 1'b1: otherwise.
    UART_1_REG_LCR = (uint32_t)0b00011111;
    printf("[uart] init done!\n");
}

uint8_t UART_RecvDataAction(uint8_t *uart_arr_p,
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

uint8_t UART_ReceDataAngle(uint8_t *uart_arr_p,
                           uint8_t *uart_arr_cnt_p,
                           uint8_t  uart_data,
                           uint8_t *servo_val_1_p,
                           uint8_t *servo_val_2_p) {
    printf("[uart] data %d is 0x%x\n", *uart_arr_cnt_p, uart_data);

    uart_arr_p[*uart_arr_cnt_p] = uart_data;
    (*uart_arr_cnt_p)++;

    PRINT_ARR_PTR(uart_arr_p, 5, "uart");

    if (*uart_arr_cnt_p == 1 || *uart_arr_cnt_p == 2) {
        if (uart_data != 0xFF) {
            memset(uart_arr_p, 0, 5);
            *uart_arr_cnt_p = 0;
            printf("[uart] header is invalid!\n");
        }
        else {
        }
    }
    else if (*uart_arr_cnt_p == 5) {
        uint8_t uart_checksum = uart_arr_p[0] + uart_arr_p[1] +
                                uart_arr_p[2] + uart_arr_p[3];
        uint8_t uart_checkbit = uart_arr_p[4];
        uint8_t uart_servo_val_1 = uart_arr_p[2];
        uint8_t uart_servo_val_2 = uart_arr_p[3];
        printf("[uart] checksum is %x + %x + %x + %x = %x\n",
               uart_arr_p[0], uart_arr_p[1], uart_arr_p[2], uart_arr_p[3], uart_checksum);
        printf("[uart] checkbit is %x\n", uart_checkbit);
        memset(uart_arr_p, 0, 5);

        *uart_arr_cnt_p = 0;
        if (uart_checksum == uart_checkbit) {
            *servo_val_1_p = uart_servo_val_1;
            *servo_val_2_p = uart_servo_val_2;
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
