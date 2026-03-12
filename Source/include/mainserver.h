#ifndef ___MAIN_SERVER__
#define ___MAIN_SERVER__

#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <Adafruit_NeoPixel.h>
#include "global.h"

#define LED1_PIN 48
#define NEO_PIN 45
#define NUM_PIXELS 1

//extern WebServer server;
//extern bool isAPMode;

String mainPage();
String settingsPage();

void startAP();
void setupServer();
// void connectToWiFi();

void main_server(void *pvParameters);

#endif