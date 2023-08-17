#pragma once

#include <ld2410.h>
#include "ld2410_state.h"
#include "deserialize.h"

#include <Arduino.h>

namespace ld2410_rest {

class Ld2410StateFiller {
private:
    ld2410::StreamReader<64>* m_reader;
    ld2410::StreamWriter* m_writer;
    const uint8_t m_detection_pin;
    Ld2410State *m_state;
    bool m_engineering_mode = false;
    


public:
    Ld2410StateFiller(decltype(m_reader) reader, decltype(m_writer) writer, decltype(m_detection_pin) detection_pin, Ld2410State* state): m_reader(reader), m_writer(writer), m_detection_pin(detection_pin), m_state(state) {
        
    }

    void setup() {
        pinMode(m_detection_pin, INPUT);
    }

    void loop() {
        m_state->set_is_detected(digitalRead(m_detection_pin) < 500);

        auto packet = ld2410::read_from_reader<ld2410::ReportingDataFrame, ld2410::EngineeringModeDataFrame>(*m_reader);
        if (packet.has_value()) {
            m_state->set_detection(std::move(packet));
        }
        
    }

};

}