#pragma once

#include "settings/SettingsStorage.h"
#include <stdint.h>
#include "KeyRelaySettings.h"

class KeyRelaySettingsStorage : public SettingsStorage {

public:
    KeyRelaySettingsStorage();

    //! loads settings from given argument
    //! \param a
    void loadKeyRelayActions(const KeyRelaySettings &a);

    //! writes settings to given argument
    //! \param a
    void writeKeyRelayActions(KeyRelaySettings &a) const;

    void resetDocument() {
        document.clear();
        is_document_empty = true;
    }

protected:
    int8_t loadSettings(const char *file_name) override;

    static void loadRelayActuation(const KeyRelaySettings::KeyCodeElementType::ElementType &a, JsonVariant node);

    static void writeRelayActuation(KeyRelaySettings::KeyCodeElementType::ElementType &a, const JsonVariant &node);

    static void loadKeyEventRelayAction(const KeyRelaySettings::KeyCodeElementType &a, JsonVariant node);

    static void writeKeyEventRelayAction(KeyRelaySettings::KeyCodeElementType &a, const JsonVariant &node);

    static void loadKeysName(const KeyRelaySettings::KeyNamesArrayType &names, JsonVariant node);

    static void writeKeysName(KeyRelaySettings::KeyNamesArrayType &names, const JsonVariant &node);

    static void loadRelaysName(const KeyRelaySettings::RelaysNameArrayType &names, JsonVariant node);

    static void writeRelaysName(KeyRelaySettings::RelaysNameArrayType &names, const JsonVariant &node);

    static void loadRelaysFlag(const uint16_t &flags, JsonVariant node);

    static void writeRelaysFlag(uint16_t &flags, const JsonVariant &node);

    static void writePersistFlag(bool &flag, const JsonVariant &node);

    static void loadPersistFlag(const bool &flag, JsonVariant node);

    bool is_document_empty{true};
};
