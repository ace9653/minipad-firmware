#pragma once

#include <cstdint>
#include "config/keys/key_type.hpp"

// The base configuration struct for the DigitalKey and HEKey struct, containing the common fields.
struct Key
{
    // Require every key to be initialized with a type, index and key char.
    Key(KeyType t, uint8_t i, char c)
    {
        type = t;
        index = i;
        keyChar = c;
    }

        Key(KeyType t, uint8_t i, char c, uint8_t a)
    {
        type = t;
        index = i;
        keyChar = c;
        externalADC = a;
    }

    // Used to identify the type of key that a Key object was initialized as (e.g. HEKey or DigitalKey).
    KeyType type;

    // The index of the key. This is hardcoded in the default config and is not changed.
    // It does not serve a config purpose but is instead for accessing the index from the DigitalKey object.
    uint8_t index;

    // The corresponding key sent via HID interface.
    char keyChar;

    //The index of the externalADC, 0 if unused
    uint8_t externalADC = 0;

    // Bools whether HID commands are sent on the key.
    bool hidEnabled = true;
};
