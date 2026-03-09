#ifndef __LED_BUTTON_H__
#define __LED_BUTTON_H__

#include "global.h"
#include <Arduino.h>

#define SINGLE_LED_PIN GPIO_NUM_6 // D3
#define LED_OFF 0
#define LED_BLINK 1

// Task blinking the Single LED based on the Button pressed
void ledBasedOnButton(void *pvParameters);

#endif