#pragma once
#include "wifi_webserver.h"
#include "ld2410_state_filler.h"
#include "from_json.h"
#include "to_json.h"

namespace ld2410_rest {

template <typename TReader, typename TWriter>
class Ld2410Webserver {
private:
    WifiWebServer m_server;
    TReader *m_reader;
    TWriter *m_writer;
    Ld2410State *m_state;

    const std::vector<rest_service_t> m_services = {
        { MethodOptions, "/", [](AsyncWebServerRequest* req, JsonDocument* res){
            res->to<JsonObject>();

            return RestServiceResult(200);
        }},
        { MethodGet, "/state", [](AsyncWebServerRequest* req, JsonDocument* res){
            auto o = res->to<JsonObject>();
            to_json(o, m_state->get_detection());

            return RestServiceResult(200);
        }},
        command_to_route<ld2410::EnableConfigurationCommand>("/EnableConfigurationCommand"),
        command_to_route<ld2410::EndConfigurationCommand>("/EndConfigurationCommand"),
        command_to_route<ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand>("/MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand"),
        command_to_route<ld2410::ReadParameterCommand>("/ReadParameterCommand"),
        command_to_route<ld2410::EnableEngineeringModeCommand>("/EnableEngineeringModeCommand"),
        command_to_route<ld2410::CloseEngineeringModeCommand>("/CloseEngineeringModeCommand"),
        command_to_route<ld2410::RangeSensitivityConfigurationCommand>("/RangeSensitivityConfigurationCommand"),
        command_to_route<ld2410::ReadFirmwareVersionCommand>("/ReadFirmwareVersionCommand"),
        command_to_route<ld2410::SetSerialPortBaudRate>("/SetSerialPortBaudRate"),
        command_to_route<ld2410::FactoryReset>("/FactoryReset"),
        command_to_route<ld2410::RestartModule>("/RestartModule"),
    };

public:
    Ld2410Webserver(): m_server() {

    }

    

    template <typename T>
    constexpr rest_service_t command_to_route(std::string route) {
    return { MethodPut, route, [=](AsyncWebServerRequest* req, JsonDocument* res){
        auto o = res->as<JsonObject>();
        auto command = from_json<T>(o);
        o.clear();
        auto ack = ld2410::write_and_read_ack(*m_writer, *m_reader, command, 500);
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
        this->m_reader = reader;
        this->m_writer = writer;
        this->m_state = state;

        m_server.begin(async_web_server, m_services);
    }
};

}