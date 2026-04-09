#include "mainserver.h"

Adafruit_NeoPixel pixels(NUM_PIXELS, NEO_PIN, NEO_GRB + NEO_KHZ800);
WebServer server(80);

// WiFi Configuration Variables
String sta_ssid = "";
String sta_pass = "";
bool isAPMode = true;
int retry_count = 0; 
IPAddress apIP(192, 168, 10, 1);
IPAddress gateway(192, 168, 10, 1);
IPAddress subnet(255, 255, 255, 0);

// ========== DEVICE STATE VARIABLES ==========
int led1_mode = 0; 
int led1_delay = 1000; 
unsigned long led1_last_ms = 0;
bool led1_state = false;

int neo_mode = 0; 
int neo_r = 255, neo_g = 255, neo_b = 255; 
int neo_blink_delay = 500; 
unsigned long neo_last_ms = 0;
bool neo_state = false;

// ========== WIFI EVENT HANDLER ==========
void WiFiEvent(WiFiEvent_t event) {
  switch(event) {
    case ARDUINO_EVENT_WIFI_STA_GOT_IP:
      Serial.print("\n[Event] CONNECTED SUCCESSFULLY! STA IP: ");
      Serial.println(WiFi.localIP());
      isAPMode = false; 
      retry_count = 0; 
      xSemaphoreGive(xBinarySemaphoreInternet);
      break;
    case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
      if (retry_count < 3) {
        retry_count++;
        WiFi.begin(sta_ssid.c_str(), sta_pass.c_str());
      } else {
        isAPMode = true; 
        WiFi.disconnect(true, false);
        WiFi.mode(WIFI_AP);
      }
      break;
  }
}

// ========== MAIN DASHBOARD UI (MODERN CLEAN) ==========
String mainPage() {
  return R"rawliteral(
  <!DOCTYPE html>
  <html lang="en">
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>ESP32 Web Server</title>
    <style>
      @import url('https://fonts.googleapis.com/css2?family=Inter:wght@400;600;800&display=swap');
      body { font-family: 'Inter', sans-serif; background: #f4f7f6; color: #1d1d1f; margin: 0; padding: 20px; display: flex; justify-content: center; }
      .dashboard { width: 100%; max-width: 800px; }
      
      h2 { text-align: center; color: #1d1d1f; font-weight: 800; letter-spacing: -0.5px; margin-bottom: 30px; }
      
      .card { background: #ffffff; padding: 25px; border-radius: 24px; box-shadow: 0 10px 30px rgba(0,0,0,0.04); margin-bottom: 25px; transition: 0.3s; }
      .card:hover { box-shadow: 0 15px 35px rgba(0,0,0,0.08); }
      .card-title { font-size: 16px; font-weight: 600; color: #86868b; margin: 0 0 15px 0; text-transform: uppercase; letter-spacing: 0.5px; }
      
      .grid-2 { display: grid; grid-template-columns: 1fr 1fr; gap: 25px; }
      @media (max-width: 650px) { .grid-2 { grid-template-columns: 1fr; } }
      
      .status-row { display: flex; justify-content: space-between; font-weight: 600; font-size: 15px; }
      .text-ap { color: #f59e0b; }
      .text-sta { color: #10b981; }
      
      .sensor-value { font-size: 32px; font-weight: 800; color: #ef4444; }
      .sensor-value.hum { color: #3b82f6; }
      
      /* AI Status Box */
      .ai-box { padding: 15px; border-radius: 16px; font-size: 24px; font-weight: 800; transition: all 0.3s; background: #f4f4f5; color: #a1a1aa;}
      .ai-normal { background: #dcfce7; color: #166534; box-shadow: 0 4px 15px rgba(22, 101, 52, 0.2); }
      .ai-abnormal { background: #fee2e2; color: #991b1b; box-shadow: 0 4px 15px rgba(153, 27, 27, 0.2); animation: pulse 1.5s infinite; }
      @keyframes pulse { 0% { transform: scale(1); } 50% { transform: scale(1.02); } 100% { transform: scale(1); } }

      /* Segmented Controls (Buttons) */
      .segment-control { display: flex; background: #f4f4f5; border-radius: 14px; padding: 4px; margin-bottom: 20px; }
      .segment-control button { flex: 1; border: none; background: transparent; padding: 10px 0; font-size: 14px; font-weight: 600; color: #71717a; border-radius: 10px; cursor: pointer; transition: all 0.2s ease; outline: none; }
      .segment-control button.active { background: #ffffff; color: #000000; box-shadow: 0 2px 8px rgba(0,0,0,0.1); }
      
      .control-row { display: flex; align-items: center; justify-content: space-between; font-size: 14px; font-weight: 600; color: #52525b; }
      input[type=range] { flex: 1; margin-left: 15px; accent-color: #007aff; cursor: pointer; }
      input[type=color] { border: none; width: 36px; height: 36px; border-radius: 50%; cursor: pointer; padding: 0; background: transparent; }
      input[type="color"]::-webkit-color-swatch-wrapper { padding: 0; }
      input[type="color"]::-webkit-color-swatch { border: none; border-radius: 50%; box-shadow: 0 2px 5px rgba(0,0,0,0.2); }

      .btn-primary { width: 100%; background: #007aff; color: white; padding: 16px; border-radius: 16px; border: none; font-weight: 600; font-size: 16px; cursor: pointer; transition: 0.2s; box-shadow: 0 4px 15px rgba(0,122,255,0.3); margin-top: 10px; }
      .btn-primary:hover { background: #006ce3; transform: translateY(-2px); }
    </style>
  </head>
  <body>
    <div class="dashboard">
      <h2>🏠 ESP32-S3 DASHBOARD</h2>
      
      <div class="card">
        <div class="grid-2">
          <div class="status-row"><span>🌐 Mode:</span> <span id="wifiMode">Loading...</span></div>
          <div class="status-row"><span>📡 Network:</span> <span id="wifiSSID" style="color: #1d1d1f;">--</span></div>
        </div>
      </div>

      <div class="grid-2">
        <div class="card" style="text-align: center;">
          <h3 class="card-title">🌡️ Temperature</h3>
          <div class="sensor-value" id="tempValue">-- °C</div>
        </div>
        <div class="card" style="text-align: center;">
          <h3 class="card-title">💧 Humidity</h3>
          <div class="sensor-value hum" id="humValue">-- %</div>
        </div>
      </div>

      <div class="card" style="text-align: center;">
        <h3 class="card-title">🧠 AI Anomaly Detection</h3>
        <div class="ai-box" id="aiStatus">Loading Model...</div>
      </div>

      <div class="grid-2">
        
        <div class="card">
          <h3 class="card-title">💡 Single LED</h3>
          <div class="segment-control" id="grp-led1">
            <button class="active" onclick="setLed1(0)">OFF</button>
            <button onclick="setLed1(1)">ON</button>
            <button onclick="setLed1(2)">BLINK</button>
          </div>
          <div class="control-row">
            <span>Speed: <span id="led1_val" style="color:#007aff;">1000</span>ms</span>
            <input type="range" id="led1_delay" min="100" max="2000" step="100" value="1000" onchange="updateLed1()">
          </div>
        </div>

        <div class="card">
          <h3 class="card-title">🌈 NeoPixel LED</h3>
          <div class="segment-control" id="grp-neo">
            <button class="active" onclick="setNeo(0)">OFF</button>
            <button onclick="setNeo(1)">ON</button>
            <button onclick="setNeo(2)">BLINK</button>
          </div>
          <div class="control-row">
            <span>Color Picker:</span>
            <input type="color" id="neo_color" value="#007aff" onchange="updateNeo()">
          </div>
        </div>

      </div>

      <button class="btn-primary" onclick="window.location='/settings'">⚙️ Wi-Fi Settings (STA)</button>
    </div>

    <script>
      let led1Mode = 0;
      let neoMode = 0;

      function updateActiveBtn(groupId, mode) {
        let btns = document.getElementById(groupId).getElementsByTagName('button');
        for(let i=0; i<btns.length; i++) {
          if(i == mode) btns[i].classList.add('active');
          else btns[i].classList.remove('active');
        }
      }

      function setLed1(mode) { led1Mode = mode; updateActiveBtn('grp-led1', mode); updateLed1(); }
      function updateLed1() {
        let delay = document.getElementById('led1_delay').value;
        document.getElementById('led1_val').innerText = delay;
        fetch(`/led1?mode=${led1Mode}&delay=${delay}`);
      }

      function setNeo(mode) { neoMode = mode; updateActiveBtn('grp-neo', mode); updateNeo(); }
      function updateNeo() {
        let hex = document.getElementById('neo_color').value;
        let r = parseInt(hex.substr(1,2), 16); let g = parseInt(hex.substr(3,2), 16); let b = parseInt(hex.substr(5,2), 16);
        fetch(`/neo?mode=${neoMode}&r=${r}&g=${g}&b=${b}`);
      }

      function updateData() {
        fetch('/data')
          .then(r => r.json())
          .then(json => {
            document.getElementById('tempValue').innerText = json.temp + ' °C';
            document.getElementById('humValue').innerText = json.hum + ' %';
            
            let modeEl = document.getElementById('wifiMode');
            modeEl.innerText = json.mode;
            modeEl.className = json.mode === "AP Only" ? "text-ap" : "text-sta";
            document.getElementById('wifiSSID').innerText = json.ssid;

            // Xử lý hiển thị AI
            let aiEl = document.getElementById('aiStatus');
            if(json.ml === "NORMAL") {
                aiEl.innerText = "✅ NORMAL ENVIRONMENT";
                aiEl.className = "ai-box ai-normal";
            } else if (json.ml === "ABNORMAL") {
                aiEl.innerText = "🚨 ABNORMAL DETECTED!";
                aiEl.className = "ai-box ai-abnormal";
            }
          });
      }
      updateData();
      setInterval(updateData, 3000);
    </script>
  </body>
  </html>
  )rawliteral";
}

// ========== SETTINGS UI ==========
String settingsPage() {
  return R"rawliteral(
  <!DOCTYPE html><html lang="en"><head><meta charset="UTF-8"><meta name="viewport" content="width=device-width, initial-scale=1.0"><title>Wi-Fi Settings</title>
  <style>
    @import url('https://fonts.googleapis.com/css2?family=Inter:wght@400;600;800&display=swap');
    body { font-family: 'Inter', sans-serif; background: #f4f7f6; display: flex; justify-content: center; align-items: center; min-height: 100vh; margin: 0; }
    .card { background: white; padding: 40px 30px; border-radius: 24px; box-shadow: 0 10px 30px rgba(0,0,0,0.05); width: 100%; max-width: 350px; text-align: center;}
    h2 { margin: 0 0 25px 0; font-weight: 800; color: #1d1d1f; }
    input { width: 100%; box-sizing: border-box; padding: 15px; margin-bottom: 15px; border-radius: 12px; border: 1px solid #d2d2d7; background: #fafafa; font-size: 15px; outline: none; transition: 0.2s;}
    input:focus { border-color: #007aff; background: #fff; }
    .btn { width: 100%; padding: 15px; font-size: 16px; font-weight: 600; cursor: pointer; border-radius: 14px; border: none; transition: 0.2s; margin-bottom: 10px;}
    .btn-connect { background: #10b981; color: white; box-shadow: 0 4px 15px rgba(16,185,129,0.3); }
    .btn-connect:hover { background: #059669; transform: translateY(-2px); }
    .btn-back { background: #f4f4f5; color: #52525b; }
    .btn-back:hover { background: #e4e4e7; }
    #msg { margin-top: 15px; font-weight: 600; color: #10b981; font-size: 14px;}
  </style></head><body>
  <div class="card"><h2>⚙️ NETWORK CONFIG</h2>
    <form id="wifiForm">
      <input type="text" id="ssid" placeholder="Wi-Fi Name (SSID)" required>
      <input type="password" id="pass" placeholder="Password">
      <button type="submit" class="btn btn-connect">Connect</button>
      <button type="button" class="btn btn-back" onclick="window.location='/'">Cancel</button>
    </form>
    <p id="msg"></p>
  </div>
  <script>
    document.getElementById('wifiForm').onsubmit = function(e){
      e.preventDefault();
      document.getElementById('msg').innerText = "Command sent. Returning to home...";
      fetch('/connect?ssid=' + encodeURIComponent(document.getElementById('ssid').value) + '&pass=' + encodeURIComponent(document.getElementById('pass').value));
      setTimeout(() => { window.location.href = '/'; }, 1500);
    };
  </script></body></html>
  )rawliteral";
}

// ========== HTTP HANDLERS ==========
void handleRoot() { server.send(200, "text/html", mainPage()); }
void handleSettings() { server.send(200, "text/html", settingsPage()); }

void handleData() {
  String modeStr = isAPMode ? "AP Only" : "AP + STA";
  String networkStr = isAPMode ? "Not Connected" : sta_ssid;
  
  // Lấy dữ liệu từ AI
  String mlStr = (glob_ml_state == 1) ? "ABNORMAL" : "NORMAL";
  
  // Đóng gói JSON
  String json = "{\"temp\":\"" + String(glob_temperature, 1) + 
                "\",\"hum\":\"" + String(glob_humidity, 1) + 
                "\",\"mode\":\"" + modeStr + 
                "\",\"ssid\":\"" + networkStr + 
                "\",\"ml\":\"" + mlStr + "\"}"; // Bổ sung trường "ml"
  server.send(200, "application/json", json);
}

void handleLed1() {
  if (server.hasArg("mode")) led1_mode = server.arg("mode").toInt();
  if (server.hasArg("delay")) led1_delay = server.arg("delay").toInt();
  if (led1_mode == 0) { led1_state = false; digitalWrite(LED1_PIN, LOW); Serial.println("=> Web command: LED 1 OFF"); } 
  else if (led1_mode == 1) { led1_state = true; digitalWrite(LED1_PIN, HIGH); Serial.println("=> Web command: LED 1 ON"); }
  server.send(200, "text/plain", "OK");
}

void handleNeo() {
  if (server.hasArg("mode")) neo_mode = server.arg("mode").toInt();
  if (server.hasArg("r")) neo_r = server.arg("r").toInt();
  if (server.hasArg("g")) neo_g = server.arg("g").toInt();
  if (server.hasArg("b")) neo_b = server.arg("b").toInt();
  
  if (neo_mode == 0) {
    neo_state = false; pixels.clear(); pixels.show(); 
    Serial.println("=> Web command: NeoPixel OFF");
  } else if (neo_mode == 1) {
    neo_state = true;
    for(int i=0; i<NUM_PIXELS; i++) pixels.setPixelColor(i, pixels.Color(neo_r, neo_g, neo_b));
    pixels.show();
    Serial.printf("=> Web command: NeoPixel ON (R:%d G:%d B:%d)\n", neo_r, neo_g, neo_b);
  }
  server.send(200, "text/plain", "OK");
}

void handleConnect() {
  sta_ssid = server.arg("ssid");
  sta_pass = server.arg("pass");
  server.send(200, "text/plain", "OK");
  Serial.println("\n[Web Server] Received new network config: " + sta_ssid);
  retry_count = 0;
  WiFi.mode(WIFI_AP_STA);
  WiFi.disconnect(); 
  vTaskDelay(pdMS_TO_TICKS(100));
  WiFi.begin(sta_ssid.c_str(), sta_pass.c_str());
}

void setupServer() {
  server.on("/", HTTP_GET, handleRoot);
  server.on("/settings", HTTP_GET, handleSettings);
  server.on("/data", HTTP_GET, handleData); 
  server.on("/led1", HTTP_GET, handleLed1);
  server.on("/neo", HTTP_GET, handleNeo); 
  server.on("/connect", HTTP_GET, handleConnect);
  server.begin();
}

// ========== MAIN TASK ==========
void main_server(void *pvParameters) {
  pinMode(LED1_PIN, OUTPUT);
  digitalWrite(LED1_PIN, LOW);
  
  pixels.begin();
  pixels.clear();
  pixels.show();

  WiFi.onEvent(WiFiEvent);
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig(apIP, gateway, subnet);
  WiFi.softAP("ESP32 Access Point", "12345678"); 

  Serial.println("\n--- STARTING WEB SERVER ---");
  Serial.print("Access Point SSID: ESP32 Access Point\n");
  Serial.print("Web Page IP: ");
  Serial.println(WiFi.softAPIP());

  setupServer();

  while (1) {
    server.handleClient();
    unsigned long current_ms = millis();

    if (led1_mode == 2) {
      if (current_ms - led1_last_ms >= led1_delay) {
        led1_last_ms = current_ms;
        led1_state = !led1_state; 
        digitalWrite(LED1_PIN, led1_state ? HIGH : LOW);
      }
    }

    if (neo_mode == 2) {
      if (current_ms - neo_last_ms >= neo_blink_delay) {
        neo_last_ms = current_ms;
        neo_state = !neo_state; 
        if (neo_state) {
          for(int i=0; i<NUM_PIXELS; i++) pixels.setPixelColor(i, pixels.Color(neo_r, neo_g, neo_b));
        } else {
          pixels.clear();
        }
        pixels.show();
      }
    }

    vTaskDelay(pdMS_TO_TICKS(10)); 
  }
}