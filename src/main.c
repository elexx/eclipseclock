#include <simba.h>
#include "apa102c.h"
#include "fast_hsv2rgb.h"

#define LEDS_N 60
#define SECOND_PRESCALER (LEDS_N / 60)
#define MINUTE_PRESCALER (LEDS_N / 60)
#define HOUR_PRESCALER (LEDS_N / 12)


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

void map(struct hsv_t *out, uint8_t minute, struct hsv_t lastColor, struct hsv_t nextColor) {
    out->h = minute * (nextColor.h - lastColor.h) / 59 + lastColor.h;
    out->s = minute * (nextColor.s - lastColor.s) / 59 + lastColor.s;
    out->v = minute * (nextColor.v - lastColor.v) / 59 + lastColor.v;
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

    struct hsv_t currentColorHsv;
    currentColorHsv.h = 0;
    currentColorHsv.s = 0;
    currentColorHsv.v = 0;

    struct rgb_t currentColorRgb;

    for (;;) {
        // for (uint8_t i = 0; i < LEDS_N; i++) {
        //     leds[i].r = 0;
        //     leds[i].g = 0;
        //     leds[i].b = 0;
        // }
        memset(leds, 0, sizeof(struct rgb_t) * LEDS_N);

        map(&currentColorHsv, date.minute, gradient[date.hour % 12], gradient[(date.hour+1) % 12]);


        fast_hsv2rgb_8bit(
            currentColorHsv.h, currentColorHsv.s, currentColorHsv.v,
            &currentColorRgb.r, &currentColorRgb.g, &currentColorRgb.b);

        for (uint8_t i = 0; i < SECOND_PRESCALER; i++) {
            leds[date.second + i].r = currentColorRgb.r;
            leds[date.second + i].g = currentColorRgb.g;
            leds[date.second + i].b = currentColorRgb.b;
        }


        // if (date.second == 0) {
        //     std_printf(OSTR("%02u %04x %04x %04x \n"), date.minute, currentColorHsv.h, nextColor.h, hue);
        // }
        apa102c_show(&led_driver);


        thrd_sleep_ms(1);

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
