#pragma once

#include <ArduinoJson.h>

struct Settings
{

    explicit Settings(const String &file_path = "/settings.json");

    void setup();

    int8_t loadSettings();

    int8_t saveSettings();

    JsonVariant getDocument();

    void print() const;

    void printMemoryUsage() const;

protected:

    int8_t loadSettings(const char *file_name);

    int8_t saveSettings(const char *file_name, JsonVariant config);

    String absolute_file_path;

    StaticJsonDocument<3800> document{};
};

