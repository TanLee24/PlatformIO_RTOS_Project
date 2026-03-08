#include "led_button.h"

void ledBasedOnButton(void *pvParameters)
{

    // pinMode(SINGLE_LED_PIN, OUTPUT);
    // digitalWrite(SINGLE_LED_PIN, LOW);
    
    // int receivedCommand = 0; 

    // while(1) 
    // {                        
    //     // Wait for the signal of the Queue
    //     if (xQueueReceive(xButtonQueue, &receivedCommand, portMAX_DELAY) == pdPASS) 
    //     {
    //         if (receivedCommand == 1) // Button A was pressed
    //         {
    //             Serial.println("Task LED: Received Command 1 -> Turn on LED");
    //         } 
    //         else if (receivedCommand == 2) // Button B was pressed
    //         {
    //             Serial.println("Task LED: Received Command 2 -> Turn off LED");
    //             digitalWrite(SINGLE_LED_PIN, LOW);
    //         }
    //     }
    // }

    pinMode(SINGLE_LED_PIN, OUTPUT);
    digitalWrite(SINGLE_LED_PIN, LOW);
    
    int receivedCommand = 0;
    int LedStatus = LED_OFF;
    bool isOn = false;

    while(1)
    {
        // Decide waiting time (Timeout) based on current state
        TickType_t waitTime = (LedStatus == LED_BLINK) ? (1000) : portMAX_DELAY; // portMAX_DELAY: wait forever

        // Event listening: Button pressed (pdPASS) or Time out (pdFALSE/Timeout)
        BaseType_t eventStatus = xQueueReceive(xButtonQueue, &receivedCommand, waitTime);

        switch(LedStatus)
        {
            case LED_OFF:
                if (eventStatus == pdPASS && receivedCommand == 1)
                {
                    Serial.println("OFF -> BLINK");
                    LedStatus = LED_BLINK;
                }
                break;
            case LED_BLINK:
                if (eventStatus == pdPASS)
                {
                    if (receivedCommand == 2)
                    {
                        Serial.println("BLINK -> OFF");
                        LedStatus = LED_OFF;
                        isOn = false;
                        digitalWrite(SINGLE_LED_PIN, LOW);
                    }
                }
                else
                {
                    isOn = !isOn;
                    digitalWrite(SINGLE_LED_PIN, isOn ? HIGH : LOW);
                }
                break;
            default:
                LedStatus = LED_OFF;
                break;
        }
    }

}