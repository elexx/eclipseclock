#include <simba.h>
#include "apa102c.h"
#include "fast_hsv2rgb.h"

#define LEDS_N 1


struct hsv_t {
    uint16_t h;
    uint8_t s;
    uint8_t v;
};


struct i2c_driver_t i2c_driver;
struct spi_driver_t spi_driver;

struct ds3231_driver_t rtc;
struct apa102c_driver_t led_driver;

struct rgb_t leds[LEDS_N];

struct hsv_t gradient[] = {
    {HSV_HUE_MAX * 0 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 1 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 2 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 3 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 4 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 5 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 6 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 7 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 8 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 9 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 10 / 11, 0xFF, 0xFF},
    {HSV_HUE_MAX * 11 / 11, 0xFF, 0xFF}
};



uint16_t map(uint8_t x, uint16_t out_min, uint16_t out_max) {
    return x * (out_max - out_min) / 59 + out_min;
}

int main() {
    // boot
    sys_start();

    // clock setup
    i2c_init(&i2c_driver, &i2c_0_dev, I2C_BAUDRATE_100KBPS, -1);
    i2c_start(&i2c_driver);
    ds3231_init(&rtc, &i2c_driver);

    // led setup
    spi_init(&spi_driver, &spi_device[0], &pin_d10_dev, SPI_MODE_MASTER, SPI_SPEED_8MBPS, 0, 0);
    spi_start(&spi_driver);
    apa102c_init(&led_driver, &spi_driver, leds, LEDS_N);

    // hello
    std_printf(sys_get_info());


    struct date_t date;
    date.hour = 0;
    date.minute = 0;
    date.second = 0;

    for (;;) {
        struct hsv_t currentColor = gradient[date.hour % 12];
        struct hsv_t nextColor = gradient[(date.hour+1) % 12];

        uint16_t hue = map(date.minute, currentColor.h, nextColor.h);
        uint8_t sat = map(date.minute, currentColor.s, nextColor.s);
        uint8_t val = map(date.minute, currentColor.v, nextColor.v);

        // if (date.second == 0) {
        //     std_printf(OSTR("%02u %04x %04x %04x \n"), date.minute, currentColor.h, nextColor.h, hue);
        // }

        fast_hsv2rgb_8bit(hue, sat, val, &leds[0].r, &leds[0].g, &leds[0].b);
        apa102c_show(&led_driver);

        thrd_sleep_ms(2);

        // use a fake clock for now
        // ds3231_get_date(&rtc, &date);
        date.second++;
        if (date.second == 60) {
            date.second = 0;
            date.minute++;
            if (date.minute == 60) {
                date.minute = 0;
                date.hour++;
                if (date.hour == 24) {
                    date.hour = 0;
                }
            }
        }
    }


    // for (;;) {
    //     memset(leds, 0, sizeof(struct rgb_t) * LEDS_N);

    //     // reset old led
    //     // leds[date.second % LEDS_N].b = 0x00;

    //     // get new time
    //     ds3231_get_date(&rtc, &date);
    //     std_printf(FSTR("value = %d:%d:%d\n"), date.hour, date.minute, date.second);

    //     // set new led
    //     leds[date.second % LEDS_N].b = 0xFF;

    //     apa102c_show(&led_driver);
    // }

    return (0);
}
