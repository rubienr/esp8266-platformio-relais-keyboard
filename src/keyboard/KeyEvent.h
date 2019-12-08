#pragma once

#include <stdint.h>

struct KeyEvent
{
    enum class Key : uint8_t
    {
        Key0, Key1, Key2, Key3, Key4, Key5, Key6, Key7, Key8, Key9, Key10, Key11, None, LastEnumeration
    };

    enum class Type : uint8_t
    {
        Pressed, Released, Repeated, DoublePressed, None, LastEnumeration
    };


    constexpr static uint8_t keyEnumerationsCount()
    {
        return static_cast<std::underlying_type<Key>::type>(Key::LastEnumeration);
    }

    constexpr static uint8_t keyTypeEnumerationsCount()
    {
        return static_cast<std::underlying_type<Type>::type>(Type::LastEnumeration);
    }

    static std::underlying_type<Key>::type uint8FromKey(Key ke)
    {
        return static_cast<std::underlying_type<Key>::type>(ke);
    }

    constexpr static uint8_t uint8FromKeyType(Type kt)
    {
        return static_cast<std::underlying_type<Type>::type>(kt);
    }

    static Type keyTypeFromUint8(uint8_t kt)
    {
        return static_cast<Type>(kt);
    }

    static Key keyFromUint8(uint8_t ke)
    {
        return static_cast<Key>(ke);
    }

    Key key{Key::None};
    Type type{Type::None};
    uint16_t repeated{0};
};




