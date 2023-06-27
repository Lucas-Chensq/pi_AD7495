#include <stdio.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#define CHANNEL 0      // SPI channel (0 or 1)
#define SPEED   1000000 // SPI bus speed in Hz

int main() {
    if (wiringPiSetup() == -1) {
        printf("Failed to initialize WiringPi library\n");
        return 1;
    }

    if (wiringPiSPISetup(CHANNEL, SPEED) == -1) {
        printf("Failed to initialize SPI communication\n");
        return 1;
    }

    while (1) {
        unsigned char buffer[3];
        buffer[0] = 0x01; // Start bit
        buffer[1] = 0x80; // Single-ended mode, channel 0
        buffer[2] = 0x00; // Placeholder for received data

        wiringPiSPIDataRW(CHANNEL, buffer, 3);

        // Combine the received bytes into a 12-bit ADC value
        unsigned int adcValue = ((buffer[1] & 0x0F) << 8) | buffer[2];

        printf("Analog value: %u\n", adcValue);

        delayMicroseconds(1000); // Delay between readings
    }

    return 0;
}
