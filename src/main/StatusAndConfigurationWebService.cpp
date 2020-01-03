#include "StatusAndConfigurationWebService.h"

#include <StreamUtils.h>
#include <FS.h>

StatusAndConfigurationWebService::StatusAndConfigurationWebService(KeyRelaySettingsStorage &settings, KeyRelaySettings &relays_actions) : settings(
        settings), relays_actions(relays_actions) {
    extra_post_html += "  <br/>";
    //extra_post_html += "  <a href=/api/get>/api/settings/relay/get</a>get relay configuration<br/>";
    extra_post_html += "  <a href=/api/settings/relay/save>/api/settings/relay/save</a><br/>";
    extra_post_html += "  <a href=/settings/relay>Configure Relays</a><br/>";

}

void StatusAndConfigurationWebService::setup() {
    WebService::setup();

    on("/settings/relay", [&]() {
        Serial.println("WebService::on: /settings/relay/");
        getRelaySettingsHtml(); });

    on("/api/settings/relay/get", [&]() {
        Serial.println("WebService::on: /api/settings/relay/get");
        getRelaySettings(); });

    on("/api/settings/relay/save", [&]() {
        Serial.println("WebService::on: /api/settings/relay/save");
        saveRelaySettings(); });

    on("/html/scripts.js", [&]() {
        Serial.println("WebService::on: /html/scripts.js");
        getScriptsJs(false); });

    on("/html/scripts-c.js", [&]() {
        Serial.println("WebService::on: /html/scripts-c.js");
        getScriptsJs(true); });

    on("/html/min.css", [&]() {
        Serial.println("WebService::on: /html/min.css");
        getCss(false); });

    on("/html/min-c.css", [&]() {
        Serial.println("WebService::on: /html/min-c.css");
        getCss(true); });
}

void StatusAndConfigurationWebService::getRelaySettings() {
    String json;

    //settings.resetDocument();
    settings.loadKeyRelayActions(relays_actions);
    serializeJson(settings.getDocumentRoot(), json);
    Serial.println(json);
    send(200, "text/json", json);
    //settings.resetDocument();
}

void StatusAndConfigurationWebService::saveRelaySettings() {
    const char *data = arg("plain").c_str();
    auto payload_bytes = arg("plain").length();

    String failed_reason_template("failed to parse {b}B due to {r}");
    failed_reason_template.replace("{b}", String(payload_bytes));
    //settings.resetDocument();
    DeserializationError e = deserializeJson(settings.getDocument(), data);

    if (e) {
        String reason(failed_reason_template);
        reason.replace("{r}", e.c_str());
        Serial.printf("WebService::on: %s\n", reason.c_str());
        Serial.printf("WebService::on: unparsed data:  %s\n", data);

        String response(R"({"return":"false", "reason":"{e}"})");
        response.replace("{e}", reason);
        send(400, "text/json", response);
    } else { // success
        settings.saveSettings();
        settings.writeKeyRelayActions(relays_actions);
        Serial.println(String("stack free ") + ESP.getFreeContStack());
        Serial.println(String("heap free ") + ESP.getFreeHeap());
        Serial.println(String("free sketch ") + ESP.getFreeSketchSpace());

        //settings.resetDocument();
        send(200, "text/json", R"({"return":"true"})");
    }

}

void StatusAndConfigurationWebService::getRelaySettingsHtml() {
    String file_name("/html/relay_conf.html");
    String html;
    getFileContent(file_name, html);
    send(200, "text/html", html);
}

void StatusAndConfigurationWebService::getScriptsJs(bool compressed) {
    String file_name("/html/scripts{c}.js");
    if (compressed) file_name.replace("{c}",("-c"));
    else file_name.replace("{c}",(""));

    String html;
    getFileContent(file_name, html);
    send(200, "text/javascript", html);
}

void StatusAndConfigurationWebService::getCss(bool compressed) {
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
