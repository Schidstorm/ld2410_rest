#pragma once


#define DEBUG_HTTPCLIENT

#include <vector>

#if defined(ESP8266)
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#elif defined(ESP32)
  #include <WiFi.h>
  #include <AsyncTCP.h>
#endif

#include <ESPAsyncWebServer.h>
#include <Arduino.h>

#include "rest_service.h"


namespace ld2410_rest {

class WifiWebServer {
private:
  StaticJsonDocument<JSON_OBJECT_SIZE(128)> m_json_doc;
  std::vector<uint8_t> m_upload_body_buffer;

public:
  WifiWebServer(): m_json_doc() {
    
  }

  void begin(AsyncWebServer * server, const std::vector<rest_service_t> services);
  void loop();

  JsonDocument *json_doc() {
    return &m_json_doc;
  }
};

}