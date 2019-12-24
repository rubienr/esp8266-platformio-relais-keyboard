#include "Settings.h"

#include <StreamUtils.h>
#include <FS.h>

void Settings::setup()
{
    loadSettings();
}

int8_t Settings::saveSettings()
{
    return saveSettings(absolute_file_path.c_str(), getDocument());
}

int8_t Settings::saveSettings(const char *file_name, JsonVariant config)
{
    if (!SPIFFS.begin())
        return -1;

    File file = SPIFFS.open(file_name, "w");
    if (!file)
    {
        SPIFFS.end();
        return -1;
    } else
    {
        WriteBufferingStream buffered_writer(file, 64);
        serializeJson(config, buffered_writer);
        buffered_writer.flush();
    }
    file.close();

    SPIFFS.end();
    return 0;
}

int8_t Settings::loadSettings()
{
    return loadSettings(absolute_file_path.c_str());
}

int8_t Settings::loadSettings(const char *file_name)
{
    Serial.println("Settings::loadSettings");

    if (!SPIFFS.begin())
    {
        Serial.println("Settings::loadSettings: failed to open spiffs");
        return -1;
    }

    File file = SPIFFS.open(file_name, "r");
    if (!file)
    {
        Serial.printf("Settings::loadSettings: failed to open file %s\n", file_name);
        SPIFFS.end();
        return -1;
    } else
    {
        ReadBufferingStream buffered_reader(file, 64);
        DeserializationError e = deserializeJson(document, buffered_reader);
        if (e)
        {
            Serial.printf("Settings::loadSettings: failed to load file due to %s\n", e.c_str());
            document.clear();
            file.close();
            SPIFFS.end();
            return -1;
        }
    }
    file.close();
    SPIFFS.end();
    Serial.println("Settings::loadSettings: loaded");
    return 0;
}

JsonVariant Settings::getDocument()
{
    return document.as<JsonVariant>();
}

void Settings::print() const
{
    Serial.println();
    serializeJson(document, Serial);
    Serial.println();
}

void Settings::printMemoryUsage() const
{
    Serial.printf("Settings::printMemoryUsage: consumed %d of %d\n", document.memoryUsage(),
                  document.capacity());
}

Settings::Settings(const String &file_path) : absolute_file_path(file_path)
{
}


