#include <simba.h>
#include "apa102c.h"

#if CONFIG_APA102C == 1

const uint8_t startframe[] = {
    0x00, 0x00, 0x00, 0x00
};
const uint8_t endframe[] = {
    0xFF, 0x00, 0x00, 0x00
};

int apa102c_module_init(void) {
    return (0);
}

int apa102c_init(struct apa102c_driver_t *self_p,
                 struct spi_driver_t *spi_p,
                 struct rgb_t *rgb_p,
                 uint16_t n_leds) {
    ASSERTN(self_p != NULL, EINVAL);
    ASSERTN(spi_p != NULL, EINVAL);
    ASSERTN(rgb_p != NULL, EINVAL);

    self_p->spi_p = spi_p;
    self_p->rgb_p = rgb_p;
    self_p->n_leds = n_leds;

    for (uint16_t i = 0; i < n_leds; i++) {
        rgb_p[i].r = 0x00;
        rgb_p[i].g = 0x00;
        rgb_p[i].b = 0x00;
    }

    apa102c_show(self_p);

    return (0);
}

int apa102c_show(struct apa102c_driver_t *self_p) {
    spi_select(self_p->spi_p);
    spi_write(self_p->spi_p, startframe, 4);

    struct rgb_t *leds = self_p->rgb_p;
    while (leds < self_p->rgb_p + self_p->n_leds) {
        spi_put(self_p->spi_p, 0xE0 | 0x1F); // Mask + highest Value
        spi_write(self_p->spi_p, &leds->raw, 3);
        leds++;
    }

    // write end frames
    uint16_t nDWords = self_p->n_leds / 32;
    do { spi_write(self_p->spi_p, endframe, 4); } while(nDWords--);

    spi_deselect(self_p->spi_p);
    return (0);
}


#endif
