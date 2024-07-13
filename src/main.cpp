#include <Arduino.h>
#include <EEPROM.h>
#include <Keyboard.h>
#include "config/configuration_controller.hpp"
#include "handlers/serial_handler.hpp"
#include "handlers/key_handler.hpp"
#include "definitions.hpp"

#include <Arduino.h>
#include <CoreMutex.h>
#include <hardware/gpio.h>
#include <hardware/pwm.h>
#include <hardware/clocks.h>
#include <hardware/pll.h>
#include <hardware/adc.h>
#include <hardware/spi.h>

#include <hardware/spi.h>

#include <string>

void setup()
{
    // Initialize the EEPROM with 1024 bytes and load the configuration from it.
    EEPROM.begin(1024);
    ConfigController.loadConfig();

        //configure SPI here

    if (EXT_ADC_KEYS > -1)
    {
        spi_init (spi0, SPI_POLLRATE * 1000);
        //I think the polarity is wrong with this? was breaking it when its enabled but saw it in example, seems to be unnecessary?
        //phob code doesnt have it so probably not necessary
        //spi_set_format (spi0, 24, SPI_CPOL_1, SPI_CPHA_1, SPI_MSB_FIRST);
        gpio_set_function(pin_SPI0SCK, GPIO_FUNC_SPI);
        gpio_set_function(pin_SPI0TX, GPIO_FUNC_SPI);
        gpio_set_function(pin_SPI0RX, GPIO_FUNC_SPI);
        //CS pins
        for (uint8_t i = 1; i <= TOTAL_EXT_ADCS; i++)
        {
            uint8_t cs = KeyHandler.findCSPin(i);
            gpio_init(cs);
            gpio_set_dir(cs, GPIO_OUT);
            gpio_put(cs, 1);
        }
    }


    // Initialize the serial and HID interface.
    Serial.begin(115200);
    Keyboard.begin();
    Keyboard.setAutoReport(false);

    // Set the amount of bits for the ADC to the defined one for a better resolution on the analog readings.
    analogReadResolution(ANALOG_RESOLUTION);

    // Set the pinmode for all pins with digital buttons connected to PULLUP, as that's the standard for working with digital buttons.
    for(int i = 0; i < DIGITAL_KEYS; i++)
        pinMode(DIGITAL_PIN(i), INPUT_PULLUP);



    // Allows to boot into UF2 bootloader mode by pressing the reset button twice.
    rp2040.enableDoubleResetBootloader();


}

void loop()
{
    // Run the keypad handler checks to handle the actual keypad functionality.
    KeyHandler.handle();
}

void serialEvent()
{
    // Handle incoming serial data.
    while(Serial.available() > 0)
    {
        // Read the incoming serial data until a newline into a buffer and terminate it with a null terminator.
        char input[SERIAL_INPUT_BUFFER_SIZE];
        const size_t length = Serial.readBytesUntil('\n', input, SERIAL_INPUT_BUFFER_SIZE);
        input[length] = '\0';

        // Pass the read input to the serial handler to handle it.
        SerialHandler.handleSerialInput(input);
    }
}
