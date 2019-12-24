#pragma once

#include <relay_board/settings/RelaySettingsStorage.h>
#include "web/WebService.h"

class StatusAndConfigurationWebService : public WebService {

public:
    explicit StatusAndConfigurationWebService(RelaySettingsStorage &settings, KeyEventRelayAction &relays_actions);

    void setup();

    void getRelaySettings();

    void saveRelaySettings();

protected:
    RelaySettingsStorage &settings;
    KeyEventRelayAction &relays_actions;

    void getRelaySettingsHtml();

    void getFileContent(const String &file_name, String &html);

    void getScriptsJs(bool compressed);

    void getCss(bool compressed);
};
