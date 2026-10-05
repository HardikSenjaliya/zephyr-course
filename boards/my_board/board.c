#include <zephyr/kernel.h>
#include <zephyr/drivers/gpio.h>

#define LED0_NODE DT_ALIAS(led0)

#if !DT_NODE_HAS_STATUS(LED0_NODE, okay)
#error "LED0 is not defined"
#endif

static const struct gpio_dt_spec led =
    GPIO_DT_SPEC_GET(LED0_NODE, gpios);

static int my_board_init(void)
{
    int ret;

    printk("Board Initialized\n");

    ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
    if (ret < 0) {
        return ret;
    }

    ret = gpio_pin_set_dt(&led, 1);
    if (ret < 0) {
        return ret;
    }

    return 0;
}

SYS_INIT(my_board_init, POST_KERNEL, 0);