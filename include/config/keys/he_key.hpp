#pragma once

#include <cstdint>
#include "config/keys/key.hpp"
#include "config/keys/key_type.hpp"
#include "definitions.hpp"

// Configuration for the hall effect keys of the keypad, containing the actuation points, calibration, sensitivities etc. of the key.
struct HEKey : Key
{
    // Default constructor for the HEKey struct for initializing the arrays in the Configuration struct.
    HEKey() : Key(KeyType::HallEffect, 0, '\0') {}

    // Initialize with the correct type for identifying the type of key that a Key object was initialized as (e.g. HEKey) and index.
    HEKey(uint8_t index, uint8_t adcIndex) : Key(KeyType::HallEffect, index, index >= 26 ? 'a' : (char)('z' - index), adcIndex) {}

    // Bool whether rapid trigger is enabled or not.
    bool rapidTrigger = true;

    // Bool whether continuous rapid trigger is enabled or not.
    bool continuousRapidTrigger = true;

    // The sensitivity of the rapid trigger algorithm when pressing up.
    uint16_t rapidTriggerUpSensitivity = TRAVEL_DISTANCE_IN_0_01MM / 20;

    // The sensitivity of the rapid trigger algorithm when pressing down.
    uint16_t rapidTriggerDownSensitivity = TRAVEL_DISTANCE_IN_0_01MM / 20;

    // The value below which the key is pressed and rapid trigger is active in rapid trigger mode.
    uint16_t lowerHysteresis = (uint16_t)(TRAVEL_DISTANCE_IN_0_01MM * 0.885);

    // The value below which the key is no longer pressed and rapid trigger is no longer active in rapid trigger mode.
    uint16_t upperHysteresis = (uint16_t)(TRAVEL_DISTANCE_IN_0_01MM * 0.115);

    // The value read when the keys are in rest position/all the way down.
    uint16_t restPosition = pow(2, ANALOG_RESOLUTION) - 1; // Set to the outer boundaries in order to make
    uint16_t downPosition = 0;                             // them overwritable by the calibration code.
};
