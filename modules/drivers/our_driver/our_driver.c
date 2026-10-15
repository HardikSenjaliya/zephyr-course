#define DT_DRV_COMPAT our_driver

#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/sys/util.h>

#include "our_driver.h"

LOG_MODULE_REGISTER(our_driver);

static int our_driver_sample_fetch(const struct device *dev, enum sensor_channel chan) {
    const struct our_driver_led_cfg *config = dev->config;
	const struct gpio_dt_spec *led = config->led;

    int err = gpio_pin_set_dt(led, 1); // Turn on the LED to indicate sample fetch
    if (err) {
        LOG_ERR("Cannot set GPIO (err %d)", err);
        return err;
    }

    return 0; // Return 0 on success
}

static int our_driver_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val) {
    const struct our_driver_led_cfg *config = dev->config;
	const struct gpio_dt_spec *led = config->led;

    int err = gpio_pin_set_dt(led, 0); // Turn off the LED to indicate sample get
    if (err) {
        LOG_ERR("Cannot set GPIO (err %d)", err);
        return err;
    }
    
    return 0; // Return 0 on success
}

static int our_driver_init(const struct device *dev) {
    // Implement the initialization logic for the driver
    LOG_INF("Initializing our driver");
    
    const struct our_driver_led_cfg *config = dev->config;
    const struct gpio_dt_spec *led = config->led;
    
    int err;

    if (device_is_ready(led->port)) {
        err = gpio_pin_configure_dt(led, GPIO_OUTPUT_INACTIVE);
    if (err) {
        LOG_ERR("Cannot configure GPIO (err %d)", err);
    }
    } else {
        LOG_ERR("%s: GPIO device not ready", dev->name);
        err = -ENODEV;
    }

    return 0; // Return 0 on success
}

static DEVICE_API(sensor, our_driver_api) = { 
    .channel_get = our_driver_channel_get,
    .sample_fetch = our_driver_sample_fetch,
};

#define DEV_INST(inst)                                                  \
    static const struct gpio_dt_spec gpio_dt_spec_##inst[] = {          \
        GPIO_DT_SPEC_INST_GET_BY_IDX(inst, gpios, 0),                   \
    };                                                                  \
                                                                        \
    static const struct our_driver_led_cfg our_driver_cfg_##inst = {    \
        .led = gpio_dt_spec_##inst,                                     \
    };                                                                  \
                                                                        \
    DEVICE_DT_INST_DEFINE(inst,                                         \
                          our_driver_init,                              \
                          NULL,                                         \
                          NULL,                                         \
                          &our_driver_cfg_##inst,                       \
                          POST_KERNEL,                                  \
                          CONFIG_SENSOR_INIT_PRIORITY,                 \
                          &our_driver_api);

DT_INST_FOREACH_STATUS_OKAY(DEV_INST)