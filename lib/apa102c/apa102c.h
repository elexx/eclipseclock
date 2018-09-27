
#ifndef __DRIVERS_APA102C_H__
#define __DRIVERS_APA102C_H__

#include <simba.h>

struct apa102c_driver_t {
    struct spi_driver_t spi;
    uint32_t address;
};


int apa102c_module_init(void);

int apa102c_init(struct apa102c_driver_t *self_p,
                 struct spi_device_t *spi_p,
                 uint32_t address);

int apa102c_write(struct apa102c_driver_t *self_p);


#endif
