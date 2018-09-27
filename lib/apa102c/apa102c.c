#include <simba.h>
#include "apa102c.h"

#if CONFIG_APA102C == 1

int apa102c_module_init(void) {
    return (0);
}

int apa102c_init(struct apa102c_driver_t *self_p,
                 struct spi_device_t *spi_p,
                 uint32_t address) {
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(spi_p != NULL, EINVAL);

    self_p->address = address;

    // spi_init(&self_p->spi,
    //          spi_p,
    //          , //atmega
    //          SPI_MODE_MASTER,
    //          SPI_SPEED_250KBPS,
    //          0,
    //          0);

    return (0);
}

int apa102c_write(struct apa102c_driver_t *self_p) {

    return (0);
}


#endif
