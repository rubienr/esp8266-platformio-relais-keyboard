#pragma once

#include <relay_board/settings/RelaySettingsStorage.h>
#include "web/WebService.h"

class RelayBoardWebService : public WebService
{

public:
    explicit RelayBoardWebService(RelaySettingsStorage &settings);

    void setup();

    void getRelaySettings();

    void setRelaySettings();

protected:
    RelaySettingsStorage &settings;

    void getRelaySettingsHtml();

    //void getJqueryMinJs();

    void getFileContent(const String &file_name, String &html);

    void getScriptsJs();
};
