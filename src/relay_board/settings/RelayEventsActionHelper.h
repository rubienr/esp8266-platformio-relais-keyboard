#pragma once

#include "RelayEventsActions.h"
#include "RelayStrings.h"

#include <type_traits>
#include <Esp.h>
#include <HardwareSerial.h>


template<typename Wrapped>
struct Wrap
{
    explicit Wrap(Wrapped impl) : wrapped(wrapped)
    {}

protected:
    Wrapped wrapped;
};

template<typename Impl>
struct RelaysActionHelper : public Wrap<const Impl &>
{
    using Wrap<const Impl &>::wrapped;

    explicit RelaysActionHelper(const Impl &impl) : Wrap<const Impl &>(impl)
    {}

    void print(String indent = "  ") const
    {
        Serial.print(indent + relay::settings::names::KeyEventRelaysAction::RelaysAction::RELAYS_ACTION);
        Serial.print("=[");
        String separator = "";

        uint8_t r = 0;
        for (const auto &actuation : wrapped.relay_actuation)
        {
            Serial.printf("%s%d", separator.c_str(),
                          static_cast<std::underlying_type<Relay::Actuation>::type>(actuation));
            if (separator.length() == 0)
            {
                separator += ", ";
            }
            r++;
        }
        Serial.println("]");
    }
};

template<typename Impl>
struct EventTypeRelaysActionHelper : public Wrap<const Impl &>
{
    using Wrap<const Impl &>::wrapped;
    using NextHelper = RelaysActionHelper<typename Impl::ElementType>;

    explicit EventTypeRelaysActionHelper(const Impl &impl) : Wrap<const Impl &>(impl)
    {}

    void print(String indent = "  ") const
    {
        Serial.println(indent + relay::settings::names::KeyEventRelaysAction::RelaysAction::SCOPE + ":");
        indent += "  ";

        uint8_t i = 0;
        for (const auto &event_type : wrapped.event_type)
        {
            String m(indent + relay::settings::names::KeyEventRelaysAction::RelaysAction::KEY_EVENT_TYPE);
            m += "=";
            m += i++;
            Serial.println(m);
            NextHelper(event_type).print(indent + "  ");
        }
    }
};

template<typename Impl = KeyEventRelayAction>
struct KeyEventsRelaysActionHelper : public Wrap<const Impl &>
{
    using Wrap<const Impl &>::wrapped;
    using NextHelper = EventTypeRelaysActionHelper<typename Impl::ElementType>;

    explicit KeyEventsRelaysActionHelper(const Impl &impl) : Wrap<const Impl &>(impl)
    {}

    void print(String indent = "  ") const
    {
        Serial.println(indent + relay::settings::names::KeyEventRelaysAction::SCOPE + ":");
        indent += "  ";

        uint8_t k = 0;
        for (const auto &key : wrapped.key_code)
        {
            ESP.wdtFeed();
            Serial.print(indent + relay::settings::names::KeyEventRelaysAction::KEY_CODE + "=");
            Serial.println(k++);
            NextHelper(key).print(indent + "  ");
        }
    }
};
