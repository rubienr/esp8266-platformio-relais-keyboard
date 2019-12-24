#include "StatusAndConfigurationWebService.h"

#include <StreamUtils.h>
#include <FS.h>

StatusAndConfigurationWebService::StatusAndConfigurationWebService(RelaySettingsStorage &settings, KeyEventRelayAction &relays_actions) : settings(
        settings), relays_actions(relays_actions) {
    extra_post_html += "  <br/>";
    //extra_post_html += "  <a href=/api/get>/api/settings/relay/get</a>get relay configuration<br/>";
    extra_post_html += "  <a href=/api/settings/relay/save>/api/settings/relay/save</a><br/>";
    extra_post_html += "  <a href=/settings/relay>Configure Relays</a><br/>";
}

void StatusAndConfigurationWebService::setup() {
    WebService::setup();

    on("/settings/relay", [&]() { getRelaySettingsHtml(); });

    on("/api/settings/relay/get", [&]() { getRelaySettings(); });
    on("/api/settings/relay/save", [&]() { saveRelaySettings(); });

    on("/html/scripts.js", [&]() { getScriptsJs(false); });
    on("/html/scripts-c.js", [&]() { getScriptsJs(true); });
    on("/html/min.css", [&]() { getCss(false); });
    on("/html/min-c.css", [&]() { getCss(true); });
}

void StatusAndConfigurationWebService::getRelaySettings() {
    Serial.println("WebService::on: /api/settings/relay/get");
    String json;
    serializeJson(settings.getDocumentRoot(), json);
    send(200, "text/json", json);
}

void StatusAndConfigurationWebService::saveRelaySettings() {
    Serial.println("WebService::on: /api/settings/relay/save");

    const char *data = arg("plain").c_str();
    auto payload_bytes = arg("plain").length();
    DynamicJsonDocument new_settings(settings.getDocument().capacity());
    new_settings.clear();

    String failed_reason_template("failed to parse {b}B due to {r}");
    failed_reason_template.replace("{b}", String(payload_bytes));
    DeserializationError e = deserializeJson(new_settings, data);
    if (e) { // json is invalid

        String reason(failed_reason_template);
        reason.replace("{r}", e.c_str());
        Serial.printf("WebService::on: %s\n", reason.c_str());
        Serial.printf("WebService::on: unparsed data:  %s\n", data);

        String response(R"({"return":"false", "reason":"{e}"})");
        response.replace("{e}", reason);
        send(400, "text/json", response);

    } else { // json is valid
        e = deserializeJson(settings.getDocument(), data);
        if (e) { // other deserialization error
            String reason(failed_reason_template);
            reason.replace("{r}", String(" server error: ") + e.c_str());
            Serial.printf("WebService::on:  %s\n", reason.c_str());

            String response(R"({"return":"false", "reason":"{e}"})");
            response.replace("{e}", e.c_str());
            send(500, "text/json", response);
        } else { // success
            settings.saveSettings();
            settings.readKeyRelayActions(relays_actions);
            send(200, "text/json", R"({"return":"true"})");
        }
    }
}

void StatusAndConfigurationWebService::getRelaySettingsHtml() {
    Serial.println("WebService::on: /settings/relay");
    String file_name("/html/relay_conf.html");
    String html;
    getFileContent(file_name, html);
    send(200, "text/html", html);
}

void StatusAndConfigurationWebService::getScriptsJs(bool compressed) {
    Serial.println("WebService::on: /html/scripts.js");

    String file_name("/html/scripts{c}.js");
    if (compressed) file_name.replace("{c}",("-c"));
    else file_name.replace("{c}",(""));

    String html;
    getFileContent(file_name, html);
    send(200, "text/javascript", html);
}

void StatusAndConfigurationWebService::getCss(bool compressed) {
    Serial.println("WebService::on: /html/min.css");

    String file_name("/html/min{c}.css");
    if (compressed) file_name.replace("{c}",("-c"));
    else file_name.replace("{c}",(""));

    String html;
    getFileContent(file_name, html);
    send(200, "text/css", html);
}

void StatusAndConfigurationWebService::getFileContent(const String &file_name, String &html) {
    SPIFFS.begin();
    File file = SPIFFS.open(file_name, "r");
    if (file) {
        ReadBufferingStream buffered_file{file, 64};
        while (buffered_file.available()) {
            //delay(10);
            html.concat(buffered_file.readString());
        }
        file.close();
        SPIFFS.end();
    } else {
        html.concat("failed to open file: " + file_name);
    }

    SPIFFS.end();
}
