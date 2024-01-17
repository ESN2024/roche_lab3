#include <unistd.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "opencores_i2c.h"
#include <stdint.h>

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

void ADXL345_SET_OFFSET_0() { 
    I2C_WRITE_ADXL345(0x1E, 0x0);
    I2C_WRITE_ADXL345(0x1F, 0x0);
    I2C_WRITE_ADXL345(0x20, 0x0);
}

void I2C_READ_BURSTMODE_ADXL345(__uint8_t *X0, __uint8_t *X1, __uint8_t *Y0, __uint8_t *Y1, __uint8_t *Z0, __uint8_t *Z1) {

    *X0 = I2C_READ_ADXL345(0x32);
    *X1 = I2C_READ_ADXL345(0x33);
    *Y0 = I2C_READ_ADXL345(0x34);
    *Y1 = I2C_READ_ADXL345(0x35);
    *Z0 = I2C_READ_ADXL345(0x36);
    *Z1 = I2C_READ_ADXL345(0x37);
}



int main() {

    __uint8_t X0,X1,Y0,Y1,Z0,Z1 ; 
    __uint16_t X, Y, Z;
    __uint8_t OFSX, OFSY, OFSZ; 
    I2C_init(OPENCORES_I2C_0_BASE,ALT_CPU_CPU_FREQ,400000); // 

    I2C_WRITE_ADXL345(0x1E, 0x0);
    I2C_WRITE_ADXL345(0x1F, 0x05);
    I2C_WRITE_ADXL345(0x20, 0xE5);

    I2C_WRITE_ADXL345(0x31, 0x7); //full range

    OFSX = I2C_READ_ADXL345(0x1E);
    OFSY = I2C_READ_ADXL345(0x1F);
    OFSZ = I2C_READ_ADXL345(0x20);

    // alt_printf("OFS : %x   %x   %x\n", OFSX, OFSY, OFSZ);

    usleep(500000);

    while(1) {
        I2C_READ_BURSTMODE_ADXL345(&X0,&X1,&Y0,&Y1,&Z0,&Z1);
        X = X1 << 8;
        X = X + X0;

        Y = Y1 << 8;
        Y = Y + Y0;

        Z = Z1 << 8;
        Z = Z + Z0;

        // alt_printf("X : %x.%x Y : %x.%x Z : %x.%x\n", X1,X0,Y1,Y0,Z1,Z0);
        // alt_printf("X : %x\n", OFSZ);
        alt_printf("X : %x Y : %x Z : %x\n", X, Y, Z);
        usleep(10000);
    }
    return 0;
}