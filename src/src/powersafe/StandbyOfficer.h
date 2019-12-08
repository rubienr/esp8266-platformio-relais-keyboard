#pragma once

#include <Arduino.h>
#include <elapsedMillis.h>

struct StandbyOfficer
{
    explicit StandbyOfficer(unsigned long seconds_idle_before_standby = 60) :
            seconds_idle_before_standby(seconds_idle_before_standby)
    {}

    void process()
    {
        if (!enabled)
            return;

        if (elapsed_seconds >= seconds_idle_before_standby)
        {
            disable();
            digitalWrite(LED_BUILTIN, LOW);
            ESP.deepSleep(ESP.deepSleepMax());
        }
    }

    void enable()
    {
        elapsed_seconds = 0;
        enabled = true;
    }

    void disable()
    {
        enabled = false;
    }

    void reset()
    {
        seconds_idle_before_standby = 0;
    }

private:
    bool enabled{false};
    elapsedSeconds elapsed_seconds{0};
    unsigned long seconds_idle_before_standby;
};