#include <simba.h>
#include "apa102c.h"

#define LEDS_N 60

struct i2c_driver_t i2c_driver;
struct spi_driver_t spi_driver;

struct ds3231_driver_t rtc;
struct apa102c_driver_t led_driver;

struct rgb_t leds[LEDS_N];

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

    while (1) {
        // reset old led
        leds[date.second % LEDS_N].b = 0x00;

        // get new time
        ds3231_get_date(&rtc, &date);
        std_printf(FSTR("value = %d:%d:%d\n"), date.hour, date.minute, date.second);

        // set new led
        leds[date.second % LEDS_N].b = 0xFF;

        apa102c_show(&led_driver);
    }

    return (0);
}
