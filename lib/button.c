#include <stdint.h>
#include "button.h"

/* Initialises a button
 */
void Button_init(struct Button *button, uint32_t pin, Button_state_reader state_reader, Button_time_reader time_reader)
{
    button->timestamp = 0;
    button->up = 0;
    button->longpress = 0;
    button->down = 0;
    button->pin = pin;
    button->bounces = 0xFFFF;
    button->state_reader = state_reader;
    button->time_reader = time_reader;
}

/* Reads the state of a button
 */
void Button_read(struct Button *button)
{
    button->bounces = (button->bounces << 1) | (uint16_t)button->state_reader(button->pin);
    button->up = button->down && (button->bounces > 0xFF00);
    button->longpress = button->down && button->timestamp && (button->time_reader() - button->timestamp > 3000000);
    if (button->down) {
	if (!button->timestamp)
	    button->timestamp = button->time_reader();
    } else
	button->timestamp = 0;
    button->down = (button->bounces < 0xFF00);
}

