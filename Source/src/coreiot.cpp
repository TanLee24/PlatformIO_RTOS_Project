#include "coreiot.h"

// ----------- CONFIGURE THESE! -----------
const char* coreIOT_Server = "app.coreiot.io";
const char* coreIOT_Token = "e6odipbkx0qh0pxaw1hb";   // Device Access Token
const int   mqttPort = 1883;
// ----------------------------------------

WiFiClient espClient;
PubSubClient client(espClient);


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

    } 
    else 
    {   
      Serial.println("Device turned OFF.");
      //TODO

    }
  } 
  else 
  {
    Serial.print("Unknown method: ");
    Serial.println(method);
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
  Serial.println(" Connected!");

  // client.setServer(CORE_IOT_SERVER.c_str(), CORE_IOT_PORT.toInt());
  // client.setCallback(callback);
}

void coreiot_task(void *pvParameters)
{
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
                        ",\"humidity\":" + String(glob_humidity) + "}";
      
      client.publish("v1/devices/me/telemetry", payload.c_str());
      
      Serial.println("Published payload: " + payload);
      vTaskDelay(3000);  // Publish every 10 seconds
    }
}



// #include <ArduinoHttpClient.h>
// const char* public_mqtt_broker = "broker.hivemq.com";
// const int public_mqtt_port = 1883;

// WiFiClient public_espClient;
// PubSubClient public_mqttClient(public_espClient);

// void coreiot_task(void *pvParameters) {
//     // 1. CHỜ CÓ INTERNET TỪ TASK 4
//     while(1) {
//         if (xSemaphoreTake(xBinarySemaphoreInternet, portMAX_DELAY)) {
//             // Lấy được cờ thì trả lại ngay để các Task khác (như CoreIoT) cũng được chạy
//             xSemaphoreGive(xBinarySemaphoreInternet); 
//             break;
//         }
//         vTaskDelay(pdMS_TO_TICKS(500));
//     }

//     Serial.println("\n====== BAT DAU DEMO HANDS-ON TASK 5 ======");

//     // 2. HTTP GET: LẤY DỮ LIỆU THỜI TIẾT TẠI KHU VỰC ĐẠI HỌC BÁCH KHOA
//     WiFiClient weatherClient;
//     HttpClient http = HttpClient(weatherClient, "api.open-meteo.com", 80);
    
//     Serial.println("[HTTP] Dang lay du lieu...");
    
//     http.get("/v1/forecast?latitude=10.772&longitude=106.658&current_weather=true");
    
//     int httpCode = http.responseStatusCode();
    
//     if (httpCode > 0) {
//         Serial.printf("[HTTP] Thanh cong, Ma loi: %d\n", httpCode);
//         String payload = http.responseBody(); // Lấy nội dung JSON
//         Serial.println(payload);
//     } else {
//         Serial.printf("[HTTP] That bai, Ma loi: %d\n", httpCode);
//     }
    
//     http.stop(); // Đóng kết nối

//     // 3. MQTT PUBLISH: GỬI HELLO WORLD LÊN PUBLIC BROKER
//     public_mqttClient.setServer(public_mqtt_broker, public_mqtt_port);
    
//     for (;;) {
//         if (WiFi.status() == WL_CONNECTED) {
//             // Kết nối lại nếu mất kết nối
//             if (!public_mqttClient.connected()) {
//                 Serial.println("[MQTT Public] Dang ket noi test.mosquitto.org...");
                
//                 // Dùng ID riêng để không bị trùng lặp với các thiết bị khác trên mạng public
//                 String clientId = "ESP32_Tan_" + String(random(0xffff), HEX);
                
//                 if (public_mqttClient.connect(clientId.c_str())) {
//                     Serial.println("[MQTT Public] Da ket noi thanh cong!");
//                 } else {
//                     Serial.printf("[MQTT Public] Loi ket noi: %d\n", public_mqttClient.state());
//                 }
//             }

//             // Gửi dữ liệu
//             if (public_mqttClient.connected()) {
//                 public_mqttClient.loop();
//                 String msg = "Hello World from ESP32-S3!";
                
//                 // Publish vào một topic test
//                 public_mqttClient.publish("yolo_uno/test_topic", msg.c_str());
//                 Serial.println("[MQTT Public] Da gui: " + msg);
//             }
//         }
        
//         // Lặp lại việc gửi MQTT mỗi 10 giây
//         vTaskDelay(pdMS_TO_TICKS(5000)); 
//     }
// }