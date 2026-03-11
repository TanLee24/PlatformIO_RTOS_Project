#ifndef __TASK_WIFI_H__
#define __TASK_WIFI_H__

#include <WiFi.h>
#include <task_check_info.h>
#include <task_webserver.h>

extern bool Wifi_reconnect();
extern void startAP();

// Connect to a Local Wi-Fi
void WiFi_Manager(void *pvParameters);

#endif