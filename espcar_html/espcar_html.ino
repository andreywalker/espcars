#include <esp32cam.h>
#include <WebServer.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include "WifiCam.hpp"

/*
const char* WIFI_SSID = "Pixel";
const char* WIFI_PASS = "asdpoi567";
*/
const char* WIFI_SSID = "home";
const char* WIFI_PASS = "RakHat39";

unsigned long timer;

int mpin1=27;
int mpin2=26;
int mpin3=25;
int mpin4=33;


WebServer server(80);
String compIP="";
StaticJsonDocument<250> jsonDocument;
char buffer[250];
const char HEADER[] = "HTTP/1.1 200 OK\r\n" \
                      "Access-Control-Allow-Origin: *\r\n" \
                      "Content-Type: multipart/x-mixed-replace; boundary=123456789000000000000987654321\r\n";
const char BOUNDARY[] = "\r\n--123456789000000000000987654321\r\n";
const char CTNTTYPE[] = "Content-Type: image/jpeg\r\nContent-Length: ";
const int hdrLen = strlen(HEADER);
const int bdrLen = strlen(BOUNDARY);
const int cntLen = strlen(CTNTTYPE);


void IndexHandle(){
  const char index_html[] PROGMEM = R"rawliteral( 
  <!DOCTYPE html>
<html>
<head>
  <style>
    body {
      background-color: #64e9b9;
      margin: 0;
      display: flex;
      justify-content: center;
      align-items: center;
      height: 100vh;
    }

    .btn_patterns {
      display: grid;
      grid-template-rows: 1fr 1fr 1fr;
      grid-template-columns: 1fr 1fr 1fr;
      gap: 1rem;
      width: 100%;
      height: 100%;
      max-width: 500px;
      max-height: 500px;
    }

    .btn {
      text-decoration: none;
      color: white;
      height: 100%;
      width: 100%;
      background-color: #4458ff;
      text-align: center;
      border-radius: 0.5rem;
      border: 0.1rem solid #4c9ae7;
      display: flex;
      align-items: center;
      justify-content: center;
      font-family: "Comic Sans MS", cursive, sans-serif;
      font-size: 2rem;
    }

    .red {
      background-color: red;
      border: 0.1rem solid #fd5a2f;
    }

    /* Specific grid positioning */
    #forward {
      grid-row: 1;
      grid-column: 2;
    }

    #left {
      grid-row: 2;
      grid-column: 1;
    }

    #stop {
      grid-row: 2;
      grid-column: 2;
    }

    #right {
      grid-row: 2;
      grid-column: 3;
    }

    #back {
      grid-row: 3;
      grid-column: 2;
    }
  </style>
</head>
<body>
  <div class="btn_patterns">
    <button class="btn" id="forward"><span>A</span></button>
    <button class="btn" id="left"><span>&lt;--</span></button>
    <button class="btn red" id="stop"><span>Стоп</span></button>
    <button class="btn" id="right"><span>--&gt;</span></button>
    <button class="btn" id="back"><span>V</span></button>
  </div>

  <script>
    const stopBtn = document.getElementById("stop");
    const forwardBtn = document.getElementById("forward");
    const leftBtn = document.getElementById("left");
    const backBtn = document.getElementById("back");
    const rightBtn = document.getElementById("right");

    stopBtn.addEventListener("touchstart", (e) => {
      e.preventDefault();
      fetch("/stop");
    });

    forwardBtn.addEventListener("touchstart", (e) => {
      e.preventDefault();
      fetch("/forward");
    });

    forwardBtn.addEventListener("touchend", (e) => {
      e.preventDefault();
      fetch("/stop");
    });

    leftBtn.addEventListener("touchstart", (e) => {
      e.preventDefault();
      fetch("/left");
    });

    leftBtn.addEventListener("touchend", (e) => {
      e.preventDefault();
      fetch("/stop");
    });

    rightBtn.addEventListener("touchstart", (e) => {
      e.preventDefault();
      fetch("/right");
    });

    rightBtn.addEventListener("touchend", (e) => {
      e.preventDefault();
      fetch("/stop");
    });

    backBtn.addEventListener("touchstart", (e) => {
      e.preventDefault();
      fetch("/back");
    });

    backBtn.addEventListener("touchend", (e) => {
      e.preventDefault();
      fetch("/stop");
    });
  </script>
</body>
</html>)rawliteral"; 
  server.send(200, "text/html; charset=utf-8", index_html);
}
void forward() { 
  Serial.println("Forward");
  digitalWrite(mpin1,1);
  digitalWrite(mpin2,0);
  digitalWrite(mpin3,1);
  digitalWrite(mpin4,0);
  IndexHandle();
} 
void left() { 
  Serial.println("Left");
  digitalWrite(mpin1,0);
  digitalWrite(mpin2,1);
  digitalWrite(mpin3,1);
  digitalWrite(mpin4,0); 
  IndexHandle();
} 
void right() { 
  Serial.println("Right"); 
  digitalWrite(mpin1,1);
  digitalWrite(mpin2,0);
  digitalWrite(mpin3,0);
  digitalWrite(mpin4,1);
  IndexHandle();
} 
void back() { 
  digitalWrite(mpin1,0);
  digitalWrite(mpin2,1);
  digitalWrite(mpin3,0);
  digitalWrite(mpin4,1);
  Serial.println("Back");
  IndexHandle();
} 

void stop() { 
  Serial.println("Stop");
  digitalWrite(mpin1,0);
  digitalWrite(mpin2,0);
  digitalWrite(mpin3,0);
  digitalWrite(mpin4,0);
  IndexHandle();
} 
void left45() { 
  Serial.println("Left45");
  digitalWrite(mpin1,0);
  digitalWrite(mpin2,1);
  digitalWrite(mpin3,1);
  digitalWrite(mpin4,0);  
  delay(200);
  digitalWrite(mpin1,0);
  digitalWrite(mpin2,0);
  digitalWrite(mpin3,0);
  digitalWrite(mpin4,0);
  delay(1000);
  IndexHandle();
} 
void right45() { 
  Serial.println("Left45");
  digitalWrite(mpin1,1);
  digitalWrite(mpin2,0);
  digitalWrite(mpin3,0);
  digitalWrite(mpin4,1);  
  delay(200);
  digitalWrite(mpin1,0);
  digitalWrite(mpin2,0);
  digitalWrite(mpin3,0);
  digitalWrite(mpin4,0);
  delay(1000);
  IndexHandle();
} 


void setIP(){
  if (server.hasArg("plain") == false) {
  }
  String body = server.arg("plain");
  deserializeJson(jsonDocument, body);
  compIP = (const char*)jsonDocument["ip"];
  Serial.println(compIP);
  server.send(200, "text/html","ok");  
}

/*
void forward(){
  timer = millis();
  while(millis()-timer<10){
    digitalWrite(14, LOW);
    digitalWrite(15, LOW);
    digitalWrite(13, HIGH);
  }
  server.send(200, "text/html","ok"); 
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
  digitalWrite(13, LOW);
}

void backward(){
  timer = millis();
  while(millis()-timer<10){
    digitalWrite(14, LOW);
    digitalWrite(15, HIGH);
    digitalWrite(13, LOW);
  }
  server.send(200, "text/html","ok");  
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
  digitalWrite(13, LOW);
  
}

void right(){
  timer = millis();
  while(millis()-timer<100){
    digitalWrite(14, LOW);
    digitalWrite(15, HIGH);
    digitalWrite(13, HIGH);
  }
  server.send(200, "text/html","ok");  
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
  digitalWrite(13, LOW);
  
}

void left(){
  timer = millis();
  while(millis()-timer<100){
    digitalWrite(14, HIGH);
    digitalWrite(15, LOW);
    digitalWrite(13, LOW);
  }
  server.send(200, "text/html","ok"); 
  digitalWrite(14, LOW);
  digitalWrite(15, LOW);
  digitalWrite(13, LOW);
  
}*/





void addMp(){
  addRequestHandlers();
}
 
 
void  setup(){
  pinMode(mpin1, OUTPUT);
  pinMode(mpin2, OUTPUT);
  pinMode(mpin3, OUTPUT);
  pinMode(mpin4, OUTPUT);
  //pinMode(2, OUTPUT);
  digitalWrite(mpin1, LOW);
  digitalWrite(mpin2, LOW);
  digitalWrite(mpin3, LOW);
  digitalWrite(mpin4, LOW);
  //digitalWrite(2, HIGH);
  delay(250);
  //digitalWrite(2, LOW);
  
  Serial.begin(115200);
  Serial.println();
  
  WiFi.persistent(false);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.println(WIFI_SSID);
  Serial.println(WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("missed...");
  }
  Serial.print("http://");
  Serial.println(WiFi.localIP());

  server.on("/setup", setIP);
  
  server.on("/forward", HTTP_GET, forward); 
  server.on("/left", HTTP_GET, left); 
  server.on("/right", HTTP_GET, right);   
  server.on("/back", HTTP_GET, back);
  server.on("/stop", HTTP_GET, stop);
  server.on("/left45", HTTP_GET, left45); 
  server.on("/right45", HTTP_GET, right45);  
  server.on("/",  HTTP_GET,  IndexHandle);
  addRequestHandlers();
  server.begin();
}
 
void loop()
{
  server.handleClient();
}
