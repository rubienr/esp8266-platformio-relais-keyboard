#include <HardwareSerial.h>
#include <WiFiManager.h>
#include <main/KeyEventHandler.h>
#include "display/Display.h"
#include "main/OperatingMode.h"
#include "expansion_board/ExpansionBoard.h"
#include "relay_board/settings/RelayEventsActionHelper.h"
#include "relay_board/settings/RelaySettingsStorage.h"

struct Resources
{
    OperatingMode operating_mode;
    Keyboard keyboard;
    Display display;
    RelaySettingsStorage settings;
    ExpansionBoard io_expander;
    RelaysBoard relays_board{io_expander};
    StandbyOfficer standby_officer{999};
    KeyEventsRelaysAction relays_actions;
    KeyEventHandler event_handler{operating_mode, standby_officer, display, relays_actions, relays_board};


    void setup()
    {
        // --- serial setup
        [&]()
        {
            Serial.begin(230400);
            while (!Serial)
                delay(10);
            Serial.println("\nsetup");
        }();

        // --- load settings
        [&]()
        {
            settings.setup();

            // --- load last state
            settings.readKeyRelayActions(relays_actions);

            // TODO RR - remove defaults
            relays_actions.setDefaults(KeyEvent::Type::Pressed, Relay::Actuation::Activate);
            relays_actions.setDefaults(KeyEvent::Type::Released, Relay::Actuation::Release);
            //settings.writeKeyRelayActions(relays_actions);
            // TODO END

            // --- informational output
            KeyEventsRelaysActionHelper<>(relays_actions).print();
            settings.print();
            settings.printMemoryUsage();
        }();

        // --- restore physical state
        [&]()
        {
            // --- restore last stored state
            io_expander.setup(settings.readRelayFlags());

            relays_board.setup();
        }();

        // --- ui periphery
        [&]()
        {
            display.setup();
            keyboard.setup(&event_handler);
        }();

        // -- power save
        [&]()
        {
            // --- LED
            pinMode(LED_BUILTIN, OUTPUT);
            digitalWrite(LED_BUILTIN, HIGH);
            wifi_status_led_uninstall();

            // --- auto standby
            standby_officer.enable();
        }();
    }

    void process()
    {
        keyboard.process();
    }
} r;

void setup()
{ r.setup(); }

void loop()
{ r.process(); }
