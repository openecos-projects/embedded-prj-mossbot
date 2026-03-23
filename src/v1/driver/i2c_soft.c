#include "i2c_soft.h"

void I2C_Soft_Init() {
    printf("[i2c soft] init...\n");
    GPIO_SetDir_Num(GPIO_0, I2C_SOFT_SCL_PIN, GPIO_OUT);
    GPIO_SetDir_Num(GPIO_0, I2C_SOFT_SDA_PIN, GPIO_OUT);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 1);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN, 1);
    printf("[i2c soft] init done!\n");
}

void I2C_Soft_Start() {
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN, 1);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 1);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN, 0);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 0);
}

void I2C_Soft_ReStart() {
    GPIO_SetDir_Num(GPIO_0, I2C_SOFT_SDA_PIN, GPIO_OUT);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN, 1);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 1);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN, 0);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 0);
}

void I2C_Soft_Stop() {
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN, 0);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 1);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN, 1);
}

uint8_t I2C_Soft_WaitAck() {
    uint8_t ack = 0;
    
    GPIO_SetDir_Num(GPIO_0, I2C_SOFT_SDA_PIN, GPIO_IN);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 1);

    ack = GPIO_GetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN);

    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 0);
    GPIO_SetDir_Num(GPIO_0, I2C_SOFT_SDA_PIN, GPIO_OUT);
    
    return ack == 0;
}

void I2C_Soft_SendAck(uint8_t ack) {
    GPIO_SetDir_Num(GPIO_0, I2C_SOFT_SDA_PIN, GPIO_OUT);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN, ack ? 1 : 0);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 1);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 0);
    GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN, 1);
}

uint8_t I2C_Soft_WriteByte(uint8_t byte) {
    for (int i = 0; i < 8; i++) {
        GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN, (byte & (0x80 >> i)) ? 1 : 0);
        GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 1);
        GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 0);
    }
    return I2C_Soft_WaitAck();
}

uint8_t I2C_Soft_ReadByteAck(uint8_t ack) {
    uint8_t byte = 0;
    GPIO_SetDir_Num(GPIO_0, I2C_SOFT_SDA_PIN, GPIO_IN);
    for (int i = 0; i < 8; i++) {
        GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 1);
        byte <<= 1;
        if (GPIO_GetVal_Num(GPIO_0, I2C_SOFT_SDA_PIN)) {
            byte |= 0x01;
        }
        GPIO_SetVal_Num(GPIO_0, I2C_SOFT_SCL_PIN, 0);
    }
    I2C_Soft_SendAck(ack);
    return byte;
}

uint8_t I2C_Soft_ReadByte() {
    return I2C_Soft_ReadByteAck(1);
}

void I2C_Soft_WriteByteN(uint8_t slv_addr, uint16_t reg_addr, uint8_t type,
                         uint8_t num, uint8_t *data) {
    I2C_Soft_Start();
    I2C_Soft_WriteByte(slv_addr);

    if (type == I2C_DEV_ADDR_16BIT) {
        I2C_Soft_WriteByte((uint8_t)((reg_addr >> 8) & 0xFF));
        I2C_Soft_WriteByte((uint8_t)(reg_addr & 0xFF));
    } 
    else {
        I2C_Soft_WriteByte((uint8_t)(reg_addr & 0xFF));
    }

    for (int i = 0; i < num; ++i) {
        I2C_Soft_WriteByte(*data);
        ++data;
    }
    I2C_Soft_Stop();
}
