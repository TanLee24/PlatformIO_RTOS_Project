#ifndef __ANALOG_READ_UART_H__
#define __ANALOG_READ_UART_H__

#include <Arduino.h>
#include "driver/uart.h"

#define SENSOR_PIN GPIO_NUM_3  // ADC Pin A2
#define UART_NUM UART_NUM_0
#define TX_PIN 43
#define RX_PIN 44

// Task to read Analog signal and print UART
void AnalogRead_UART(void *pvParameters);

#endif