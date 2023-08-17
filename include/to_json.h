#pragma once

#include <variant>
#include <ArduinoJson.h>
#include <ld2410.h>

namespace ld2410_rest {

template<typename T>
void to_json(JsonObject &o, const T &d);
template<typename T1, typename T2>
void to_json(JsonObject &o, const std::variant<T1, T2> &d);
template<typename T>
void to_json(JsonObject &o, const std::optional<T> &d);

template<typename T>
void to_json(JsonObject &o, const std::optional<T> &d) {
    if (d.has_value()) 
        to_json(o, d.value());
}

template<typename T1, typename T2>
void to_json(JsonObject &o, const std::variant<T1, T2> &d) {
    if (std::holds_alternative<T1>(d))
        to_json(o, std::get<T1>(d));
    else 
        to_json(o, std::get<T2>(d));
}


template<>
void to_json(JsonObject &o, const ld2410::EnableConfigurationCommandAck &d) {
    o["status"] = d.status();
    o["protocol_version"] = d.protocol_version();
    o["buffer"] = d.buffer();
}

template<>
void to_json(JsonObject &o, const ld2410::EndConfigurationCommandAck &d) {
    o["status"] = d.status();
}

template<>
void to_json(JsonObject &o, const ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck &d) {
    o["status"] = d.status();
}

template<>
void to_json(JsonObject &o, const ld2410::ReadParameterCommandAck &d) {
    o["status"] = d.status();
    o["header"] = d.header();
    o["maximum_distance_gate_n"] = d.maximum_distance_gate_n();
    o["configure_maximum_moving_distance_gate"] = d.configure_maximum_moving_distance_gate();
    o["configure_maximum_static_gate"] = d.configure_maximum_static_gate();
    auto distance_gate_motion_sensitivity = o["distance_gate_motion_sensitivity"].to<JsonArray>();
    for(auto v: d.distance_gate_motion_sensitivity()) {
        distance_gate_motion_sensitivity.add(v);
    }
    auto distance_gate_rest_sensitivity = o["distance_gate_rest_sensitivity"].to<JsonArray>();
    for(auto v: d.distance_gate_rest_sensitivity()) {
        distance_gate_rest_sensitivity.add(v);
    }
    o["no_time_duration"] = d.no_time_duration();
}

template<>
void to_json(JsonObject &o, const ld2410::EnableEngineeringModeCommandAck &d) {
    o["status"] = d.status();
}

template<>
void to_json(JsonObject &o, const ld2410::CloseEngineeringModeCommandAck &d) {
    o["status"] = d.status();
}

template<>
void to_json(JsonObject &o, const ld2410::RangeSensitivityConfigurationCommandAck &d) {
    o["status"] = d.status();
}

template<>
void to_json(JsonObject &o, const ld2410::ReadFirmwareVersionCommandAck &d) {
    o["firmware_type"] = d.firmware_type();
    o["major_version_number"] = d.major_version_number();
    o["minor_version_number"] = d.minor_version_number();
}

template<>
void to_json(JsonObject &o, const ld2410::SetSerialPortBaudRateAck &d) {
    o["status"] = d.status();
}


template<>
void to_json(JsonObject &o, const ld2410::FactoryResetAck &d) {
    o["status"] = d.status();
}

template<>
void to_json(JsonObject &o, const ld2410::RestartModuleAck &d) {
    o["status"] = d.status();
}

template<>
void to_json(JsonObject &o, const ld2410::ReportingDataFrame &d) {
    o["target_state"] = d.target_state();
    o["movement_target_distance"] = d.movement_target_distance();
    o["exercise_target_energy_value"] = d.exercise_target_energy_value();
    o["stationary_target_distance"] = d.stationary_target_distance();
    o["stationary_target_energy_value"] = d.stationary_target_energy_value();
    o["detection_distance"] = d.detection_distance();
    o["tail"] = d.tail();
    o["check"] = d.check();
}

template<>
void to_json(JsonObject &o, const ld2410::EngineeringModeDataFrame &d) {
    o["target_state"] = d.target_state();
    o["movement_target_distance"] = d.movement_target_distance();
    o["exercise_target_energy_value"] = d.exercise_target_energy_value();
    o["stationary_target_distance"] = d.stationary_target_distance();
    o["stationary_target_energy_value"] = d.stationary_target_energy_value();
    o["detection_distance"] = d.detection_distance();
    o["maximum_moving_distance_gate_n"] = d.maximum_moving_distance_gate_n();
    o["maximum_static_distance_gate_n"] = d.maximum_static_distance_gate_n();

    auto movement_distance_gate_energy_value = o["movement_distance_gate_energy_value"].to<JsonArray>();
    for(auto v: d.movement_distance_gate_energy_value()) {
        movement_distance_gate_energy_value.add(v);
    }
    auto static_distance_gate_energy_value = o["static_distance_gate_energy_value"].to<JsonArray>();
    for(auto v: d.static_distance_gate_energy_value()) {
        static_distance_gate_energy_value.add(v);
    }
}

}