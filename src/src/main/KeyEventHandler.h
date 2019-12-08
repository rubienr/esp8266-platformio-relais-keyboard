#pragma once

#include <Arduino.h>
#include <display/Display.h>
#include "OperatingMode.h"
#include "powersafe/StandbyOfficer.h"
#include "keyboard/KeyEventReceiver.h"
#include "keyboard/Keyboard.h"

class KeyEventHandler : public KeyEventReceiver
{
    OperatingMode &operating_mode;
    StandbyOfficer &standby_officer;
    Display &display;

public:

    explicit KeyEventHandler(OperatingMode &operating_mode, StandbyOfficer &standby_officer, Display &display) :
            operating_mode(operating_mode),
            standby_officer(standby_officer),
            display(display)
    {}

    bool take(KeyEvent e) override
    {
        digitalWrite(LED_BUILTIN, HIGH);
        display.clearDisplay();
        display.setCursor(0, 0);
        display.print("K");
        display.print(std::underlying_type<KeyEvent::Key>::type(e.key));
        display.print(" ");
        display.display();

        Serial.print("KeyEventHandler::take: key=");
        Serial.print(std::underlying_type<KeyEvent::Key>::type(e.key));
        Serial.print(" type=");
        Serial.print(std::underlying_type<KeyEvent::Type>::type(e.type));
        Serial.print(" repeated=");
        Serial.println(e.repeated);

        bool consumed = false;
        standby_officer.reset();

        if (!consumed && e.type == KeyEvent::Type::DoublePressed)
        {
            switch (e.key)
            {
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

        if (!consumed && e.type == KeyEvent::Type::Repeated)
        {
            const uint8_t long_press = 8;
            const uint8_t extra_long_press = 2 * long_press;

            switch (e.key)
            {
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
                    if (e.repeated >= extra_long_press)
                    {
                        operating_mode.setMode(OperatingMode::Mode::SwitchToWifi);
                        consumed = true;
                    }
                    break;
                case KeyEvent::Key::Key6:
                    break;
                case KeyEvent::Key::Key7:
                    break;
                case KeyEvent::Key::Key8:
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

        if (!consumed && (e.type == KeyEvent::Type::Pressed || e.type == KeyEvent::Type::Repeated))
        {
            switch (e.key)
            {
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
        return consumed;
        digitalWrite(LED_BUILTIN, LOW);
    }
};
