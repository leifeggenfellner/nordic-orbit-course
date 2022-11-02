/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <zephyr/zephyr.h>
#include <logging/log.h>
#include <dk_buttons_and_leds.h>
#include "mpu_sensor.h"

#define RUN_STATUS_LED DK_LED1
#define RUN_LED_BLINK_INTERVAL 1000

#define LOG_MODULE_NAME_APP app
LOG_MODULE_REGISTER(LOG_MODULE_NAME_APP);

/* BLINK LEDS */
// void blink_run_led(uint8_t *blink_state)
// {
// 	for (;;)
// 	{
// 		*blink_state = !(*blink_state);
// 		dk_set_led(RUN_STATUS_LED, (*blink_state));
// 		k_sleep(K_MSEC(RUN_LED_BLINK_INTERVAL));
// 	}
// }

/* CALLBACKS */
void button_handler(uint32_t button_state, uint32_t change_detected)
{
	uint8_t button_pressed = 0;
	if (change_detected & button_state)
	{
		switch (change_detected)
		{
		case DK_BTN1_MSK:
			button_pressed = 1;
			dk_set_led(DK_LED1, 1);
			k_sleep(K_MSEC(100));
			dk_set_led(DK_LED1, 0);
			break;
		case DK_BTN2_MSK:
			button_pressed = 2;
			dk_set_led(DK_LED2, 1);
			k_sleep(K_MSEC(100));
			dk_set_led(DK_LED2, 0);
			break;
		case DK_BTN3_MSK:
			button_pressed = 3;
			dk_set_led(DK_LED3, 1);
			k_sleep(K_MSEC(100));
			dk_set_led(DK_LED3, 0);
			break;
		case DK_BTN4_MSK:
			button_pressed = 4;
			dk_set_led(DK_LED4, 1);
			k_sleep(K_MSEC(100));
			dk_set_led(DK_LED4, 0);
			break;
		default:
			break;
		}
		LOG_INF("Button %d pressed", button_pressed);
	}
}

/* CONFIGURATION */
static void configure_dk_button_leds(void)
{
	int err;

	err = dk_leds_init();

	if (err)
	{
		LOG_ERR("Couldn't init LEDS (err %d)", err);
	}

	err = dk_buttons_init(button_handler);

	if (err)
	{
		LOG_ERR("Couldn't init buttons (err %d)", err);
	}

	err = mpu_sensor_init();

	if (err)
	{
		LOG_ERR("Couldn't init MPU (err %d)", err);
	}
}

void main(void)
{
	LOG_INF("Hello World! %s\n", CONFIG_BOARD);

	configure_dk_button_leds();

	// uint8_t blink_state = 0;

	// blink_run_led(&blink_state);
}