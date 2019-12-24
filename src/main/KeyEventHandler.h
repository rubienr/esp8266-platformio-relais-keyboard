#pragma once

#include <Arduino.h>
#include <display/Display.h>
#include <relay_board/RelaysBoard.h>
#include <relay_board/settings/RelaySettingsStorage.h>
#include "OperatingMode.h"
#include "powersafe/StandbyOfficer.h"
#include "keyboard/KeyEventReceiver.h"
#include "keyboard/Keyboard.h"

class KeyEventHandler : public KeyEventReceiver {
    OperatingMode &operating_mode;
    StandbyOfficer &standby_officer;
    Display &display;
    KeyEventRelayAction &relay_settings;
    RelaysBoard &relays_board;

public:

    explicit KeyEventHandler(
            OperatingMode &operating_mode,
            StandbyOfficer &standby_officer,
            Display &display,
            KeyEventRelayAction &relay_settings,
            RelaysBoard &relays_board) :
            operating_mode(operating_mode),
            standby_officer(standby_officer),
            display(display),
            relay_settings(relay_settings),
            relays_board(relays_board) {}

    bool take(KeyEvent e) override {
        digitalWrite(LED_BUILTIN, LOW);
        display.reset();
        display.printf("KEY %d \n", std::underlying_type<KeyEvent::Key>::type(e.key));
        display.printf("PRE %d \n", (e.type == KeyEvent::Type::Pressed));
        display.printf("REL %d \n", (e.type == KeyEvent::Type::Released));
        display.printf("REP %d \n", e.repeated);
        display.printf("MODE %d \n", OperatingMode::uint8FromMode(operating_mode.getMode()));
        display.display();

/*      Serial.print("KeyEventHandler::take: key=");
        Serial.print(std::underlying_type<KeyEvent::Key>::type(e.key));
        Serial.print(" type=");
        Serial.print(std::underlying_type<KeyEvent::Type>::type(e.type));
        Serial.print(" repeated=");
        Serial.println(e.repeated);*/

        bool consumed = false;
        standby_officer.reset();


        if (!consumed && e.type == KeyEvent::Type::Repeated) {
            const uint8_t long_press = 8;
            const uint8_t extra_long_press = 2 * long_press;

            switch (e.key) {
                case KeyEvent::Key::Key0:
                    break;
                case KeyEvent::Key::Key1:
                    break;
                case KeyEvent::Key::Key2:
                    break;
                case KeyEvent::Key::Key3:
                    break;
                case KeyEvent::Key::Key4:
                    break;
                case KeyEvent::Key::Key5:
                    break;
                case KeyEvent::Key::Key6:
                    break;
                case KeyEvent::Key::Key7:
                    break;
                case KeyEvent::Key::Key8:
                    if (e.repeated >= extra_long_press && operating_mode == OperatingMode::Mode::Normal) {
                        operating_mode = OperatingMode::Mode::SwitchToWifi;
                        consumed = true;

                        display.clearDisplay();
                        display.setCursor(0, 0);
                        display.printf("MODE %d \n", OperatingMode::uint8FromMode(operating_mode.getMode()));
                        display.display();
                    }
                    break;
                case KeyEvent::Key::Key9:
                    break;
                case KeyEvent::Key::Key10:
                    break;
                case KeyEvent::Key::Key11:
                    break;
                case KeyEvent::Key::None:
                case KeyEvent::Key::LastEnumeration:
                    break;
            }
        }

        if (!consumed && ((e.type == KeyEvent::Type::Pressed) || (e.type == KeyEvent::Type::DoublePressed) ||
                          (e.type == KeyEvent::Type::Repeated))) {
            uint8_t key = KeyEvent::uint8FromKey(e.key);
            uint8_t event = KeyEvent::uint8FromKeyType(KeyEvent::Type::Pressed);
            const auto &actions = relay_settings.key_code[key].event_type[event].relay_actuation;
            relays_board.actuate(actions);
            consumed = true;
        } else {
            uint8_t key = KeyEvent::uint8FromKey(e.key);
            uint8_t event = KeyEvent::uint8FromKeyType(KeyEvent::Type::Released);
            const auto &actions = relay_settings.key_code[key].event_type[event].relay_actuation;
            relays_board.actuate(actions);
            consumed = true;
        }

        digitalWrite(LED_BUILTIN, HIGH);
        return consumed;
    }
};
