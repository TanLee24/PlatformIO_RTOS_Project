#include "light_led.h"

Adafruit_NeoPixel pixels3(4, FOUR_LED_PIN, NEO_GRB + NEO_KHZ800);

void lightAffectsLed(void *pvParameters)
{
    pixels3.begin();
    while(1) 
    {                          
        Serial.println("Light Affects LED");
        Serial.println(analogRead(LIGHT_SENSOR_PIN)); // A0
        if(analogRead(LIGHT_SENSOR_PIN) < 250)
        {
            pixels3.setPixelColor(0, pixels3.Color(127,0,127));
            pixels3.setPixelColor(1, pixels3.Color(127,0,127));
            pixels3.setPixelColor(2, pixels3.Color(127,0,127));
            pixels3.setPixelColor(3, pixels3.Color(127,0,127));
            pixels3.show();
        }
        if(analogRead(LIGHT_SENSOR_PIN) > 550)
        {
            pixels3.setPixelColor(0, pixels3.Color(0,0,0));
            pixels3.setPixelColor(1, pixels3.Color(0,0,0));
            pixels3.setPixelColor(2, pixels3.Color(0,0,0));
            pixels3.setPixelColor(3, pixels3.Color(0,0,0));
            pixels3.show();
        }
        vTaskDelay(1000);
    }
}