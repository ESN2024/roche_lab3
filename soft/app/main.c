#include <unistd.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "opencores_i2c.h"

unsigned int I2C_READ_ADXL345() {

    unsigned int data;

    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 0);
    I2C_write(OPENCORES_I2C_0_BASE,0x32,0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);

    data = I2C_read(OPENCORES_I2C_0_BASE, 0);
}

int main() {

    __uint32_t X0,X1,Y0 ; 
    I2C_init(OPENCORES_I2C_0_BASE,ALT_CPU_CPU_FREQ,400000); // 

    while(1) {
        I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 0);
        I2C_write(OPENCORES_I2C_0_BASE,0x32,0);
        I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
        X0 = I2C_read(OPENCORES_I2C_0_BASE, 0);

        I2C_write(OPENCORES_I2C_0_BASE,0x33,0);
        I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
        X1 = I2C_read(OPENCORES_I2C_0_BASE, 0);

        I2C_write(OPENCORES_I2C_0_BASE,0x34,0);
        I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
        Y0 = I2C_read(OPENCORES_I2C_0_BASE, 1);

        alt_printf("X1 : %x, X0 : %x \n", X1, X0);
        alt_printf("Y0 : %x", Y0);
    }
    return 0;
}


