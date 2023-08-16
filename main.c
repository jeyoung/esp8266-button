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
static unsigned int  pin_read(unsigned int  pin)
{
    return GPIO_INPUT_GET(pin);
}

/* Reads the system time
 */
static unsigned int  time_read()
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

// user_pre_init is required from SDK v3.0.0 onwards
// It is used to register the parition map with the SDK, primarily to allow
// the app to use the SDK's OTA capability.  We don't make use of that in 
// otb-iot and therefore the only info we provide is the mandatory stuff:
// - RF calibration data
// - Physical data
// - System parameter
// The location and length of these are from the 2A SDK getting started guide
void ICACHE_FLASH_ATTR user_pre_init(void)
{
  bool rc = false;
  static const partition_item_t part_table[] = 
  {
    {SYSTEM_PARTITION_RF_CAL,
     0x3fb000,
     0x1000},
    {SYSTEM_PARTITION_PHY_DATA,
     0x3fc000,
     0x1000},
    {SYSTEM_PARTITION_SYSTEM_PARAMETER,
     0x3fd000,
     0x3000},
  };

  // This isn't an ideal approach but there's not much point moving on unless
  // or until this has succeeded cos otherwise the SDK will just barf and 
  // refuse to call user_init()
  while (!rc)
  {
    rc = system_partition_table_regist(part_table,
				       sizeof(part_table)/sizeof(part_table[0]),
                                       4);
  }

  return;
}
