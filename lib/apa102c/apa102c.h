
#ifndef __DRIVERS_APA102C_H__
#define __DRIVERS_APA102C_H__

#include <simba.h>

struct apa102c_driver_t {
    struct spi_driver_t *spi_p;
    struct rgb_t *rgb_p;
    uint16_t n_leds;
};

struct rgb_t {
    union {
        struct {
            uint8_t b;
            uint8_t g;
            uint8_t r;
        };
        uint8_t raw[3];
    };
};


int apa102c_module_init(void);

int apa102c_init(struct apa102c_driver_t *self_p,
                 struct spi_driver_t *spi_p,
                 struct rgb_t *rgb_p,
                 uint16_t n_leds);

int apa102c_show(struct apa102c_driver_t *self_p);


#endif
