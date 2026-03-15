#include "global.h"

float glob_temperature = 0;
float glob_humidity = 0;
int glob_ml_state = 0;

// AP Mode Username and Password
String AP_SSID = "ESP32-S3 Local";
String AP_PASS = "12345678";
// STA Mode Username and Password
String STA_SSID = "BuiThiNgocKieu";
String STA_PASS = "15051971";

String CORE_IOT_TOKEN;
String CORE_IOT_SERVER;
String CORE_IOT_PORT;

// AP Mode
String ssid = "ESP32-YOUR NETWORK HERE!!!";
String password = "12345678";
//STA Mode
String wifi_ssid = "abcde";
String wifi_password = "123456789";
boolean isWifiConnected = false;

SemaphoreHandle_t xBinarySemaphoreInternet = xSemaphoreCreateBinary();
// Initialize queue with length 1 (Mailbox style) to store the latest soil moisture value
QueueHandle_t xQueueSoilMoisture = xQueueCreate(1, sizeof(int));
// Queue with max 5 elements
QueueHandle_t xButtonQueue = xQueueCreate(5, sizeof(int));