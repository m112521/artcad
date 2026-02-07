#include <TB6612_ESP32.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <ESP32Servo.h>

#define AIN1 13 // ESP32 Pin D13 to TB6612FNG Pin AIN1
#define BIN1 12 // ESP32 Pin D12 to TB6612FNG Pin BIN1
#define AIN2 14 // ESP32 Pin D14 to TB6612FNG Pin AIN2
#define BIN2 27 // ESP32 Pin D27 to TB6612FNG Pin BIN2
#define PWMA 26 // ESP32 Pin D26 to TB6612FNG Pin PWMA
#define PWMB 25 // ESP32 Pin D25 to TB6612FNG Pin PWMB
#define STBY 33 // ESP32 Pin D33 to TB6612FNG Pin STBY

const char* ssid = "bot00001";
const char* password = "artcad2026";

#define RELAY_PIN 15 

#define SERVO1_PIN 5
#define SERVO2_PIN 4
#define SERVO3_PIN 16
#define SERVO4_PIN 17
#define SERVO5_PIN 18

Servo servo1;
// Servo servo2;
// Servo servo3;
// Servo servo4;
// Servo bigServo;

int speed = 255;
int direction = 0;
int slider = 0;
int fire = 0;
int led = 0;

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

JSONVar readings;

const int offsetA = 1;
const int offsetB = 1;

Motor motor1 = Motor(AIN1, AIN2, PWMA, offsetA, STBY, 5000, 8, 5);
Motor motor2 = Motor(BIN1, BIN2, PWMB, offsetB, STBY, 5000, 8, 6);

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<meta name="viewport" content="width=device-width, initial-scale=1.0">

<style>
    body {height: 100vh;overflow: hidden;margin: 0;}
    .parent {
        display: grid;
        grid-template-columns: repeat(2, 1fr);
        grid-template-rows: repeat(2, 1fr);
        grid-column-gap: 0px;
        grid-row-gap: 0px;
        height: 100%;
        }
        
        .div1 { grid-area: 1 / 2 / 3 / 3; height: 100vh;}
        .div2 { grid-area: 1 / 1 / 2 / 2; padding: 0.5rem;}
        .div3 { grid-area: 2 / 1 / 3 / 2; }
        .parent-j {
        display: grid;
        grid-template-columns: repeat(3, 1fr);
        grid-template-rows: repeat(3, 1fr);
        grid-column-gap: 0px;
        grid-row-gap: 0px;
        height: 100%;
        /* justify-items: center;
        align-items: center; */
        }

        .div-u { grid-area: 1 / 2 / 2 / 3; border: 1px solid gray;padding:2rem 3rem; border-radius:100% 100% 0 0;/**display:flex;flex-wrap:nowrap;justify-content:center;align-items:center;**/}
        .div-l { grid-area: 2 / 1 / 3 / 2; border: 1px solid gray;padding:2.95rem 2.4rem; border-radius:100% 0 0 100%;}
        .div-d { grid-area: 3 / 2 / 4 / 3; border: 1px solid gray;padding:2rem 3rem; border-radius:0 0 100% 100%;}
        .div-r { grid-area: 2 / 3 / 3 / 4; border: 1px solid gray;padding:2.95rem 2.4rem; border-radius:0 100% 100% 0;}
        .div-s { grid-area: 2 / 2 / 3 / 3; border: 1px solid rgb(233, 80, 80);padding:3rem 2.3rem; }

        .circ-btn{border-radius:100%;border:1px solid red;width:180px;height:180px;background:none;}
        @media screen and (orientation: portrait) {
            .parent {
                display: grid;
                grid-template-columns: 1fr;
                grid-template-rows: repeat(3, 1fr);
                grid-column-gap: 0px;
                grid-row-gap: 0px;
                }

                .div1 { grid-area: 3 / 1 / 4 / 2; height: auto;}
                .div2 { grid-area: 2 / 1 / 3 / 2; }
                .div3 { grid-area: 1 / 1 / 2 / 2; }
        }
</style>
<div class="parent">
    <div class="div1">
        <div class="parent-j">
            <button class="div-u" id="btn-up">F</button>
            <button class="div-l" id="btn-lt">L</button>
            <button class="div-d" id="btn-dn">B</button>
            <button class="div-r" id="btn-rt">R</button>
            <button class="div-s" id="btn-sp">STOP</button>
        </div>
    </div>
    <div class="div3"><button class="circ-btn" id="btn-fire">LAUNCH</button><button class="circ-btn" id="btn-led">LED</button></div>
</div>

<script>
let gateway = `ws://${window.location.hostname}/ws`;

let fireBtn = document.querySelector("#btn-fire");
let ledBtn = document.querySelector("#btn-led");
let ledState = 0;
let fireState = 0;

let websocket;
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();
    initButtons();
}

function initButtons() {
  document.querySelector('#btn-up').addEventListener('touchstart', ()=>{ websocket.send(JSON.stringify({dir:11})) });
  document.querySelector('#btn-up').addEventListener('touchend', ()=>{ websocket.send(JSON.stringify({dir:12})) });

  document.querySelector('#btn-dn').addEventListener('touchstart', ()=>{ websocket.send(JSON.stringify({dir:21})) });
  document.querySelector('#btn-dn').addEventListener('touchend', ()=>{ websocket.send(JSON.stringify({dir:22})) });

  document.querySelector('#btn-lt').addEventListener('touchstart', ()=>{ websocket.send(JSON.stringify({dir:31})) });
  document.querySelector('#btn-lt').addEventListener('touchend', ()=>{ websocket.send(JSON.stringify({dir:32})) });

  document.querySelector('#btn-rt').addEventListener('touchstart', ()=>{ websocket.send(JSON.stringify({dir:41})) });
  document.querySelector('#btn-rt').addEventListener('touchend', ()=>{ websocket.send(JSON.stringify({dir:42})) });

  document.querySelector('#btn-led').addEventListener('click', ()=>{ 
    ledState = !ledState;
    toggleBg(ledBtn, ledState);
    websocket.send(JSON.stringify({led:ledState ? 1 : 0})); 
  });
  
  document.querySelector('#btn-fire').addEventListener('click', ()=>{ 
    fireState = !fireState;
    toggleBg(fireBtn, fireState);
    websocket.send(JSON.stringify({fire:fireState ? 1 : 0})); 
  });
}

function toggleBg(btn, state) {
    if (state) {
        btn.style.background = '#ff0000';
    }
    else {
        btn.style.background = '#ffffff';
    }
}

function initWebSocket() {
    console.log('Trying to open a WebSocket connection…');
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    websocket.onmessage = onMessage;
}

function onOpen(event) {
    console.log('Connection opened');
    getReadings();
}

function onClose(event) {
    console.log('Connection closed');
    setTimeout(initWebSocket, 2000);
}

function getReadings(){
    websocket.send("getReadings");
}

function onMessage(event) {
    websocket.send("getReadings");
}</script>

)rawliteral";


String getSensorReadings(){
  readings["s"] = String(slider);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}

void initWiFi() {
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}

void notifyClients(String sensorReadings) {
  ws.textAll(sensorReadings);
}

void handleWebSocketMessage(void *arg, uint8_t *data, size_t len) {
  AwsFrameInfo *info = (AwsFrameInfo*)arg;
  if (info->final && info->index == 0 && info->len == len && info->opcode == WS_TEXT) {
    
    JSONVar myObject = JSON.parse((const char*)data);
    if (myObject.hasOwnProperty("fire")) {
      fire = (int)myObject["fire"];      
    }
    else if (myObject.hasOwnProperty("led")) {
      led = (int)myObject["led"];      
    }
    else if (myObject.hasOwnProperty("dir")) {
      direction = (int)myObject["dir"];
      move(direction, speed);      
    }

    String sensorReadings = getSensorReadings();
    notifyClients(sensorReadings);
  }
}

void move(int direction, int speed) {
  if (direction == 11) { // Forward
    forward(motor1, motor2, speed);
  }
  else if (direction == 12) { // Forward
    motor1.brake();
    motor2.brake();
  }
  else if (direction == 21) { // Backward
    back(motor1, motor2, speed);
  }
  else if (direction == 22) { // Backward
    motor1.brake();
    motor2.brake();
  }
  else if (direction == 31) { // Left
    motor1.drive(-255); // speed, optional duration
    motor2.drive(255);
  }
  else if (direction == 32) { // Left
    motor1.brake();
    motor2.brake();
  }
  else if (direction == 41) { // Right
    motor1.drive(255); // speed, optional duration
    motor2.drive(-255);
  }
  else if (direction == 42) { // Right
    motor1.brake();
    motor2.brake();
  }
}

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type, void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket() {
  ws.onEvent(onEvent);
  server.addHandler(&ws);
}

void setup()
{
  Serial.begin(115200);
  pinMode(RELAY_PIN, OUTPUT);  

  servo1.attach(SERVO1_PIN);
  //servo2.attach(SERVO2_PIN);
  //servo3.attach(SERVO3_PIN);
  //servo4.attach(SERVO4_PIN);
  //bigServo.attach(SERVO5_PIN);

  servo1.write(90);
  // servo2.write(90); 
  // servo3.write(90); 
  // servo4.write(90);
  
  initWiFi();
  initWebSocket();
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
    request->send_P(200, "text/html", index_html);
  });
  server.begin();
}

void loop()
{
  // при нажатии на кнопку LED на смартфоне
  if (led == 1) {
    servo1.write(180); // поворот сервы на 180 градусов
  }
  else {
    servo1.write(0); // поворот сервы на 0 градусов
  }

  if (fire == 1) {
    digitalWrite(RELAY_PIN, HIGH);
  }
  else {
    digitalWrite(RELAY_PIN, LOW);
  }
  //Serial.println(String(direction) + " " + String(slider) + " " + String(fire) + " " + String(led));
}
