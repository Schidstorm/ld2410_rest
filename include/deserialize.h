#pragma once

#include <ld2410.h>
#include <MsgPack.h>

namespace ld2410_rest {

    class EngineeringModeDataFrame: public ld2410::EngineeringModeDataFrame {
        MSGPACK_DEFINE(m_target_state, m_movement_target_distance, m_exercise_target_energy_value, m_stationary_target_distance, m_stationary_target_energy_value, m_detection_distance, m_maximum_moving_distance_gate_n, m_maximum_static_distance_gate_n, m_movement_distance_gate_energy_value, m_static_distance_gate_energy_value)
    };

    class ReportingDataFrame: public ld2410::ReportingDataFrame {
        MSGPACK_DEFINE(m_target_state, m_movement_target_distance, m_exercise_target_energy_value, m_stationary_target_distance, m_stationary_target_energy_value, m_detection_distance, m_tail, m_check)
    };

    class EnableConfigurationCommandAck: public ld2410::EnableConfigurationCommandAck {
        MSGPACK_DEFINE(m_status, m_protocol_version, m_buffer)
    };

    class EnableConfigurationCommand: public ld2410::EnableConfigurationCommand {
        MSGPACK_DEFINE(m_value)

    public:
        using ack_t = EnableConfigurationCommandAck;
    };

    class EndConfigurationCommandAck: public ld2410::EndConfigurationCommandAck {
        MSGPACK_DEFINE(m_status)
    };

    class EndConfigurationCommand: public ld2410::EndConfigurationCommand {

    public:
        using ack_t = EndConfigurationCommandAck;

    };

    class MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck: public ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck {
        MSGPACK_DEFINE(m_status)
    };

    class MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand: public ld2410::MaximumDistanceGateandUnmannedDurationParameterConfigurationCommand {
        MSGPACK_DEFINE(m_maximum_moving_distance_word, m_maximum_moving_distance_parameter, m_maximum_static_distance_door_word, m_maximum_static_distance_door_parameter, m_no_person_duration, m_section_unattended_duration)

    public:
        using ack_t = MaximumDistanceGateandUnmannedDurationParameterConfigurationCommandAck;
    };

    class ReadParameterCommandAck: public ld2410::ReadParameterCommandAck {
        MSGPACK_DEFINE(m_status, m_header, m_maximum_distance_gate_n, m_configure_maximum_moving_distance_gate, m_configure_maximum_static_gate, m_distance_gate_motion_sensitivity, m_distance_gate_rest_sensitivity, m_no_time_duration)
    };

    class ReadParameterCommand: public ld2410::ReadParameterCommand {
    public:
        using ack_t = ReadParameterCommandAck;
    };

    class EnableEngineeringModeCommandAck: public ld2410::EnableEngineeringModeCommandAck {
        MSGPACK_DEFINE(m_status)
    };

    class EnableEngineeringModeCommand: public ld2410::EnableEngineeringModeCommand {
    public:
        using ack_t = EnableEngineeringModeCommandAck;
    };

    class CloseEngineeringModeCommandAck: public ld2410::CloseEngineeringModeCommandAck {
        MSGPACK_DEFINE(m_status)
    };

    class CloseEngineeringModeCommand: public ld2410::CloseEngineeringModeCommand {
    public:
        using ack_t = CloseEngineeringModeCommandAck;
    };

    class RangeSensitivityConfigurationCommandAck: public ld2410::RangeSensitivityConfigurationCommandAck {
        MSGPACK_DEFINE(m_status)
    };

    class RangeSensitivityConfigurationCommand: public ld2410::RangeSensitivityConfigurationCommand {
        MSGPACK_DEFINE(m_distance_gate_word, m_distance_gate_value, m_motion_sensitivity_word, m_motion_sensitivity_value, m_static_sensitivity_word, m_static_sensitivity_value)

    public:
        using ack_t = RangeSensitivityConfigurationCommandAck;
    };

    class ReadFirmwareVersionCommandAck: public ld2410::ReadFirmwareVersionCommandAck {
        MSGPACK_DEFINE(m_firmware_type, m_major_version_number, m_minor_version_number)
    };

    class ReadFirmwareVersionCommand: public ld2410::ReadFirmwareVersionCommand {

    public:
        using ack_t = ReadFirmwareVersionCommandAck;
    };

    class SetSerialPortBaudRateAck: public ld2410::SetSerialPortBaudRateAck {
        MSGPACK_DEFINE(m_status)
    };

    LD2410_PACKET SetSerialPortBaudRate {
        LD2410_PROP(uint16_t, baudRate_selection_index)

    public:
        static inline constexpr ld2410::to_bytes_union<uint32_t> definition_header{ld2410::CommandHeader};
        static inline constexpr ld2410::to_bytes_union<uint32_t> definition_mfr{ld2410::CommandMFR};
        static inline constexpr ld2410::to_bytes_union<uint16_t> definition_type{0x00a1};
        using ack_t = SetSerialPortBaudRateAck;

        template <typename TWriter>
        void write(TWriter &writer) const {
            LD2410_WRITE_SHORT(baudRate_selection_index);
        }

        static const size_t size() {
            size_t size_ = 0;
            size_ += sizeof(SetSerialPortBaudRate::m_baudRate_selection_index);
            return size_;
        }
    };

    

    class FactoryResetAck: public ld2410::FactoryResetAck {
        MSGPACK_DEFINE(m_status)
    };

    class FactoryReset: public ld2410::FactoryReset {

    public:
        using ack_t = FactoryResetAck;
    };

    class RestartModuleAck: public ld2410::RestartModuleAck {
        MSGPACK_DEFINE(m_status)
    };

    class RestartModule: public ld2410::RestartModule {

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