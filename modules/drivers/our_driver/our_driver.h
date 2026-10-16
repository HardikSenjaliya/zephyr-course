#ifndef ZEPHYR_DRIVERS_OUR_DRIVER_H
#define ZEPHYR_DRIVERS_OUR_DRIVER_H

#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

struct our_driver_led_cfg {
    const struct gpio_dt_spec *led;
};

struct our_driver_data {
    size_t sample_count;
};

struct our_driver_api {
    struct sensor_driver_api sensor_api;
    int (*increment_sample_count)(const struct device *dev);
};

int our_driver_set_value(const struct device *dev, int value);


#endif // ZEPHYR_DRIVERS_OUR_DRIVER_H