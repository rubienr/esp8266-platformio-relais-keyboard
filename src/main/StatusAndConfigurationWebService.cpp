#include "StatusAndConfigurationWebService.h"

#include <StreamUtils.h>
#include <FS.h>

#if defined(ESP32)
#include <SPIFFS.h>
#endif

StatusAndConfigurationWebService::StatusAndConfigurationWebService(KeysEventsRelaysStateStorage &relays_state_storage, KeysEventsRelaysState &relays_state) : relays_state_storage(
        relays_state_storage), relays_state(relays_state) {
    extra_post_html += "  <br/>";
    //extra_post_html += "  <a href=/api/get>/api/relayboardsettings/relay/get</a>get relay configuration<br/>";
    extra_post_html += "  <a href=/api/relayboardsettings/relay/save>/api/relayboardsettings/relay/save</a><br/>";
    extra_post_html += "  <a href=/relayboardsettings/relay>Configure Relays</a><br/>";

}

void StatusAndConfigurationWebService::setup() {
    WebService::setup();

    on("/relayboardsettings/relay", [&]() {
        Serial.println("WebService::on: /relayboardsettings/relay/");
        getRelaySettingsHtml(); });

    on("/api/relayboardsettings/relay/get", [&]() {
        Serial.println("WebService::on: /api/relayboardsettings/relay/get");
        getRelaySettings(); });

    on("/api/relayboardsettings/relay/save", [&]() {
        Serial.println("WebService::on: /api/relayboardsettings/relay/save");
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

    //relayboardsettings.resetDocument();
    relays_state_storage.loadKeyRelayActions(relays_state);
    serializeJson(relays_state_storage.getDocumentRoot(), json);
    Serial.println(json);
    send(200, "text/json", json);
    //relayboardsettings.resetDocument();
}

void StatusAndConfigurationWebService::saveRelaySettings() {
    const char *data = arg("plain").c_str();
    auto payload_bytes = arg("plain").length();

    String failed_reason_template("failed to parse {b}B due to {r}");
    failed_reason_template.replace("{b}", String(payload_bytes));
    //relayboardsettings.resetDocument();
    DeserializationError e = deserializeJson(relays_state_storage.getDocument(), data);

    if (e) {
        String reason(failed_reason_template);
        reason.replace("{r}", e.c_str());
        Serial.printf("WebService::on: %s\n", reason.c_str());
        Serial.printf("WebService::on: unparsed data:  %s\n", data);

        String response(R"({"return":"false", "reason":"{e}"})");
        response.replace("{e}", reason);
        send(400, "text/json", response);
    } else { // success
        relays_state_storage.saveSettings();
        relays_state_storage.writeKeyRelayActions(relays_state);
#if defined (ESP8266)
        Serial.println(String("stack free ") + ESP.getFreeContStack());
#endif
        Serial.println(String("heap free ") + ESP.getFreeHeap());
        Serial.println(String("free sketch ") + ESP.getFreeSketchSpace());

        //relayboardsettings.resetDocument();
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
