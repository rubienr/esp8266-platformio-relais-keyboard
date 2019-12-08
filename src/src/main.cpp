#include <HardwareSerial.h>
#include <WiFiManager.h>
#include <main/KeyEventHandler.h>
#include "display/Display.h"
#include "main/OperatingMode.h"
#include "expansion_board/ExpansionBoard.h"
#include "relay_board/RelayEventsActionHelper.h"
#include "relay_board/RelaySettingsStorage.h"

struct Resources
{
    OperatingMode operating_mode;
    Keyboard keyboard;
    Display display;
    StandbyOfficer standby_officer{999};
    KeyEventHandler event_handler{operating_mode, standby_officer, display};
    RelaySettingsStorage settings;
    ExpansionBoard io_expander;

    void setup()
    {
        Serial.begin(230400);
        while (!Serial)
            delay(10);
        Serial.println("\nsetup");

        keyboard.setup(&event_handler);
        display.setup();
        standby_officer.enable();

        settings.setup();
        io_expander.setup(settings.readRelayFlags());
        pinMode(LED_BUILTIN, OUTPUT);


        Serial.println("load settings");

        KeyEventsRelaysAction a;
        KeyEventsRelaysActionHelper h(a);
        settings.readKeyRelayActions(a);

        Serial.println("store settings");
        uint8_t foo = (uint8_t) a.key_code[0].event_type[0].relay_actuation[0];
        a.key_code[0].event_type[0].relay_actuation[0] = static_cast<Relay::Actuation>(
                ++foo % 3);

        settings.writeKeyRelayActions(a);
        settings.saveSettings();
        h.print();

        settings.print();
        settings.printMemoryUsage();
    }

    void process()
    {
        digitalWrite(LED_BUILTIN, LOW);
    }
} r;

void setup()
{ r.setup(); }

void loop()
{ r.process(); }
