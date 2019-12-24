#pragma once

#include "web/WebService.h"

// TODO: remove
class ConfigurationWebService : public WebService
{

public:
    void setup()
    {
        extra_pre_html += "  <a href=/settings>settings</a><br/>";
        WebService::setup();
        on("/settings", [&]()
        {
            Serial.println("ConfigurationWebService::on: /settings");
            send(200, "text/plain", "foo");
        });
    }
};
