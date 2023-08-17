#pragma once
#include "simple_webserver.h"
#include "ld2410_state_filler.h"
#include "deserialize.h"

namespace ld2410_rest {

template <typename TReader, typename TWriter>
class Ld2410Webserver {
private:
    SimpleWebserver m_server;

public:
    Ld2410Webserver(): m_server(80, 2000) {

    }

    void on_request(WiFiClient* client, const String &method, const String &url, const uint8_t* body, size_t body_size, TReader *reader, TWriter *writer, Ld2410State *state) {
        if (method ==  "OPTIONS") {
            client->println("HTTP/1.1 204 No Content");
            client->println("Access-Control-Allow-Origin: *");
            client->println("Access-Control-Allow-Methods: PUT, GET");
            client->println("Access-Control-Allow-Headers: content-type");
            client->println("Connection: close");
            client->println();
        } else if (method == "GET") {
            auto detection = state->get_detection();
            if (detection.has_value()) {
                if (std::holds_alternative<ReportingDataFrame>(detection.value())) {
                    serialize_and_write_to(std::get<ReportingDataFrame>(detection.value()), print_ok_data(client));
                } else {
                    serialize_and_write_to(std::get<EngineeringModeDataFrame>(detection.value()), print_ok_data(client));
                }
            }
        } else if (method == "PUT") {
            if (url == "/EnableConfigurationCommand") { process_command<EnableConfigurationCommand>(client, body, body_size, reader, writer); }
            else if (url == "/MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand") { process_command<MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand>(client, body, body_size, reader, writer); }
            else if (url == "/ReadParameterCommand") { process_command<ReadParameterCommand>(client, body, body_size, reader, writer); }
            else if (url == "/EnableEngineeringModeCommand") { process_command<EnableEngineeringModeCommand>(client, body, body_size, reader, writer); }
            else if (url == "/CloseEngineeringModeCommand") { process_command<CloseEngineeringModeCommand>(client, body, body_size, reader, writer); }
            else if (url == "/RangeSensitivityConfigurationCommand") { process_command<RangeSensitivityConfigurationCommand>(client, body, body_size, reader, writer); }
            else if (url == "/ReadFirmwareVersionCommand") { process_command<ReadFirmwareVersionCommand>(client, body, body_size, reader, writer); }
            else if (url == "/SetSerialPortBaudRate") { process_command<SetSerialPortBaudRate>(client, body, body_size, reader, writer); }
            else if (url == "/FactoryReset") { process_command<FactoryReset>(client, body, body_size, reader, writer); }
            else if (url == "/RestartModule") { process_command<RestartModule>(client, body, body_size, reader, writer); }
        }
    }

    auto print_ok_data(WiFiClient* client) {
        return [=](const uint8_t* body, size_t body_size){
            client->println("HTTP/1.1 200 Ok");
            client->println("Access-Control-Allow-Origin: *");
            client->println("Access-Control-Allow-Methods: PUT, GET");
            client->println("Access-Control-Allow-Headers: content-type");
            client->println("Content-Type: application/msgpack");
            client->print("Content-Length: ");
            client->println(body_size);
            client->println("Connection: close");
            client->println();
            client->write(body, body_size);
        };
    }

    template <typename T>
    void process_command(WiFiClient* client, const uint8_t* body, size_t body_size, TReader *reader, TWriter *writer) {
        T command;
        deserialize<T>(body, body_size, command);
        auto response = ld2410::write_and_read_ack(*writer, *reader, command, 1000);
        if (response.has_value()) {
            serialize_and_write_to(response.value(), print_ok_data(client));
        }
    }

    void begin() {
        m_server.begin();
    }

    void loop(TReader *reader, TWriter *writer, Ld2410State *state) {
        m_server.loop([&](WiFiClient* client, const String &method, const String &url, const char* body, size_t body_size) {
            this->on_request(client, method, url, (const uint8_t*)body, body_size, reader, writer, state);
        });
    }
};

}