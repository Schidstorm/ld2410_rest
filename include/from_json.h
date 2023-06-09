#pragma once


#include <ArduinoJson.h>
#include <ld2410.h>

namespace ld2410_rest {

template<typename T>
T from_json(JsonObject &o);


template<>
ld2410::EnableConfigurationCommand from_json(JsonObject &o) {
    ld2410::EnableConfigurationCommand r;
    r.value(o["value"]);
    return r;
}

template<>
ld2410::EndConfigurationCommand from_json(JsonObject &o) {
    ld2410::EndConfigurationCommand r;
    return r;
}

template<>
ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand from_json(JsonObject &o) {
    ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand r;
    r.maximum_moving_distance_word(o["maximum_moving_distance_word"]);
    r.maximum_moving_distance_parameter(o["maximum_moving_distance_parameter"]);
    r.maximum_static_distance_door_word(o["maximum_static_distance_door_word"]);
    r.maximum_static_distance_door_parameter(o["maximum_static_distance_door_parameter"]);
    r.no_person_duration(o["no_person_duration"]);
    r.section_unattended_duration(o["section_unattended_duration"]);
    return r;
}

template<>
ld2410::ReadParameterCommand from_json(JsonObject &o) {
    ld2410::ReadParameterCommand r;
    return r;
}

template<>
ld2410::EnableEngineeringModeCommand from_json(JsonObject &o) {
    ld2410::EnableEngineeringModeCommand r;
    return r;
}

template<>
ld2410::CloseEngineeringModeCommand from_json(JsonObject &o) {
    ld2410::CloseEngineeringModeCommand r;
    return r;
}

template<>
ld2410::RangeSensitivityConfigurationCommand from_json(JsonObject &o) {
    ld2410::RangeSensitivityConfigurationCommand r;
    r.distance_gate_word(o["distance_gate_word"]);
    r.distance_gate_value(o["distance_gate_value"]);
    r.motion_sensitivity_word(o["motion_sensitivity_word"]);
    r.motion_sensitivity_value(o["motion_sensitivity_value"]);
    r.static_sensitivity_word(o["static_sensitivity_word"]);
    r.static_sensitivity_value(o["static_sensitivity_value"]);
    return r;
}

template<>
ld2410::ReadFirmwareVersionCommand from_json(JsonObject &o) {
    ld2410::ReadFirmwareVersionCommand r;
    return r;
}

template<>
ld2410::SetSerialPortBaudRate from_json(JsonObject &o) {
    ld2410::SetSerialPortBaudRate r;
    r.baudRate_selection_index((ld2410::BaudRate)(o["baudRate_selection_index"].as<JsonInteger>()));
    return r;
}



template<>
ld2410::FactoryReset from_json(JsonObject &o) {
    ld2410::FactoryReset r;
    return r;
}

template<>
ld2410::RestartModule from_json(JsonObject &o) {
    ld2410::RestartModule r;
    return r;
}

}