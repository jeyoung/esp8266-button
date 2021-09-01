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

static struct Button button = {0};

/* This program debounces button presses.
 */

static volatile uint16_t bounces = 0xFFFF;

static uint32_t pin_read(uint32_t pin)
{
    return GPIO_INPUT_GET(pin);
}

static uint32_t time_read()
{
    return system_get_time();
}

static void main_on_timer(void *arg)
{
    Button_read(&button);
    GPIO_OUTPUT_SET(LED_PIN, button.longpress && button.up);
    os_timer_arm(&os_timer, 25, 0);
}

void ICACHE_FLASH_ATTR user_init(void)
{
    /* UART0 is the default debugging interface, so we must initialise UART
     * with the desired baud rate
     */
    uart_init(BIT_RATE_921600, BIT_RATE_921600);
    
    gpio_init();
    GPIO_DIS_OUTPUT(BUTTON_PIN);

    Button_init(&button, BUTTON_PIN, pin_read, time_read);

    os_timer_disarm(&os_timer);
    os_timer_setfn(&os_timer, &main_on_timer, (void *)NULL);
    os_timer_arm(&os_timer, 25, 0);
}
