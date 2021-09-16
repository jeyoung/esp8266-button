#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>

/* Forward declaration of function to read the state of a GPIO
 */
typedef uint32_t (*Button_state_reader)(uint32_t);

/* Forward declaration of function to read the system time
 */
typedef uint32_t (*Button_time_reader)();

/* Represents a button
 */
struct Button
{
    /* The timestamp of the last button press
     */
    volatile uint32_t timestamp;

    /* Indicates whether a button press is released
     */
    volatile uint32_t up;

    /* Indicates whether a long button press is detected
     */
    volatile uint32_t longpress;

    /* Indicates whether a button press is detected
     */
    volatile uint32_t down;

    /* The GPIO pin linked to this button
     */
    volatile uint32_t pin;

    /* The number of bounces when the button is pressed
     */
    volatile uint16_t bounces;

    /* The function used for reading the state of the button
     */
    Button_state_reader state_reader;

    /* The function used for reading the system time
     */
    Button_time_reader time_reader;
};

/* Initialises a button
 */
void Button_init(struct Button *button,uint32_t pin, Button_state_reader state_reader, Button_time_reader time_reader);

/* Reads the state of a button
 */
void Button_read(struct Button *button);

#endif
