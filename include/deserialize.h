#pragma once

#include <ld2410.h>
#include <MsgPack.h>

namespace ld2410_rest {

    template<typename T>
    class Command;

    template<typename T>
    Command<T> createCommand(T val) {
        return (Command<T>)val;
    }

    template<typename T = ld2410::EngineeringModeDataFrame>
    class Command: public ld2410::EngineeringModeDataFrame {
        MSGPACK_DEFINE(m_target_state, m_movement_target_distance, m_exercise_target_energy_value, m_stationary_target_distance, m_stationary_target_energy_value, m_detection_distance, m_maximum_moving_distance_gate_n, m_maximum_static_distance_gate_n, m_movement_distance_gate_energy_value, m_static_distance_gate_energy_value)
    };

    template<typename T = ld2410::ReportingDataFrame>
    class Command: public ld2410::ReportingDataFrame {
        MSGPACK_DEFINE(m_target_state, m_movement_target_distance, m_exercise_target_energy_value, m_stationary_target_distance, m_stationary_target_energy_value, m_detection_distance, m_tail, m_check)
    };

    template<typename T = ld2410::EnableConfigurationCommandAck>
    class Command: public ld2410::EnableConfigurationCommandAck {
        MSGPACK_DEFINE(m_status, m_protocol_version, m_buffer)
    };

    template<typename T = ld2410::EnableConfigurationCommand>
    class Command: public ld2410::EnableConfigurationCommand {
        MSGPACK_DEFINE(m_value)

    public:
        using ack_t = EnableConfigurationCommandAck;
    };

    template<typename T = ld2410::EndConfigurationCommandAck>
    class Command: public ld2410::EndConfigurationCommandAck {
        MSGPACK_DEFINE(m_status)
    };

    template<typename T = ld2410::EndConfigurationCommand>
    class Command: public ld2410::EndConfigurationCommand {

    public:
        using ack_t = EndConfigurationCommandAck;

    };

    template<typename T = ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck>
    class Command: public ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck {
        MSGPACK_DEFINE(m_status)
    };

    template<typename T = ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand>
    class Command: public ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand {
        MSGPACK_DEFINE(m_maximum_moving_distance_word, m_maximum_moving_distance_parameter, m_maximum_static_distance_door_word, m_maximum_static_distance_door_parameter, m_no_person_duration, m_section_unattended_duration)

    public:
        using ack_t = MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck;
    };

    template<typename T = ld2410::ReadParameterCommandAck>
    class Command: public ld2410::ReadParameterCommandAck {
        MSGPACK_DEFINE(m_status, m_header, m_maximum_distance_gate_n, m_configure_maximum_moving_distance_gate, m_configure_maximum_static_gate, m_distance_gate_motion_sensitivity, m_distance_gate_rest_sensitivity, m_no_time_duration)
    };

    template<typename T = ld2410::ReadParameterCommand>
    class Command: public ld2410::ReadParameterCommand {
    public:
        using ack_t = ReadParameterCommandAck;
    };

    template<typename T = ld2410::EnableEngineeringModeCommandAck>
    class Command: public ld2410::EnableEngineeringModeCommandAck {
        MSGPACK_DEFINE(m_status)
    };

    template<typename T = ld2410::EnableEngineeringModeCommand>
    class Command: public ld2410::EnableEngineeringModeCommand {
    public:
        using ack_t = EnableEngineeringModeCommandAck;
    };

    template<typename T = ld2410::CloseEngineeringModeCommandAck>
    class Command: public ld2410::CloseEngineeringModeCommandAck {
        MSGPACK_DEFINE(m_status)
    };

    template<typename T = ld2410::CloseEngineeringModeCommand>
    class Command: public ld2410::CloseEngineeringModeCommand {
    public:
        using ack_t = CloseEngineeringModeCommandAck;
    };

    template<typename T = ld2410::RangeSensitivityConfigurationCommandAck>
    class Command: public ld2410::RangeSensitivityConfigurationCommandAck {
        MSGPACK_DEFINE(m_status)
    };

    template<typename T = ld2410::RangeSensitivityConfigurationCommand>
    class Command: public ld2410::RangeSensitivityConfigurationCommand {
        MSGPACK_DEFINE(m_distance_gate_word, m_distance_gate_value, m_motion_sensitivity_word, m_motion_sensitivity_value, m_static_sensitivity_word, m_static_sensitivity_value)

    public:
        using ack_t = RangeSensitivityConfigurationCommandAck;
    };

    template<typename T = ld2410::ReadFirmwareVersionCommandAck>
    class Command: public ld2410::ReadFirmwareVersionCommandAck {
        MSGPACK_DEFINE(m_firmware_type, m_major_version_number, m_minor_version_number)
    };

    template<typename T = ld2410::ReadFirmwareVersionCommand>
    class Command: public ld2410::ReadFirmwareVersionCommand {

    public:
        using ack_t = ReadFirmwareVersionCommandAck;
    };

    template<typename T = ld2410::SetSerialPortBaudRateAck>
    class Command: public ld2410::SetSerialPortBaudRateAck {
        MSGPACK_DEFINE(m_status)
    };

    template<typename T = ld2410::SetSerialPortBaudRate>
    class Command: public ld2410::SetSerialPortBaudRate {
        MSGPACK_DEFINE(m_baudRate_selection_index)

    public:
        using ack_t = SetSerialPortBaudRateAck;
    };

    template<typename T = ld2410::FactoryResetAck>
    class Command: public ld2410::FactoryResetAck {
        MSGPACK_DEFINE(m_status)
    };

    template<typename T = ld2410::FactoryReset>
    class Command: public ld2410::FactoryReset {

    public:
        using ack_t = FactoryResetAck;
    };

    template<typename T = ld2410::RestartModuleAck>
    class Command: public ld2410::RestartModuleAck {
        MSGPACK_DEFINE(m_status)
    };

    template<typename T = ld2410::RestartModule>
    class Command: public ld2410::RestartModule {

    public:
        using ack_t = RestartModuleAck;
    };

    template <typename T>
    bool deserialize(const uint8_t* data, const size_t size, T &result) {
        MsgPack::Unpacker unpacker;
        unpacker.feed(data, size);
        return unpacker.deserialize(result);
    }

    template <typename T, typename THandleData>
    void serialize_and_write_to(const T &result, THandleData handle_data) {
        MsgPack::Packer packer;
        packer.serialize(result);
        handle_data(packer.data(), packer.size());
    }
}