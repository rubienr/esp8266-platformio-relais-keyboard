#pragma once

#include <KeyEvent.h>
#include <RelaysBoardTemplate.h>
#include <KeysEventsRelaysStateTemplate.h>
#include <KeysEventsRelaysStateStorageTemplate.h>

// relays board with 8 relays
using RelaysBoard = RelaysBoardTemplate<8>;

namespace {
constexpr uint8_t keys_count = KeyEvent::keysCount();
constexpr uint8_t events_count = KeyEvent::uint8FromKeyType(KeyEvent::Type::Released) + 1;
constexpr uint8_t relays_count = RelaysBoard::RELAYS_COUNT;
}

// key-event-relay status mapping
using KeysEventsRelaysState = KeysEventsRelaysStateTemplate<keys_count, events_count, relays_count>;

// key-event-relay-status storage
using KeysEventsRelaysStateStorage = KeysEventsRelaysStateStorageTemplate<KeysEventsRelaysState>;