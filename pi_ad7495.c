//config CLK DAT CS through IO

#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define CHANNEL 0 // SPI channel (0 or 1)
#define SPEED   1000000 // SPI bus speed in Hz

void delayAD7495() {
    delayMicroseconds(1); // Adjust the delay as needed
}

uint16_t readAnalogValue() {
    uint8_t i;
    uint16_t adValue = 0;

    digitalWrite(10, LOW); // Set CS pin high (assuming GPIO pin 10 for CS)
    digitalWrite(11, HIGH); // Set CLK pin high (assuming GPIO pin 11 for CLK)

    delayAD7495();
    for (i = 0; i < 16; i++) {
        digitalWrite(11, HIGH); // Set CLK pin high (assuming GPIO pin 11 for CLK)

        if (digitalRead(9)) // Read DAT pin (assuming GPIO pin 9 for DAT)
            adValue |= 0x01;

        adValue <<= 1;

        delayAD7495();
        digitalWrite(11, LOW); // Set CLK pin low
        delayAD7495();
    }

    delayAD7495();
    digitalWrite(10, High); // Set CS pin low
    digitalWrite(11, HIGH); // Set CLK pin high
    
    adValue &= 0x0FFF; // Mask the 12 least significant bits

    return adValue;
}

int main() {
    if (wiringPiSetup() == -1) {
        printf("Failed to initialize WiringPi library\n");
        return 1;
    }

    if (wiringPiSPISetup(CHANNEL, SPEED) == -1) {
        printf("Failed to initialize SPI communication\n");
        return 1;
    }

    pinMode(10, OUTPUT); // Set CS pin as output
    pinMode(11, OUTPUT); // Set CLK pin as output
    pinMode(9, INPUT); // Set DAT pin as input

    while (1) {
        uint16_t analogValue = readAnalogValue();

        printf("Analog value: %u\n", analogValue);

        delay(1000); // Delay between readings
    }

    return 0;
}

