#include "lu9685.h"

void LU9685_SetAngleSingle(uint8_t servo_idx, uint8_t servo_val) {
    printf("[lu9685] move servo %d to %d degree...\n", servo_idx, servo_val);
    I2C_Soft_WriteByteN(0x00, servo_idx, I2C_DEV_ADDR_8BIT, 1, &servo_val);
    printf("[lu9685] move servo done!\n");
}

void LU9685_SetAngleMulti(LU9685Struct *servo_arr_p, uint8_t servo_arr_len) {
    char servo_num_str[100] = {0};
    char servo_val_str[100] = {0};
    uint8_t servo_arr[] = {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    for (uint8_t i = 0; i < servo_arr_len; i++) {
        for (uint8_t j = 0; j < LENGTH(servo_arr); j++) {
            if (servo_arr_p->num == j) {
                if (servo_num_str[0] == 0) {
                    sprintf(servo_num_str, "%d", j);
                    sprintf(servo_val_str, "%d", servo_arr_p->val);
                }
                else {
                    sprintf(servo_num_str, "%s, %d", servo_num_str, j);
                    sprintf(servo_val_str, "%s, %d", servo_val_str, servo_arr_p->val);
                }

                servo_arr[j] = servo_arr_p->val;
            }
            else {
            }
        }
        servo_arr_p++;
    }

    PRINT_ARR(servo_arr, "lu9685");
    printf("[lu9685] move servo [%s] to [%s] degrees...\n", servo_num_str,
                                                            servo_val_str);
    I2C_Soft_WriteByteN(0x00, 0xFD, I2C_DEV_ADDR_8BIT, LENGTH(servo_arr), servo_arr);
    printf("[lu9685] move servo done!\n");
}

void LU9685_SetAction(LU9685Struct *servo_arr_p, uint8_t servo_arr_len,
                                                 uint8_t servo_action) {
    printf("[lu9685] move servo action 0x%x...\n", servo_action);
    switch (servo_action) {
        case 0x00:
            for (uint8_t i = 0; i < 10; i++) {
                SET_SERVO_POS(servo_arr_p,  70,  70,  70,  70,  70);
                SET_SERVO_POS(servo_arr_p, 110, 110, 110, 110, 110);
            }
            break;
        case 0x01:
            break;
        case 0x02:
            break;
        case 0x03:
            break;
        case 0x04:
            break;
        case 0x05:
            break;
        case 0x06:
            break;
        case 0x07:
            break;
        case 0x08:
            break;
        case 0x09:
            break;
        default:
            printf("[lu9685] move servo action 0x%x is not supported!\n",
                   servo_action);
            break;
    }
    printf("[lu9685] move servo action done!\n\n");
}
