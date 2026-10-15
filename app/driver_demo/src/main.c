#include <stdio.h>
#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>

static const struct device *led = DEVICE_DT_GET(DT_ALIAS(app_led));

int main(void)
{

    if (!device_is_ready(led)) {
        printk("Device not ready\n");
        return 0;
    }

    while (1) {
        sensor_channel_get(led, SENSOR_CHAN_ACCEL_XYZ, NULL);
        k_msleep(1000);
        sensor_sample_fetch(led);
        k_msleep(1000);
    }


    return 0;
}