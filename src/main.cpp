#include <Arduino.h>
#include <EEPROM.h>
#include <Keyboard.h>
#include "config/configuration_controller.hpp"
#include "handlers/serial_handler.hpp"
#include "handlers/keypad_handler.hpp"
#include "definitions.hpp"
#include <hardware/spi.h>
void setup()
{
    // Initialize the EEPROM with 1024 bytes and load the configuration from it.
    EEPROM.begin(1024);
    ConfigController.loadConfig();

    // Initialize the serial and HID interface.
    Serial.begin(115200);
    Keyboard.begin();
    Keyboard.setAutoReport(false);

    // Set the amount of bits for the ADC to the defined one for a better resolution on the analog readings.
    analogReadResolution(ANALOG_RESOLUTION);

    // Allows to boot into UF2 bootloader mode by pressing the reset button twice.
    rp2040.enableDoubleResetBootloader();

    // Set digital pins to support pullup
    for(int i = 0; i < DIGITAL_KEYS; i++)
        pinMode(i, INPUT_PULLUP);

    //Set SPI Here
    //Could use secondary spi for better pollrate maybe but would make digital key implementation harder
    if (ADC_KEYS > 0)
    {

        spi_init (spi0, SPI_POLLRATE * 1000);
        spi_set_format (spi0, 8, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
        gpio_set_function(pin_SPI0SCK, GPIO_FUNC_SPI);
        gpio_set_function(pin_SPI0TX, GPIO_FUNC_SPI);
        gpio_set_function(pin_SPI0RX, GPIO_FUNC_SPI);
        //CS pins
        gpio_init(pin_SPI0CS1);
        gpio_set_dir(pin_SPI0CS1, GPIO_OUT);
        gpio_put(pin_SPI0CS1, 1);

    }


}

void loop()
{
    // Run the keypad handler checks to handle the actual keypad functionality.
    KeypadHandler.handle();
}

void serialEvent()
{
    // Handle incoming serial data.
    while(Serial.available() > 0)
    {
        // Read the incoming serial data until a newline into a buffer and terminate it with a null terminator.
        char input[SERIAL_INPUT_BUFFER_SIZE];
        const size_t inputLength = Serial.readBytesUntil('\n', input, SERIAL_INPUT_BUFFER_SIZE);
        input[inputLength] = '\0';

        // Pass the read input to the serial handler to handle it.
        SerialHandler.handleSerialInput(input);
    }
}
