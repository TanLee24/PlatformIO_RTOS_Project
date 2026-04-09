#include "coreiot.h"

// ----------- CONFIGURE THESE! -----------
const char* coreIOT_Server = "app.coreiot.io";
const char* coreIOT_Token = "e6odipbkx0qh0pxaw1hb";   // Device Access Token
const int   mqttPort = 1883;
// ----------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);

void callback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.println("] ");

  // Allocate a temporary buffer for the message
  char message[length + 1];
  memcpy(message, payload, length);
  message[length] = '\0';
  Serial.print("Payload: ");
  Serial.println(message);

  // Parse JSON
  StaticJsonDocument<256> doc;
  DeserializationError error = deserializeJson(doc, message);

  if (error) 
  {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
  }

  const char* method = doc["method"];
  if (strcmp(method, "POWER") == 0) 
  {
    // Check params type (could be boolean, int, or string according to your RPC)
    // Example: {"method": "setValueLED", "params": "ON"}
    const char* params = doc["params"];

    if (strcmp(params, "ON") == 0) 
    {
      Serial.println("Device turned ON.");
      //TODO
      digitalWrite(LED_GPIO, HIGH);

    } 
    else 
    {   
      Serial.println("Device turned OFF.");
      //TODO
      digitalWrite(LED_GPIO, LOW);
    }
  } 
  else 
  {
    Serial.print("Unknown method: ");
    Serial.println(method);
  }
}

void reconnect() 
{
  client.setServer(coreIOT_Server, mqttPort);
  client.setCallback(callback);
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("ESP32_ClientID", coreIOT_Token, NULL)) 
    {
        
      Serial.println("connected to CoreIOT Server!");
      client.subscribe("v1/devices/me/rpc/request/+");
      Serial.println("Subscribed to v1/devices/me/rpc/request/+");

    } 
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 3 seconds");
      delay(3000);
    }
  }
}

void setup_coreiot()
{
  while(1)
  {
    if (xSemaphoreTake(xBinarySemaphoreInternet, portMAX_DELAY)) 
    {
      break;
    }
    delay(500);
    Serial.print(".");
  }
  reconnect();
  Serial.println("Connected!");
}

void coreiot_task(void *pvParameters)
{
  pinMode(LED_GPIO, OUTPUT);
  digitalWrite(LED_GPIO, LOW);

  // Home
  // String latitude = "10.790201567988808";
  // String longitude = "106.75724545406152";

  // BK CS1
  String latitude = "10.772391155216926";
  String longitude = "106.65788031863018";

  setup_coreiot();
  
  while(1)
  {
    if (!client.connected()) 
    {
      reconnect();
    }
    client.loop();
    // Sample payload, publish to 'v1/devices/me/telemetry'
    String payload = "{\"temperature\":" + String(glob_temperature) +  
                      ",\"humidity\":" + String(glob_humidity) + 
                      ",\"longitude\":" + longitude +
                      ",\"latitude\":" + latitude + "}";
    
    client.publish("v1/devices/me/telemetry", payload.c_str());
    
    Serial.println("Published payload: " + payload);
    
    vTaskDelay(3000);  // Publish every 3 seconds
  }
}