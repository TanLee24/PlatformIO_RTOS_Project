#include "button_led.h"

void doubleButton(void *pvParameters)
{
    // Input Pull-up (HIGH when default, LOW when button is pressed)
    pinMode(BUTTON1_PIN, INPUT_PULLUP);
    pinMode(BUTTON2_PIN, INPUT_PULLUP);
    
    bool lastState1 = HIGH;
    bool lastState2 = HIGH;

    while (1)
    {
        bool currState1 = digitalRead(BUTTON1_PIN);
        bool currState2 = digitalRead(BUTTON2_PIN);
        
        // --- Handling Button A Event ---
        if (lastState1 == HIGH && currState1 == LOW)
        {
            vTaskDelay(50); // Debounce
            if (digitalRead(BUTTON1_PIN) == LOW) 
            {
                int msg = 1; // 1 is the command of button A
                if (xQueueSend(xButtonQueue, &msg, 0) == pdPASS) 
                {
                    Serial.println("Task BUTTON: Button A Pressed!");
                }
            }
        }

        // --- Handling Button B Event ---
        if (lastState2 == HIGH && currState2 == LOW)
        {
            vTaskDelay(50); // Debounce
            if (digitalRead(BUTTON2_PIN) == LOW) 
            {
                int msg = 2; // 2 is the command of button B
                if (xQueueSend(xButtonQueue, &msg, 0) == pdPASS) 
                {
                    Serial.println("Task BUTTON: Button B Pressed!");
                }
            }
        }
        
        lastState1 = currState1;
        lastState2 = currState2;
        vTaskDelay(10);
    }
}