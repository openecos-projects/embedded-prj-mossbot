#ifndef I2C_SOFT_H
#define I2C_SOFT_H

#include <klib.h>
#include <ecos.h>
#include "timer.h"

#define I2C_SOFT_SCL_PIN 14
#define I2C_SOFT_SDA_PIN 15

void I2C_Soft_Init();
void I2C_Soft_Start();
void I2C_Soft_ReStart();
void I2C_Soft_Stop();
uint8_t I2C_Soft_WriteByte(uint8_t byte);
uint8_t I2C_Soft_WaitAck();
void I2C_Soft_SendAck(uint8_t ack);
uint8_t I2C_Soft_ReadByteAck(uint8_t ack);
uint8_t I2C_Soft_ReadByte();
void I2C_Soft_WriteByteN(uint8_t slv_addr, uint16_t reg_addr, uint8_t type,
                         uint8_t num, uint8_t *data);

#endif
