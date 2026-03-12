#include "global.h"

#include "led_blinky.h"
#include "neo_blinky.h"
#include "light_led.h"
#include "soilMoisture_relay.h"
#include "temp_humi_monitor.h"
#include "button_led.h"
#include "led_button.h"
#include "analog_read_uart.h"

#include "mainserver.h"
// #include "tinyml.h"
#include "coreiot.h"

// include task
#include "task_check_info.h"
#include "task_toogle_boot.h"
#include "task_wifi.h"
#include "task_webserver.h"
#include "task_core_iot.h"


void setup()
{
  Serial.begin(115200);
  // check_info_File(0);

  xTaskCreate(led_blinky, "Task LED Blink", 4096, NULL, 2, NULL);
  // xTaskCreate(neo_blinky, "Task NEO Blink", 4096, NULL, 2, NULL);
  // xTaskCreate(lightAffectsLed, "Light Affects LED", 4096, NULL, 2, NULL);
  // xTaskCreate(soilMoistureAffectsPump, "Soil Moisture Affects Pump", 4096, NULL, 2, NULL);
  // xTaskCreate(temp_humi_monitor, "Task TEMP HUMI Monitor", 4096, NULL, 2, NULL);
  // xTaskCreate(doubleButton, "Task Press The Double Button", 4096, NULL, 2, NULL);
  // xTaskCreate(ledBasedOnButton, "Led Blink Based on Button", 4096, NULL, 2, NULL);
  // xTaskCreate(AnalogRead_UART, "Read Analog and Print UART", 4096, NULL, 2, NULL);
  xTaskCreate(WiFi_Manager, "WiFi_Task", 4096, NULL, 2, NULL);
  // xTaskCreate(coreiot_task, "CoreIOT Task", 4096, NULL, 2, NULL);
  // xTaskCreate(main_server_task, "Task Main Server" ,8192  ,NULL  ,2 , NULL);
  
  // xTaskCreate( tiny_ml_task, "Tiny ML Task" ,2048  ,NULL  ,2 , NULL);
  // xTaskCreate(Task_Toogle_BOOT, "Task_Toogle_BOOT", 4096, NULL, 2, NULL);
}

void loop()
{
  // if (check_info_File(1))
  // {
  //   if (!Wifi_reconnect())
  //   {
  //     Webserver_stop();
  //   }
  //   else
  //   {
  //     //CORE_IOT_reconnect();
  //   }
  // }
  // Webserver_reconnect();
}