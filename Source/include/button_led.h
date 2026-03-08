#ifndef __BUTTON_LED_H__
#define __BUTTON_LED_H__

#include "global.h"
#include <Arduino.h>

#define BUTTON1_PIN GPIO_NUM_8 // D5
#define BUTTON2_PIN GPIO_NUM_9 // D6

void doubleButton(void *pvParameters);

#endif