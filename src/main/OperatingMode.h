#pragma once

#include <type_traits>
#include <HardwareSerial.h>

struct OperatingMode
{
    enum class Mode : uint8_t
    {
        Normal,
        SwitchToWifi,
        Wifi
    };

    static uint8_t uint8FromMode(const Mode mode)
    {
        return static_cast<std::underlying_type<Mode>::type>(mode);
    }

    static Mode modeFromUint8(const uint8_t mode)
    {
        return static_cast<Mode>(mode);
    }

    void setMode(Mode new_mode)
    {
        this->operator=(new_mode);
    }

    Mode getMode() const
    {
        return mode;
    }

    OperatingMode &operator=(const Mode &new_mode)
    {
        auto current = std::underlying_type<Mode>::type(mode);
        auto next = std::underlying_type<Mode>::type(new_mode);
        Serial.printf("OperatingMode::setMode: switching from %d to %d\n", current, next);

        this->mode = new_mode;

        return *this;
    }

    bool operator==(Mode other_mode) const
    {
        return this->mode == other_mode;
    }

    bool operator!=(Mode other_mode) const
    {
        return !(*this == other_mode);
    }

protected:
    Mode mode{Mode::Normal};
};
