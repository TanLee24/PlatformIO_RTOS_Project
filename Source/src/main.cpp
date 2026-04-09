#include "global.h"

#include "led_blinky.h"
#include "neo_blinky.h"
#include "light_led.h"
#include "soilMoisture_relay.h"
#include "temp_humi_monitor.h"
#include "button_led.h"
#include "led_button.h"
#include "analog_read_uart.h"
#include "task_wifi.h"
#include "mainserver.h"
#include "coreiot.h"
#include "tinyml.h"
#include "power_optimization.h"

void setup()
{
  Serial.begin(115200);

  // xTaskCreate(led_blinky, "Task LED Blinky", 4096, NULL, 2, NULL);
  // xTaskCreate(neo_blinky, "Task NEO Blinky", 4096, NULL, 2, NULL);
  xTaskCreate(lightAffectsLed, "Task Light Affects LED", 4096, NULL, 2, NULL);
  xTaskCreate(soilMoistureAffectsPump, "Task Soil Moisture Affects Pump", 4096, NULL, 2, NULL);
  xTaskCreate(temp_humi_monitor, "Task TEMP HUMI Monitor", 4096, NULL, 2, NULL);
  xTaskCreate(doubleButton, "Task Press The Double Button", 4096, NULL, 2, NULL);
  xTaskCreate(ledBasedOnButton, "Task Led Blinky Based on Button", 4096, NULL, 2, NULL);
  xTaskCreate(wifiConnect, "Task Connect Wi-Fi", 4096, NULL, 2, NULL);
  xTaskCreate(coreiot_task, "Task CoreIOT", 4096, NULL, 2, NULL);
  // xTaskCreate(main_server, "Task Web Server", 8192 , NULL, 2, NULL);
  xTaskCreate(tinyML, "Task Tiny ML", 4096, NULL, 2, NULL);
  // xTaskCreate(AnalogRead_UART, "Task Read Analog and Print UART", 4096, NULL, 2, NULL);
  // xTaskCreate(powerOptimization, "Task Power Optimization", 4096, NULL, 2, NULL);
}

void loop() {}