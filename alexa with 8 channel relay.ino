#include <Arduino.h>
#if defined(ESP8266)
    #include <ESP8266WiFi.h>
#elif defined(ESP32)
    #include <WiFi.h>
#endif
#include <ESPAsyncWebServer.h>
#include "fauxmoESP.h"

#define WIFI_SSID "YOUR SSID"
#define WIFI_PASS "YOUR PASSWORD"

fauxmoESP fauxmo;
AsyncWebServer server(80);

#define SERIAL_BAUDRATE                 115200
#define LED1                           23
#define LED2                           22
#define LED3                           21
#define LED4                           19
#define LED5                           18
#define LED6                           5
#define LED7                           4
#define LED8                           2

void wifiSetup() {
    WiFi.mode(WIFI_STA);
    Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(100);
    }
    Serial.println();
    Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());

}

void serverSetup() {
    server.on("/index.html", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send(200, "text/plain", "Hello, world");
    });

    server.onRequestBody([](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        if (fauxmo.process(request->client(), request->method() == HTTP_GET, request->url(), String((char *)data))) return;
    });
    server.onNotFound([](AsyncWebServerRequest *request) {
        String body = (request->hasParam("body", true)) ? request->getParam("body", true)->value() : String();
        if (fauxmo.process(request->client(), request->method() == HTTP_GET, request->url(), body)) return;
        
    });    
    server.begin();

}

void setup() {

  
    Serial.begin(SERIAL_BAUDRATE);
    Serial.println();
    Serial.println();
    pinMode(LED1, OUTPUT);
    digitalWrite(LED1, HIGH); 
    pinMode(LED2, OUTPUT);
    digitalWrite(LED2, HIGH);
    pinMode(LED3, OUTPUT);
    digitalWrite(LED3, HIGH);
    pinMode(LED4, OUTPUT);
    digitalWrite(LED4, HIGH);
    pinMode(LED5, OUTPUT);
    digitalWrite(LED5, HIGH);
    pinMode(LED6, OUTPUT);
    digitalWrite(LED6, HIGH);
    pinMode(LED7, OUTPUT);
    digitalWrite(LED7, HIGH);
    pinMode(LED8, OUTPUT);
    digitalWrite(LED8, HIGH);
    wifiSetup();
    serverSetup();
    fauxmo.createServer(false);
    fauxmo.setPort(80); 
    fauxmo.enable(true);
    fauxmo.addDevice("switch 1");
fauxmo.addDevice("switch 2");
fauxmo.addDevice("switch 3");
    fauxmo.addDevice("switch 4");
    fauxmo.addDevice("switch 5");
    fauxmo.addDevice("switch 6");
    fauxmo.addDevice("switch 7");
    fauxmo.addDevice("switch 8");

    fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {
     Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
if (strcmp(device_name, "switch 1")==0) {
            digitalWrite(LED1, state ? LOW  : HIGH);
        } else if (strcmp(device_name, "switch 2")==0) {
            digitalWrite(LED2, state ? LOW  : HIGH);
        } else if (strcmp(device_name, "switch 3")==0) {
            digitalWrite(LED3, state ? LOW  : HIGH);
        } else if (strcmp(device_name, "switch 4")==0) {
            digitalWrite(LED4, state ? LOW  : HIGH);
        } else if (strcmp(device_name, "switch 5")==0) {
            digitalWrite(LED5, state ? LOW  : HIGH);
        }else if (strcmp(device_name, "switch 6")==0) {
            digitalWrite(LED6, state ? LOW  : HIGH);
        }else if (strcmp(device_name, "switch 7")==0) {
            digitalWrite(LED7, state ? LOW  : HIGH);
        }else if (strcmp(device_name, "switch 8")==0) {
            digitalWrite(LED8, state ? LOW  : HIGH);
        }


    });

}

void loop() {
fauxmo.handle();
static unsigned long last = millis();
    if (millis() - last > 5000) {
        last = millis();
        Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
    }

}
