#include "i2c.h"

void I2C_Init() {
    printf("[i2c] init...\n");
    i2c_config();
    printf("[i2c] init done!\n");
}
