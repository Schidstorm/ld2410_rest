#pragma once
#include "simple_webserver.h"
#include "ld2410_state_filler.h"
#include "from_json.h"
#include "to_json.h"
#include "deserialize.h"

namespace ld2410_rest {

template <typename TReader, typename TWriter>
class Ld2410Webserver {
private:
    SimpleWebserver m_server;

public:
    Ld2410Webserver(): m_server(80, 2000) {

    }

    void on_request(WiFiClient* client, const String &method, const String &url, const char* body, size_t body_size, TReader *reader, TWriter *writer, Ld2410State *state) {
        if (method ==  "OPTIONS") {
            client->println("HTTP/1.1 204 No Content");
            client->println("Access-Control-Allow-Origin: *");
            client->println("Access-Control-Allow-Methods: PUT, GET");
            client->println("Access-Control-Allow-Headers: content-type");
            client->println("Connection: close");
            client->println();
        } else if (method == "GET") {
            doc.clear();
            auto o = doc.to<JsonObject>();
            to_json(o, state->get_detection());
            auto content_length = measureJson(o);
            
            client->println("HTTP/1.1 200 Ok");
            client->println("Access-Control-Allow-Origin: *");
            client->println("Access-Control-Allow-Methods: PUT, GET");
            client->println("Access-Control-Allow-Headers: content-type");
            client->print("Content-Length: ");
            client->println(content_length);
            client->println("Connection: close");
            client->println();

            serializeJson(o, *client);
        } else if (method == "PUT") {
            if (url == "/EnableConfigurationCommand") { process_command<ld2410::EnableConfigurationCommand>(client, body, body_size, reader, writer); }
            else if (url == "/MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand") { process_command<ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand>(client, body, body_size, reader, writer); }
            else if (url == "/ReadParameterCommand") { process_command<ld2410::ReadParameterCommand>(client, body, body_size, reader, writer); }
            else if (url == "/EnableEngineeringModeCommand") { process_command<ld2410::EnableEngineeringModeCommand>(client, body, body_size, reader, writer); }
            else if (url == "/CloseEngineeringModeCommand") { process_command<ld2410::CloseEngineeringModeCommand>(client, body, body_size, reader, writer); }
            else if (url == "/RangeSensitivityConfigurationCommand") { process_command<ld2410::RangeSensitivityConfigurationCommand>(client, body, body_size, reader, writer); }
            else if (url == "/ReadFirmwareVersionCommand") { process_command<ld2410::ReadFirmwareVersionCommand>(client, body, body_size, reader, writer); }
            else if (url == "/SetSerialPortBaudRate") { process_command<ld2410::SetSerialPortBaudRate>(client, body, body_size, reader, writer); }
            else if (url == "/FactoryReset") { process_command<ld2410::FactoryReset>(client, body, body_size, reader, writer); }
            else if (url == "/RestartModule") { process_command<ld2410::RestartModule>(client, body, body_size, reader, writer); }
        }
    }

    template <typename T>
    void process_command(WiFiClient* client, const char* body, size_t body_size, TReader *reader, TWriter *writer) {
        T command;
        deserialize<T>(body, body_size, command);
        auto response = ld2410::write_and_read_ack(*writer, *reader, command, 1000);
        serialize_and_write_to(response, client);
    }

    void begin() {
        m_server.begin();
    }

    void loop(TReader *reader, TWriter *writer, Ld2410State *state) {
        m_server.loop([&](WiFiClient* client, const String &method, const String &url, const char* body, size_t body_size) {
            this->on_request(client, method, url, body, body_size, reader, writer, state);
        });
    }
};

}