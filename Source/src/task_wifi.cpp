#include "task_wifi.h"

void startAP()
{
    WiFi.mode(WIFI_AP);
    WiFi.softAP(String(AP_SSID), String(AP_PASS));
    Serial.print("AP IP: ");
    Serial.println(WiFi.softAPIP());
}

void startSTA()
{
    if (STA_SSID.isEmpty())
    {
        vTaskDelete(NULL);
    }

    WiFi.mode(WIFI_STA);

    if (STA_PASS.isEmpty())
    {
        WiFi.begin(STA_SSID.c_str());
    }
    else
    {
        WiFi.begin(STA_SSID.c_str(), STA_PASS.c_str());
    }

    while (WiFi.status() != WL_CONNECTED)
    {
        vTaskDelay(100 / portTICK_PERIOD_MS);
    }
    Serial.println("Your Wi-Fi IP address: ");
    Serial.println(WiFi.localIP());
    //Give a semaphore here
    xSemaphoreGive(xBinarySemaphoreInternet);
}

bool Wifi_reconnect()
{
    const wl_status_t status = WiFi.status();
    if (status == WL_CONNECTED)
    {
        return true;
    }
    startSTA();
    return false;
}

void wifiConnect(void *pvParameters) 
{
    Serial.println("Connecting to Wi-Fi...");
    
    // startSTA();
    // startAP();

    while (1)
    {
        Wifi_reconnect();
        vTaskDelay(5000);
    }
}