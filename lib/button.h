#ifndef __BUTTON_H__
#define __BUTTON_H__

#include <stdint.h>

typedef uint32_t (*Button_state_reader)(uint32_t);

typedef uint32_t (*Button_time_reader)();

struct Button
{
    volatile uint32_t timestamp;
    volatile uint32_t up;
    volatile uint32_t longpress;
    volatile uint32_t down;
    volatile uint32_t pin;
    volatile uint16_t bounces;
    Button_state_reader state_reader;
    Button_time_reader time_reader;
};

void Button_init(struct Button *button,uint32_t pin, Button_state_reader state_reader, Button_time_reader time_reader);

void Button_read(struct Button *button);

#endif
