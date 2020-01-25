#include <HardwareSerial.h>
#include <WiFiManager.h>
#include <main/KeyEventHandler.h>
#include <main/StatusBar.h>
#include <Display.h>
#include <Countdown.h>
#include "main/OperatingMode.h"
#include <ExpansionBoard.h>
#include <KeyEventsRelaysStateHelper.h>
#include <KeysEventsRelaysStateStorageTemplate.h>
#include <WifiConfigOrFallbackAccesspointManager.h>
#include "main/StatusAndConfigurationWebService.h"

struct Resources
{
    struct EarlyInitializer
    {
        EarlyInitializer()
        {
            Serial.begin(230400);
            while (!Serial) delay(10);
            Serial.printf("\n\n\n");
#if defined(ESP8266)
            wifi_status_led_uninstall();
#endif
            pinMode(LED_BUILTIN, OUTPUT);
            digitalWrite(LED_BUILTIN, HIGH);
        }
    } early_init;

    OperatingMode operating_mode;
    Keyboard keyboard;
    Display display;
    StatusBar status_bar{operating_mode, display};
    KeysEventsRelaysStateStorage settings;
    ExpansionBoard io_expander;
    RelaysBoard relays_board{io_expander};
    Countdown standby_officer{30};
    KeysEventsRelaysState relays_state{};
    KeyEventHandler event_handler{operating_mode, standby_officer, display, relays_state, relays_board};
    StatusAndConfigurationWebService web_service{settings, relays_state};

    void setup()
    {
        // --- load relayboardsettings
        [&]()
        {
            // --- load last state
            settings.setup();
            settings.writeKeyRelayActions(relays_state);

            // --- informational output
            KeyEventsRelaysStateHelper<KeysEventsRelaysState>(relays_state).print();

            settings.print();
            settings.printMemoryUsage();
            //relayboardsettings.resetDocument();
        }();

        // --- restore physical state
        [&]()
        {
            // --- restore last stored state
            io_expander.setup(relays_state.relay_flags);
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
            // --- auto standby
            standby_officer.enable();
        }();
    }

    void process()
    {
        if (operating_mode == OperatingMode::Mode::Normal)
            processNormalMode();
        else if (operating_mode == OperatingMode::Mode::SwitchToWifi)
            processSwitchToWifi();
        else if (operating_mode == OperatingMode::Mode::Wifi)
            processWifi();
    }

    void processNormalMode()
    {
        standby_officer.process([]()
                                {});
        if (keyboard.process())
        {
            status_bar.update();
        }
    }

    void processSwitchToWifi()
    {
        WiFi.mode(WIFI_AP);
        status_bar.update();
        WifiConfigOrFallbackAccesspointManager foo(display);

        status_bar.update();
        web_service.setup();
        operating_mode.setMode(OperatingMode::Mode::Wifi);
    }

    void processWifi()
    {
        if (web_service.process())
        {
            status_bar.update();
        }
        processNormalMode();
    }
} r;

void setup()
{ r.setup(); }

void loop()
{ r.process();

}
