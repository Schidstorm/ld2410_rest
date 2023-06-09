#pragma once
#include "wifi_webserver.h"
#include "ld2410_state_filler.h"
#include "from_json.h"
#include "to_json.h"

namespace ld2410_rest {

class Ld2410Webserver {
private:
    WifiWebServer m_server;

public:
    Ld2410Webserver(): m_server() {

    }

    

    template <typename T, typename TReader, typename TWriter>
    constexpr rest_service_t command_to_route(std::string route, TReader *reader, TWriter *writer) {
    return { MethodPut, route, [=](AsyncWebServerRequest* req, JsonDocument* res){
        auto o = res->as<JsonObject>();
        auto command = from_json<T>(o);
        o.clear();
        auto ack = ld2410::write_and_read_ack(*writer, *reader, command, 500);
        if (!ack) {
            o["error"] = "no ack packet";
        } else {
            to_json(o, ack);
        }

        return RestServiceResult(200);
    }};
    }

    template <typename TReader, typename TWriter>
    void begin(AsyncWebServer* async_web_server, Ld2410State *state, TReader *reader, TWriter *writer) {
        m_server.begin(async_web_server, {
            { MethodOptions, "/", [](AsyncWebServerRequest* req, JsonDocument* res){
                res->to<JsonObject>();

                return RestServiceResult(200);
            }},
            { MethodGet, "/state", [=](AsyncWebServerRequest* req, JsonDocument* res){
                auto o = res->to<JsonObject>();
                to_json(o, state->get_detection());

                return RestServiceResult(200);
            }},
            command_to_route<ld2410::EnableConfigurationCommand>("/EnableConfigurationCommand", reader, writer),
            command_to_route<ld2410::EndConfigurationCommand>("/EndConfigurationCommand", reader, writer),
            command_to_route<ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand>("/MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand", reader, writer),
            command_to_route<ld2410::ReadParameterCommand>("/ReadParameterCommand", reader, writer),
            command_to_route<ld2410::EnableEngineeringModeCommand>("/EnableEngineeringModeCommand", reader, writer),
            command_to_route<ld2410::CloseEngineeringModeCommand>("/CloseEngineeringModeCommand", reader, writer),
            command_to_route<ld2410::RangeSensitivityConfigurationCommand>("/RangeSensitivityConfigurationCommand", reader, writer),
            command_to_route<ld2410::ReadFirmwareVersionCommand>("/ReadFirmwareVersionCommand", reader, writer),
            command_to_route<ld2410::SetSerialPortBaudRate>("/SetSerialPortBaudRate", reader, writer),
            command_to_route<ld2410::FactoryReset>("/FactoryReset", reader, writer),
            command_to_route<ld2410::RestartModule>("/RestartModule", reader, writer),
        });
    }
};

}