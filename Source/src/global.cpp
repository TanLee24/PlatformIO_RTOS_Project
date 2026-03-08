#include "global.h"
float glob_temperature = 0;
float glob_humidity = 0;

String WIFI_SSID;
String WIFI_PASS;
String CORE_IOT_TOKEN;
String CORE_IOT_SERVER;
String CORE_IOT_PORT;

String ssid = "ESP32-YOUR NETWORK HERE!!!";
String password = "12345678";
String wifi_ssid = "abcde";
String wifi_password = "123456789";
boolean isWifiConnected = false;
SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();

// Initialize queue with length 1 (Mailbox style) to store the latest soil moisture value
QueueHandle_t xQueueSoilMoisture = xQueueCreate(1, sizeof(int));
// Queue with max 5 elements
QueueHandle_t xButtonQueue = xQueueCreate(5, sizeof(int));