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

        // -- load settings

        KeyEventsRelaysAction a;
        KeyEventsRelaysActionHelper h(a);
        settings.readKeyRelayActions(a);

        h.print();
        settings.print();
        settings.printMemoryUsage();

        // --- LED

        pinMode(LED_BUILTIN, OUTPUT);
        digitalWrite(LED_BUILTIN, HIGH);
        wifi_status_led_uninstall();
    }

    void process()
    {
    }
} r;

void setup()
{ r.setup(); }

void loop()
{ r.process(); }
