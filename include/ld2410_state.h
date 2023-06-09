#pragma once

#include <cstdint>
#include <variant>
#include <vector>
#include <map>
#include <ld2410.h>

#include <ArduinoJson.h>

namespace ld2410_rest {

class Ld2410State {
private:
    std::optional<std::variant<ld2410::ReportingDataFrame, ld2410::EngineeringModeDataFrame>> m_detection;
    bool m_is_detected;

public:

    Ld2410State(): m_detection(std::nullopt), m_is_detected(false) {

    }

    void set_detection(decltype(m_detection) &&v) {
        m_detection = v;
    }

    const decltype(m_detection) get_detection() const {
        return m_detection;
    }

    void set_is_detected(bool is_detected) {
        m_is_detected = is_detected;
    }

    void is_detected(bool is_detected) {
        m_is_detected = is_detected;
    }

    bool is_detected() const {
        return m_is_detected;
    }

    void to_json(JsonDocument &doc) const {
        auto result = doc.to<JsonObject>();

        if (m_detection.has_value()) {
            if (std::holds_alternative<ld2410::ReportingDataFrame>(*m_detection)) {
                auto frame = std::get<ld2410::ReportingDataFrame>(*m_detection);
                result["state"]["target_state"] = frame.target_state();
                result["state"]["movement_target_distance"] = frame.movement_target_distance();
                result["state"]["exercise_target_energy_value"] = frame.exercise_target_energy_value();
                result["state"]["stationary_target_distance"] = frame.stationary_target_distance();
                result["state"]["stationary_target_energy_value"] = frame.stationary_target_energy_value();
                result["state"]["detection_distance"] = frame.detection_distance();
            } else {
                auto frame = std::get<ld2410::EngineeringModeDataFrame>(*m_detection);
                result["state"]["target_state"] = frame.target_state();
                result["state"]["movement_target_distance"] = frame.movement_target_distance();
                result["state"]["exercise_target_energy_value"] = frame.exercise_target_energy_value();
                result["state"]["stationary_target_distance"] = frame.stationary_target_distance();
                result["state"]["stationary_target_energy_value"] = frame.stationary_target_energy_value();
                result["state"]["detection_distance"] = frame.detection_distance();
                auto arr = result["state"]["movement_distance_gate_energy_value"].to<JsonArray>();
                for (auto v: frame.movement_distance_gate_energy_value()) {
                    arr.add(v);
                }

                auto arr2 = result["state"]["static_distance_gate_energy_value"].to<JsonArray>();
                for (auto v: frame.static_distance_gate_energy_value()) {
                    arr2.add(v);
                }
            }
        }

        result["xv"]["test"] = "abc";
        result["detected"] = m_is_detected;
    }
};

}