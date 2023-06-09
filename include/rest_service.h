#pragma once

#include <functional>
#include <variant>
#include <tuple>
#include <optional>

#include <ArduinoJson.h>
#include <ESPAsyncWebServer.h>

namespace ld2410_rest {

class RestMethod {
private:
  const char* m_name;
  WebRequestMethod m_i_method;

public:
  RestMethod(const char* name, WebRequestMethod i_method): m_name(name), m_i_method(i_method) {

  }

  decltype(m_name) name() const {
    return m_name;
  }

  decltype(m_i_method) internal_method() const {
    return m_i_method;
  }
};

class RestServiceError {
private:
    uint16_t m_status;
    String m_message;

public:
    RestServiceError(decltype(m_status) status, decltype(m_message) message): m_status(status), m_message(message) {

    }

    RestServiceError(): RestServiceError(500, "undefined") {

    }

    decltype(m_message) message() const {
        return m_message;
    }

    decltype(m_status) status() const {
        return m_status;
    }
};

class RestServiceResult {
private:
    uint16_t m_status;

public:
    RestServiceResult(decltype(m_status) status): m_status(status) {

    }

    decltype(m_status) status() const {
        return m_status;
    }
};


const auto MethodGet = RestMethod("GET", HTTP_GET);
const auto MethodPost = RestMethod("POST", HTTP_POST);
const auto MethodDelete = RestMethod("DELETE", HTTP_DELETE);
const auto MethodPut = RestMethod("PUT", HTTP_PUT);
const auto MethodPatch = RestMethod("PATCH", HTTP_PATCH);
const auto MethodHead = RestMethod("HEAD", HTTP_HEAD);
const auto MethodOptions = RestMethod("OPTIONS", HTTP_OPTIONS);
const auto MethodAny = RestMethod("ANY", HTTP_ANY);

using rest_service_result_t = std::variant<RestServiceError, RestServiceResult>;
using rest_service_handler_t = std::function<rest_service_result_t(AsyncWebServerRequest*, JsonDocument*)>;

struct RestServiceTrigger {
    RestMethod method;
    const char* path;
    rest_service_handler_t handler;  
};

using rest_service_t = RestServiceTrigger;

}