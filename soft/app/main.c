#include <unistd.h>
#include "system.h"
#include "altera_avalon_pio_regs.h"
#include "opencores_i2c.h"

int main() {

    __uint32_t X0 ; 
    I2C_init(OPENCORES_I2C_0_BASE,ALT_CPU_CPU_FREQ,400000); // 


    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 0);
    I2C_write(OPENCORES_I2C_0_BASE,0x32,0);
    I2C_start(OPENCORES_I2C_0_BASE, 0x1D, 1);

    X0 = I2C_read(OPENCORES_I2C_0_BASE, 0);

    alt_printf("%x\n", X0);

    return 0;
}


