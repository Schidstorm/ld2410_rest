#include "wifi_webserver.h"

using namespace ld2410_rest;


void WifiWebServer::begin(AsyncWebServer* server, const std::vector<rest_service_t> &services) {

  for(auto &srv: services) {
    Serial.print("registering rest service for ");
    Serial.print(srv.method.name().c_str());
    Serial.print(" ");
    Serial.println(srv.path.c_str());

    AsyncCallbackWebHandler* optionsHandler = new AsyncCallbackWebHandler();
    optionsHandler->setUri(srv.path.c_str());
    optionsHandler->setMethod(MethodOptions.internal_method());
    optionsHandler->onRequest([&](AsyncWebServerRequest *request){
      auto response = request->beginResponse(204, "application/json", "");
      response->addHeader("Access-Control-Allow-Origin", "*");
      response->addHeader("Access-Control-Allow-Methods", srv.method.name().c_str());
      response->addHeader("Access-Control-Allow-Headers", "content-type");
      request->send(response);
    });
    server->addHandler(optionsHandler);

    AsyncCallbackWebHandler* handler = new AsyncCallbackWebHandler();
    handler->setUri(srv.path.c_str());
    handler->setMethod(srv.method.internal_method());
    handler->onBody([&](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total) {
        if (m_upload_body_buffer.size() == 0) {
          m_upload_body_buffer.reserve(total+1); // be sure at least a zero byte is at the end
        }

        m_upload_body_buffer.insert(m_upload_body_buffer.end(), data, data + len);
    });
    handler->onRequest([&](AsyncWebServerRequest *request) {
      auto &handler = srv.handler;
      if (m_upload_body_buffer.size() > 0) {
        m_upload_body_buffer[m_upload_body_buffer.size() - 1] = '\0'; // be sure at least a zero byte is at the end
        deserializeJson(m_json_doc, m_upload_body_buffer.data());
        m_upload_body_buffer.clear();
      }

      auto response = handler(request, &m_json_doc);
      
      if (std::holds_alternative<RestServiceResult>(response)) {
        auto &result = std::get<RestServiceResult>(response);

        Serial.printf("[%s] %d %s\n", request->methodToString(), result.status(), request->url().c_str());

        // auto json = DynamicJsonDocument(result.body());
        auto json_size = ArduinoJson::measureJson(m_json_doc);
        std::vector<uint8_t> result_buffer;
        result_buffer.resize(json_size);
        ArduinoJson::serializeJson(m_json_doc, result_buffer.data(), result_buffer.size());
        auto response = request->beginResponse_P(result.status(), "application/json", result_buffer.data(), result_buffer.size());
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", srv.method.name().c_str());
        response->addHeader("Access-Control-Allow-Headers", "content-type");
        request->send(response);
      } else {
        auto &err = std::get<RestServiceError>(response);
        auto jsonObj = JsonObject();
        jsonObj["error"] = err.message();
        auto json = DynamicJsonDocument(jsonObj);
        auto json_size = ArduinoJson::measureJson(json);
        std::string result_buffer;
        result_buffer.resize(json_size);
        ArduinoJson::serializeJson(json, result_buffer.data(), result_buffer.size());
        auto response = request->beginResponse(err.status(), "application/json", result_buffer.c_str());
        response->addHeader("Access-Control-Allow-Origin", "*");
        response->addHeader("Access-Control-Allow-Methods", srv.method.name().c_str());
        response->addHeader("Access-Control-Allow-Headers", "content-type");
        request->send(response);
      }
    });
    server->addHandler(handler);
  }
}


void WifiWebServer::loop() {
  
}