#include "RelayEventsActionHelper.h"

#include <type_traits>
#include <Esp.h>

void RelaysActionHelper::print(String indent)
{
    Serial.print(indent + relay::settings::names::KeyEventRelaysAction::RelaysAction::RELAYS_ACTION);
    Serial.print("=[");
    String separator = "";

    for (uint8_t r = 0; r < KeyEventsRelaysAction::Wrapped::Wrapped::RELAYS_COUNT; r++)
    {
        Serial.printf("%s%d", separator.c_str(),
                      static_cast<std::underlying_type<Relay::Actuation>::type>(impl.relay_actuation[r]));
        if (separator.length() == 0)
        {
            separator += ", ";
        }
    }
    Serial.println("]");
}

RelaysActionHelper::RelaysActionHelper(const RelaysActionHelper::Impl &impl) : impl(impl)
{}

EventTypeRelaysActionHelper::EventTypeRelaysActionHelper(const EventTypeRelaysActionHelper::Impl &impl) : impl(impl)
{}

void EventTypeRelaysActionHelper::print(String indent)
{
    Serial.println(indent + relay::settings::names::KeyEventRelaysAction::RelaysAction::SCOPE + ":");
    indent += "  ";

    for (uint8_t e = 0; e < KeyEventsRelaysAction::Wrapped::KEY_EVENT_TYPES_COUNT; e++)
    {
        String m(indent + relay::settings::names::KeyEventRelaysAction::RelaysAction::KEY_EVENT_TYPE);
        m += "=";
        m += e;
        Serial.println(m);
        Helper(impl.event_type[e]).print(indent + "  ");
    }
}

KeyEventsRelaysActionHelper::KeyEventsRelaysActionHelper(const KeyEventsRelaysActionHelper::Impl &impl) : impl(impl)
{}

void KeyEventsRelaysActionHelper::print(String indent)
{
    Serial.println(indent + relay::settings::names::KeyEventRelaysAction::SCOPE + ":");
    indent += "  ";

    for (uint8_t k = 0; k < Impl::KEYS_COUNT; k++)
    {
        ESP.wdtFeed();
        Serial.print(indent + relay::settings::names::KeyEventRelaysAction::KEY_CODE + "=");
        Serial.println(k);
        Helper(impl.key_code[k]).print(indent + "  ");
    }
}
