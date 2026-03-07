#include "soilMoisture_relay.h"

void soilMoistureAffectsPump(void *pvParameters) 
{

  pinMode(RELAY_PIN, OUTPUT); // D5

  while(1) 
  {                          
    Serial.println("Task Soild and Relay");
    
    int sensorValue = analogRead(SOIL_SENSOR_PIN); // A1
    // Send the sensor value to the Queue. 
    // xQueueOverwrite is used to ensure the queue always holds the MOST RECENT value.
    // If the queue is full, the old value is discarded.
    xQueueOverwrite(xQueueSoilMoisture, &sensorValue);
    
    Serial.print("Soil Value sent to Queue: ");
    Serial.println(sensorValue);
    
    if(sensorValue > 1500) // A0, wet soil -> deactivate pump
    {
      digitalWrite(GPIO_NUM_8, LOW);
    }
    if(sensorValue < 50) // A0, dry soil -> activate pump
    {
      digitalWrite(GPIO_NUM_8, HIGH);
    }
    vTaskDelay(1000);
  }
}