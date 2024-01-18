#include <stdio.h>
#include "system.h"
#include <sys/alt_irq.h>
#include <io.h>
#include <alt_types.h>
#include "sys/alt_sys_init.h"
#include "altera_avalon_pio_regs.h"
#include "altera_avalon_timer_regs.h"
#include "altera_avalon_timer.h"
#include "opencores_i2c.h"
#include <stdint.h>

__uint8_t I2C_READ_ADXL345(__uint8_t reg) {

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

__int16_t complement_2(__int16_t number, __int16_t *negatif){
    if (number & 0x8000) {
        number = -(0xFFFF) - number + 1;
        *negatif = 1;
    } else {
        *negatif = 0; //controle le signe
    }
    return number;
}


int main() {

    __uint8_t X0,X1,Y0,Y1,Z0,Z1,m,u,d,c;
    __int16_t X, Y, Z;
    __uint8_t OFSX, OFSY, OFSZ; 
    I2C_init(OPENCORES_I2C_0_BASE,ALT_CPU_CPU_FREQ,400000); // 

    I2C_WRITE_ADXL345(0x1E, 0x0);
    I2C_WRITE_ADXL345(0x1F, 0x05);
    I2C_WRITE_ADXL345(0x20, 0xCD);

    I2C_WRITE_ADXL345(0x31, 0x7); //full range

    OFSX = I2C_READ_ADXL345(0x1E);
    OFSY = I2C_READ_ADXL345(0x1F);
    OFSZ = I2C_READ_ADXL345(0x20);

    // alt_printf("OFS : %x   %x   %x\n", OFSX, OFSY, OFSZ);

    usleep(500000);

    while(1) {
        I2C_READ_BURSTMODE_ADXL345(&X0,&X1,&Y0,&Y1,&Z0,&Z1);

        X = (X1 << 8) + X0;

        Y = (Y1 << 8) + Y0;

        Z = (Z1 << 8) + Z0;

        X = complement_2(X)*3.9; //permet d avoir un resultat en mg
        Y = complement_2(Y)*3.9;
        Z = complement_2(Z)*3.9;

            m = X / 1000;           
            c = (X / 100) % 10;     
            d = (X / 10) % 10;      
            u = X % 10;             

            if (negatif) {
                IOWR_ALTERA_AVALON_PIO_DATA(PIO_4_BASE, 0b1111); //mets le signe -
            } else {
                IOWR_ALTERA_AVALON_PIO_DATA(PIO_4_BASE, 0b1011); //affiche rien
            }

            IOWR_ALTERA_AVALON_PIO_DATA(PIO_0_BASE, u); 

            IOWR_ALTERA_AVALON_PIO_DATA(PIO_1_BASE, d);

            IOWR_ALTERA_AVALON_PIO_DATA(PIO_2_BASE, c);

            IOWR_ALTERA_AVALON_PIO_DATA(PIO_3_BASE, m);

        // alt_printf("X : %x.%x Y : %x.%x Z : %x.%x\n", X1,X0,Y1,Y0,Z1,Z0);
        // alt_printf("X : %x\n", OFSZ);
        alt_printf("X : %x Y : %x Z : %x\n", X, Y, Z);
        usleep(900000);
    }
    return 0;
}