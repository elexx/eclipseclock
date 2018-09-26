#include <simba.h>

struct ds3231_driver_t rtc;
struct i2c_driver_t i2c_driver;

int main() {
	// boot
	sys_start();
	i2c_module_init();

	i2c_init(&i2c_driver, &i2c_0_dev, I2C_BAUDRATE_100KBPS, -1);
	i2c_start(&i2c_driver);

	ds3231_init(&rtc, &i2c_driver);


	// hello
	std_printf(sys_get_info());



	struct date_t date;

	while (1) {
		ds3231_get_date(&rtc, &date);
		std_printf(FSTR("value = %d:%d:%d\n"), date.hour, date.minute, date.second);

		thrd_sleep_ms(1000);
	}

	return 0;
}
