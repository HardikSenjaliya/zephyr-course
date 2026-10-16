#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/shell/shell.h>
#include <zephyr/sys/printk.h>
#include <errno.h>

#include "our_driver.h"

static const struct device *led = DEVICE_DT_GET(DT_ALIAS(app_led));

#define SENSOR_VALUE_MIN 0
#define SENSOR_VALUE_MAX 1

static int cmd_sensor_set(const struct shell *sh,
                          size_t argc, char **argv)
{
    if (argc != 2) {
        shell_error(sh, "Usage: sensor set <value>");
        return -EINVAL;
    }

    char *endptr;
    errno = 0;
    long value = strtol(argv[1], &endptr, 10);

    if (errno != 0 || endptr == argv[1] || *endptr != '\0') {
        shell_error(sh, "Value must be an integer");
        return -EINVAL;
    }

    if (value < SENSOR_VALUE_MIN || value > SENSOR_VALUE_MAX) {
        shell_error(sh, "Value must be between %d and %d",
                    SENSOR_VALUE_MIN, SENSOR_VALUE_MAX);
        return -EINVAL;
    }

    if (!device_is_ready(led)) {
        shell_error(sh, "Sensor device is not ready");
        return -ENODEV;
    }

    int ret = our_driver_set_value(led, (int)value);

    if (ret < 0) {
        shell_error(sh, "Failed to set value: %d", ret);
        return ret;
    }

    shell_print(sh, "Value set to %ld", value);
    return 0;
}

/* sensor fetch */
static int cmd_sensor_fetch(const struct shell *sh,
                            size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (!device_is_ready(led)) {
        shell_error(sh, "Sensor device is not ready");
        return -ENODEV;
    }

    int ret = sensor_sample_fetch(led);

    if (ret < 0) {
        shell_error(sh, "Sample fetch failed: %d", ret);
        return ret;
    }

    shell_print(sh, "Sample fetched successfully");
    return 0;
}

/* sensor read */
static int cmd_sensor_read(const struct shell *sh,
                           size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    if (!device_is_ready(led)) {
        shell_error(sh, "Sensor device is not ready");
        return -ENODEV;
    }

    struct sensor_value val;

    int ret = sensor_channel_get(led,
                                 SENSOR_CHAN_ACCEL_XYZ,
                                 &val);

    if (ret < 0) {
        shell_error(sh, "Channel read failed: %d", ret);
        return ret;
    }

    shell_print(sh, "%u", val.val1);

    return 0;
}

/* sensor info */
static int cmd_sensor_info(const struct shell *sh,
                           size_t argc, char **argv)
{
    ARG_UNUSED(argc);
    ARG_UNUSED(argv);

    shell_print(sh, "Device name: %s", led->name);
    shell_print(sh, "Ready: %s",
                device_is_ready(led) ? "yes" : "no");

    return 0;
}

/* Define subcommands */
SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcmds,
    SHELL_CMD(fetch, NULL,
              "Fetch a sensor sample",
              cmd_sensor_fetch),
    SHELL_CMD(read, NULL,
              "Read the XYZ sensor channel",
              cmd_sensor_read),
    SHELL_CMD(info, NULL,
              "Show device name and ready state",
              cmd_sensor_info),
    SHELL_CMD_ARG(set, NULL,
                  "Set a value (0 or 1)",
                  cmd_sensor_set, 2, 0),
    SHELL_SUBCMD_SET_END
);

/* Register root command */
SHELL_CMD_REGISTER(sensor, &sensor_subcmds,
                   "Sensor commands", NULL);
