#include <unistd.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "opencores_i2c.h"

void I2C_READ_BURSTMODE_ADXL345(__uint8_t *X0, __uint8_t *Y0, __uint8_t *Z0, __uint8_t *X1, __uint8_t *Y1, __uint8_t *Z1) {

    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 0);

    I2C_write(OPENCORES_I2C_0_BASE,0x32,0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
    *X0 = I2C_read(OPENCORES_I2C_0_BASE, 0);

    I2C_write(OPENCORES_I2C_0_BASE,0x33,0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
    *X1 = I2C_read(OPENCORES_I2C_0_BASE, 0);

    I2C_write(OPENCORES_I2C_0_BASE,0x34,0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
    *Y0 = I2C_read(OPENCORES_I2C_0_BASE, 0);

    I2C_write(OPENCORES_I2C_0_BASE,0x35,0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
    *Y1 = I2C_read(OPENCORES_I2C_0_BASE, 0);

    I2C_write(OPENCORES_I2C_0_BASE,0x36,0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
    *Z0 = I2C_read(OPENCORES_I2C_0_BASE, 0);

    I2C_write(OPENCORES_I2C_0_BASE,0x37,0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
    *Z1 = I2C_read(OPENCORES_I2C_0_BASE, 1);

}

unsigned int I2C_READ_ADXL345(__uint8_t reg) {

    __uint8_t data_out;    

    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 0);

    I2C_write(OPENCORES_I2C_0_BASE,reg,0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);
    data_out = I2C_read(OPENCORES_I2C_0_BASE, 1);

    return data_out;
}

void I2C_WRITE_ADXL345(__uint8_t reg, __uint8_t data_in) {  

    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 0);

    I2C_write(OPENCORES_I2C_0_BASE,reg,0);
    I2C_write(OPENCORES_I2C_0_BASE,data_in,1);
}

int main() {

    __uint8_t X0,X1,Y0,Y1,Z0,Z1 ; 
    __uint8_t OFSX, OFSY, OFSZ; 
    I2C_init(OPENCORES_I2C_0_BASE,ALT_CPU_CPU_FREQ,400000); // 

    OFSX = I2C_READ_ADXL345(0x1E);
    OFSY = I2C_READ_ADXL345(0x1F);
    OFSZ = I2C_READ_ADXL345(0x20);

    alt_printf("OFS : %x   %x   %x\n", OFSX, OFSY, OFSZ);

    usleep(100000);

    I2C_WRITE_ADXL345(0x1E, 0x0);
    I2C_WRITE_ADXL345(0x1F, 0x0);
    I2C_WRITE_ADXL345(0x20, 0x0);

    OFSX = I2C_READ_ADXL345(0x1E);
    OFSY = I2C_READ_ADXL345(0x1F);
    OFSZ = I2C_READ_ADXL345(0x20);

    alt_printf("OFS : %x   %x   %x\n", OFSX, OFSY, OFSZ);

    while(1) {
        // I2C_READ_BURSTMODE_ADXL345(&X0,&X1,&Y0,&Y1,&Z0,&Z1);
        // alt_printf("X : %x%x Y : %x%x Z : %x%x\n", X1,X0,Y1,Y0,Z1,Z0);
        // alt_printf("X : %x\n", X0);
        usleep(10000);
    }
    return 0;
}