#ifndef __LIGHT__LED_H__
#define __LIGHT__LED_H__

#include <Arduino.h>
#include <Adafruit_NeoPixel.h>

#define LIGHT_SENSOR_PIN GPIO_NUM_1 // A0
#define FOUR_LED_PIN GPIO_NUM_6 // D3

extern Adafruit_NeoPixel pixels3;

// Task to monitor light and affect LED: dark -> turn on LED, bright -> turn off LED
void lightAffectsLed(void *pvParameters);

#endif