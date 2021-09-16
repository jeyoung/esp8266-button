#include <stdio.h>
#include <stdint.h>

#include "mem.h"
#include "osapi.h"
#include "user_interface.h"
#include "uart.h"

#include "main.h"
#include "button.h"

#define BUTTON_PIN 2
#define LED_PIN 0

static os_timer_t os_timer;

/* Represents a button
 */
static struct Button button = {0};

/* This program debounces button presses.
 */

static volatile uint16_t bounces = 0xFFFF;

/* Reads the state of the specified GPIO pin
 */
static uint32_t pin_read(uint32_t pin)
{
    return GPIO_INPUT_GET(pin);
}

/* Reads the system time
 */
static uint32_t time_read()
{
    return system_get_time();
}

/* Main timer tick handler
 */
static void main_on_timer(void *arg)
{
    Button_read(&button);
    os_printf("Bounces %d\n", button.bounces);
    if (button.longpress)
	os_printf("Long press\n");
    GPIO_OUTPUT_SET(LED_PIN, button.longpress && button.up);
    os_timer_arm(&os_timer, 25, 0);
}

/* User-defined intialisation
 */
void ICACHE_FLASH_ATTR user_init(void)
{
    uart_init(BIT_RATE_921600, BIT_RATE_921600);
    
    gpio_init();
    GPIO_DIS_OUTPUT(BUTTON_PIN);

    /* Initialise a button, setting the GPIO pin, the pin reading function, and
     * the time reading function
     */
    Button_init(&button, BUTTON_PIN, pin_read, time_read);

    os_timer_disarm(&os_timer);
    os_timer_setfn(&os_timer, &main_on_timer, (void *)NULL);
    os_timer_arm(&os_timer, 25, 0);
}
