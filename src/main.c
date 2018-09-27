#include <simba.h>
#include "apa102c.h"

struct i2c_driver_t i2c_driver;
struct spi_driver_t spi_driver;

struct ds3231_driver_t rtc;
struct apa102c_driver_t leds;

int main() {
    // boot
    sys_start();

    i2c_init(&i2c_driver, &i2c_0_dev, I2C_BAUDRATE_100KBPS, -1);
    i2c_start(&i2c_driver);

    ds3231_init(&rtc, &i2c_driver);

    spi_init(&spi_driver, &spi_device[0], &pin_d10_dev, SPI_MODE_MASTER, SPI_SPEED_8MBPS, 0, 0);
    spi_start(&spi_driver);

    uint8_t startframe[] = {
        0x00, 0x00, 0x00, 0x00, //start sequence
    };
    uint8_t endframe[] = {
        0xFF, 0x00, 0x00, 0x00 // endsequence
    };

    // apa102c_init(&leds, &)


    // hello
    std_printf(sys_get_info());



    struct date_t date;

    while (1) {
        ds3231_get_date(&rtc, &date);
        std_printf(FSTR("value = %d:%d:%d\n"), date.hour, date.minute, date.second);


        uint8_t brightness = 0x1F;
        uint8_t red = 0x0F;
        uint8_t green = 0x0F;
        uint8_t blue = date.second << 2;

        uint8_t buf[] = {
            0xE0 | brightness, blue, green, red, // 111 xxxxx, blue, green, red
        };

        spi_write(&spi_driver, startframe, 4);
        spi_write(&spi_driver, buf, 4);


        // endframe for nLeds
        uint16_t nLeds = 1;
        uint16_t nDWords = nLeds / 32;
        do { spi_write(&spi_driver, endframe, 4); } while(nDWords--);


        thrd_sleep_ms(1000);
    }

    return (0);
}
