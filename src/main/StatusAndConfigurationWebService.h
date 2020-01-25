#pragma once

#include "main/static_settings.h"
#include "web/WebService.h"

class StatusAndConfigurationWebService : public WebService {

public:
    explicit StatusAndConfigurationWebService(KeysEventsRelaysStateStorage &relays_state_storage, KeysEventsRelaysState &relays_state);

    void setup() override;

    void getRelaySettings();

    void saveRelaySettings();

protected:
    KeysEventsRelaysStateStorage &relays_state_storage;
    KeysEventsRelaysState &relays_state;

    void getRelaySettingsHtml();

    void getFileContent(const String &file_name, String &html);

    void getScriptsJs(bool compressed);

    void getCss(bool compressed);
};
