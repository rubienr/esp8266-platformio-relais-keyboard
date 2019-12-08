#pragma once

#include "RelayEventsActions.h"
#include "RelayStrings.h"

#include "RelayEventsActions.h"
#include <HardwareSerial.h>
#include <Esp.h>

struct RelaysActionHelper
{
    using Impl = KeyEventsRelaysAction::Wrapped::Wrapped;

    explicit RelaysActionHelper(const Impl &impl);

    void print(String indent = "  ");

    const Impl &impl;
};

struct EventTypeRelaysActionHelper
{
    using Impl = KeyEventsRelaysAction::Wrapped;
    using Helper = RelaysActionHelper;

    explicit EventTypeRelaysActionHelper(const Impl &impl);

    void print(String indent = "  ");

    const Impl &impl;
};

struct KeyEventsRelaysActionHelper
{
    using Impl = KeyEventsRelaysAction;
    using Helper = EventTypeRelaysActionHelper;

    explicit KeyEventsRelaysActionHelper(const Impl &impl);

    void print(String indent = "  ");

    const Impl &impl;
};
