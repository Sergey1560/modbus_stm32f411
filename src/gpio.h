#ifndef GPIO_H
#define GPIO_H

#include "common_defs.h"

#include "mb.h"
#include "mbport.h"


void led_init(void);
void btn_init(void);
uint8_t led_state(void);
uint8_t btn_state(void);

void led_change(uint8_t value);

#endif