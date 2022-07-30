/*
 * Copyright (c) 2016 Intel Corporation
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <zephyr/drivers/gpio.h>

#define SLEEP_TIME_MS 500

#define LED0_NODE DT_ALIAS(led0)
#define SW0_NODE DT_ALIAS(sw0)

/*
 * A build error on this line means your board is unsupported.
 * See the sample documentation for information on how to fix this.
 */
static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED0_NODE, gpios);
static const struct gpio_dt_spec button = GPIO_DT_SPEC_GET_OR(SW0_NODE, gpios, {0});

static struct gpio_callback button_cb_data;

/*
 * Set up work queue
 */
#define WORKQ_STACK_SIZE 512
#define WORKQ_PRIO 5

K_THREAD_STACK_DEFINE(workq_stack, WORKQ_STACK_SIZE);
struct k_work_q workq;

static int counter = 0;

static void work_handler(struct k_work *item)
{
	printk("counter=%d\n", counter);
	counter++;
	gpio_pin_set_dt(&led, 1);
	k_msleep(500);
	gpio_pin_set_dt(&led, 0);

	k_free(item);
}

static void button_pressed(const struct device *dev, struct gpio_callback *cb,
		uint32_t pins)
{
	struct k_work *work = k_malloc(sizeof(struct k_work));
	k_work_init(work, work_handler);
	k_work_submit(work);
}

void main(void)
{
	int ret;

	printk("hello, starting up now!\n");

	if (!device_is_ready(button.port)) {
		printk("button not ready\n");
		return;
	}

	if (!device_is_ready(led.port)) {
		printk("led not ready\n");
		return;
	}

	// initialize LED, turn on to start
	ret = gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE);
	if (ret < 0) {
		return;
	}

	// initialize interrupt
	ret = gpio_pin_configure_dt(&button, GPIO_INPUT);
	if (ret < 0) {
		printk("couldn't configure button\n");
		return;
	}

	ret = gpio_pin_interrupt_configure_dt(&button, GPIO_INT_EDGE_TO_ACTIVE);
	if (ret < 0) {
		printk("couldn't configure button interrupt\n");
		return;
	}

	gpio_init_callback(&button_cb_data, button_pressed, BIT(button.pin));
	gpio_add_callback(button.port, &button_cb_data);

	// initialize work queue
	k_work_queue_init(&workq);
	k_work_queue_start(&workq, workq_stack,
			K_THREAD_STACK_SIZEOF(workq_stack),
			WORKQ_PRIO,
			NULL);


	while (1) {
		k_msleep(SLEEP_TIME_MS);
	}
}
