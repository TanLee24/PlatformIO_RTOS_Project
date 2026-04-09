#ifndef __BUTTON_LED_H__
#define __BUTTON_LED_H__

#include "global.h"
#include <Arduino.h>

#define BUTTON1_PIN GPIO_NUM_10 // D7
#define BUTTON2_PIN GPIO_NUM_17 // D8

// Task pressing the Double button to config the status of the Single LED
void doubleButton(void *pvParameters);

#endif