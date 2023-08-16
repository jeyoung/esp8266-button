#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>

/* Forward declaration of function to read the state of a GPIO
 */
typedef unsigned int (*Button_state_reader)(unsigned int);

/* Forward declaration of function to read the system time
 */
typedef unsigned int (*Button_time_reader)();

/* Represents a button
 */
struct Button
{
    /* The timestamp of the last button press
     */
    volatile unsigned int timestamp;

    /* Indicates whether a button press is released
     */
    volatile unsigned int up;

    /* Indicates whether a long button press is detected
     */
    volatile unsigned int longpress;

    /* Indicates whether a button press is detected
     */
    volatile unsigned int down;

    /* The GPIO pin linked to this button
     */
    volatile unsigned int pin;

    /* The number of bounces when the button is pressed
     */
    volatile unsigned int bounces;

    /* The function used for reading the state of the button
     */
    Button_state_reader state_reader;

    /* The function used for reading the system time
     */
    Button_time_reader time_reader;
};

/* Initialises a button
 */
void Button_init(struct Button *button, unsigned int pin, Button_state_reader state_reader, Button_time_reader time_reader);

/* Reads the state of a button
 */
void Button_read(struct Button *button);

#endif
