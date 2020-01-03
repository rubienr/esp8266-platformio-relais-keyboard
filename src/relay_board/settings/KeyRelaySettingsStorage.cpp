#include "KeyRelaySettingsStorage.h"

#include <stdint.h>
#include <WString.h>

#include "KeyRelayStrings.h"

KeyRelaySettingsStorage::KeyRelaySettingsStorage() : SettingsStorage("/conf/key_relay_conf.json") {}

void KeyRelaySettingsStorage::loadKeyRelayActions(const KeyRelaySettings &a) {
    JsonVariant root = getDocumentRoot();
    JsonVariant keys_actions = root.getOrAddMember("ka");
    JsonVariant persist = root.getOrAddMember("persist");
    JsonVariant names = root.getOrAddMember("names");
    JsonVariant relays_flag = root.getOrAddMember("saved_state");
    JsonVariant keys_name = names.getOrAddMember("k");
    JsonVariant relays_name = names.getOrAddMember("r");

    loadPersistFlag(a.persist_state, persist);
    loadRelaysFlag(a.relay_flags, relays_flag);
    loadKeysName(a.keys_name, keys_name);
    loadRelaysName(a.relays_name, relays_name);

    uint8_t i = 0;
    for (const auto &r : a.key_code) {
        loadKeyEventRelayAction(r, keys_actions.getOrAddMember(String(i++)));
    }
}

void KeyRelaySettingsStorage::loadKeyEventRelayAction(const KeyRelaySettings::KeyCodeElementType &a, JsonVariant node) {
    uint8_t i = 0;

    for (const auto &e : a.event_type) {
        loadRelayActuation(e, node.getOrAddMember(String(i++)));
    }
}

void
KeyRelaySettingsStorage::loadRelayActuation(const KeyRelaySettings::KeyCodeElementType::ElementType &a,
                                            JsonVariant node) {
    uint8_t i = 0;

    for (const auto &r : a.relay_actuation) {
        JsonVariant array_element = node.getElement(i++);
        if (array_element.isNull()) {
            node.add(Relay::uint8FromActuation(r));
        } else {
            array_element.set(Relay::uint8FromActuation(r));
        }
    }
}

int8_t KeyRelaySettingsStorage::loadSettings(const char *file_name) {
    Serial.println("KeyRelaySettingsStorage::loadSettings: ...");
    uint8_t result = SettingsStorage::loadSettings(file_name);
    if (0 == result) {
        is_document_empty = false;
        Serial.println("KeyRelaySettingsStorage::loadSettings: failed to load document");
    }
    return result;
}

void KeyRelaySettingsStorage::loadKeysName(const KeyRelaySettings::KeyNamesArrayType &names, JsonVariant node) {
    uint8_t i = 0;

    for (const auto &name : names) {
        JsonVariant array_element = node.getElement(i++);
        if (array_element.isNull()) {
            node.add(name);
        } else {
            array_element.set(name);
        }
    }
}

void KeyRelaySettingsStorage::loadRelaysName(const KeyRelaySettings::RelaysNameArrayType &names, JsonVariant node) {
    uint8_t i = 0;

    for (const auto &name : names) {
        JsonVariant array_element = node.getElement(i++);
        if (array_element.isNull()) {
            node.add(name);
        } else {
            array_element.set(name);
        }
    }
}

void KeyRelaySettingsStorage::loadPersistFlag(const bool &flag, JsonVariant node) {
    node.set(flag);
}

void KeyRelaySettingsStorage::loadRelaysFlag(const uint16_t &flags, JsonVariant node) {
    node.set(flags);
}

void KeyRelaySettingsStorage::writeKeyRelayActions(KeyRelaySettings &a) const {

    if (is_document_empty) {
        Serial.println(
                "KeyRelaySettingsStorage::writeKeyRelayActions: failed to write settings since no document is loaded");
        return;
    }

    JsonVariant root = getDocumentRoot();
    JsonVariant keys_actions = root.getOrAddMember("ka");
    JsonVariant persist = root.getMember("persist");
    JsonVariant names = root.getMember("names");
    JsonVariant relays_flag = root.getMember("saved_state");
    JsonVariant keys_name = names.getMember("k");
    JsonVariant relays_name = names.getMember("r");

    writePersistFlag(a.persist_state, persist);
    writeRelaysFlag(a.relay_flags, relays_flag);
    writeKeysName(a.keys_name, keys_name);
    writeRelaysName(a.relays_name, relays_name);

    uint8_t i = 0;
    for (auto &r : a.key_code) writeKeyEventRelayAction(r, keys_actions.getOrAddMember(String(i++)));

}

void KeyRelaySettingsStorage::writeKeysName(KeyRelaySettings::KeyNamesArrayType &names, const JsonVariant &node) {
    uint8_t i = 0;

    if (!node.isNull())
        for (String &name : names) {//KeyNamesElementType
            JsonVariant value = node.getElement(i++);
            if (!value.isNull()) {
                name = value.as<String>();
            }
        }
}

void
KeyRelaySettingsStorage::writeRelaysName(KeyRelaySettings::RelaysNameArrayType &names, const JsonVariant &node) {
    uint8_t i = 0;

    for (auto &name : names) {
        JsonVariant value = node.getElement(i++);
        if (!value.isNull()) {
            name = value.as<String>();
        } else {
            name = i;
        }
    }
}

void KeyRelaySettingsStorage::writePersistFlag(bool &flag, const JsonVariant &node) {
    if (!node.isNull())
        flag = node.as<uint16_t>();
}

void KeyRelaySettingsStorage::writeRelaysFlag(uint16_t &flags, const JsonVariant &node) {
    if (!node.isNull())
        flags = node.as<uint16_t>();
}

void
KeyRelaySettingsStorage::writeKeyEventRelayAction(KeyRelaySettings::KeyCodeElementType &a, const JsonVariant &node) {
    uint8_t i = 0;
    for (auto &e : a.event_type) writeRelayActuation(e, node.getMember(String(i++)));
}

void
KeyRelaySettingsStorage::writeRelayActuation(KeyRelaySettings::KeyCodeElementType::ElementType &a,
                                             const JsonVariant &node) {
    uint8_t i = 0;

    if (!node.isNull())
        for (auto &r : a.relay_actuation) {
            JsonVariant value = node.getElement(i++);
            if (!value.isNull()) {
                r = Relay::actuationFromUint8(value.as<uint8_t>());
            }
        }
}
