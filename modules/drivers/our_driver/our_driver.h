#ifndef ZEPHYR_DRIVERS_OUR_DRIVER_H
#define ZEPHYR_DRIVERS_OUR_DRIVER_H

#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>

struct our_driver_led_cfg {
    const struct gpio_dt_spec *led;
};

#endif // ZEPHYR_DRIVERS_OUR_DRIVER_H