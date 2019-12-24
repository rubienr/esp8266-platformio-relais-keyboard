#include "RelayBoardWebService.h"

#include <StreamUtils.h>
#include <FS.h>

RelayBoardWebService::RelayBoardWebService(RelaySettingsStorage &settings) : settings(settings)
{
    extra_post_html += "  <br/>";
    extra_post_html += "  <a href=/api/get>/api/settings/relay/get</a><br/>";
    extra_post_html += "  <a href=/api/settings/relay/set>/api/settings/relay/set</a><br/>";
    extra_post_html += "  <a href=/settings/relay>relay settings</a><br/>";
}

void RelayBoardWebService::setup()
{
    WebService::setup();

    on("/settings/relay", [&]()
    { getRelaySettingsHtml(); });

    on("/api/settings/relay/get", [&]()
    { getRelaySettings(); });
    on("/api/settings/relay/set", [&]()
    { setRelaySettings(); });

    //on("/html/jquery-min.js", [&]()
    //{ getJqueryMinJs(); });
    on("/html/scripts.js", [&]()
    { getScriptsJs(); });
}

void RelayBoardWebService::getRelaySettings()
{
    Serial.println("WebService::on: /api/settings/relay/get");
    String json;
    serializeJson(settings.getDocument(), json);
    send(200, "text/json", json);
}

void RelayBoardWebService::setRelaySettings()
{
    Serial.println("WebService::on: /api/settings/relay/set");
}

void RelayBoardWebService::getRelaySettingsHtml()
{
    Serial.println("WebService::on: /settings/relay");
    String file_name("/html/relay_conf.html");
    String html;
    getFileContent(file_name, html);
    send(200, "text/html", html);
}

/*void RelayBoardWebService::getJqueryMinJs()
{
    Serial.println("WebService::on: /html/jquery-min.js");
    String file_name("/html/jquery-3.4.1.min.js");
    String html;
    getFileContent(file_name, html);
    send(200, "text/javascript", html);
}*/

void RelayBoardWebService::getScriptsJs()
{
    Serial.println("WebService::on: /html/scripts.js");
    String file_name("/html/scripts.js");
    String html;
    getFileContent(file_name, html);
    send(200, "text/javascript", html);
}

void RelayBoardWebService::getFileContent(const String &file_name, String &html)
{
    SPIFFS.begin();
    File file = SPIFFS.open(file_name, "r");
    if (file)
    {
        ReadBufferingStream buffered_file{file, 64};
        while (buffered_file.available())
        {
            //delay(10);
            html.concat(buffered_file.readString());
        }
        file.close();
        SPIFFS.end();
    } else
    {
        html.concat("failed to open file: " + file_name);
    }

    SPIFFS.end();
}
