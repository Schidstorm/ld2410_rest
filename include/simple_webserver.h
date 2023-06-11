#pragma once

#include <WiFiServer.h>
#include <WiFiClient.h>
#include <optional>
#include <ArduinoJson.h>


class SimpleWebserver {
private:
    WiFiServer m_server;
    unsigned long m_timeout_time;

public:
    SimpleWebserver(uint16_t port, unsigned long timeout_time): m_server(port), m_timeout_time(timeout_time) {

    }

    void begin() {
        m_server.begin();
    }

    template<typename THandler>
    void loop(THandler handler) {
        WiFiClient client = m_server.available();   // Listen for incoming clients

        if (client) {                             // If a new client connects,
            auto first_line = read_line(&client);
            auto method = get_method(first_line);
            auto path = get_path(first_line);
            DynamicJsonDocument body_buffer = ArduinoJson::DynamicJsonDocument(JSON_OBJECT_SIZE(64));
            size_t content_length = 0;
            Serial.println(method);
            Serial.println(path);

            while(true) {
                auto header = read_line(&client);
                if (header.length() == 0) break;
                if (header.startsWith("Content-Length: ")) {
                    content_length = atoi(&header[strlen("Content-Length: ")]);
                }
            }

            if (content_length > 0) {
                String body;
                body.reserve(content_length+1);
                body[body.length() - 1] = '\0';
                client.readBytes(&body[0], content_length);
                deserializeJson(body_buffer, &body[0]);
            }

            handler(&client, method, path, body_buffer);

            // Close the connection
            client.stop();
            Serial.println("Client disconnected.");
            Serial.println("");
        }
    }

private:
    String read_line(WiFiClient *client) {
        auto res = read_until(client, '\r');
        client->read();

        return res;
    }

    String read_until(WiFiClient *client, char end_char) {
        auto currentTime = millis();
        auto previousTime = currentTime;
        String currentLine = "";                // make a String to hold incoming data from the client
        while (client->connected() && currentTime - previousTime <= m_timeout_time) {  // loop while the client's connected
            currentTime = millis();
            if (client->available()) {             // if there's bytes to read from the client,
                char c = client->read();

                if (c == end_char) {
                    break;
                } else{
                    currentLine.concat(c);
                }
            }
        }

        return currentLine;
    }

    String get_method(const String &first_line) {
        String method = "";
        for(size_t i = 0; i < first_line.length(); i++) {
            if (first_line[i] == ' ') {
                break;
            } else {
                method += first_line[i];
            }
        }

        return method;
    }

    String get_path(const String &first_line) {
        String path = "";
        bool found_space = false;
        for(size_t i = 0; i < first_line.length(); i++) {
            if (found_space) path += first_line[i];
            if (first_line[i] == ' ') {
                found_space = true;
            }
        }

        return path;
    }
};