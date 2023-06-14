import spidev
import RPi.GPIO as GPIO

# Define ADC channel configuration
ADC_CHANNEL = 0  # Analog input channel number (0 to 7)
SPI_SPEED = 1000000  # SPI bus speed in Hz

# Initialize SPI
spi = spidev.SpiDev()
spi.open(0, 0)  # Open SPI bus 0, device 0
spi.max_speed_hz = SPI_SPEED

# Initialize GPIO
GPIO.setmode(GPIO.BCM)
CS_PIN = 10  # GPIO pin for ADC chip select (CS)
GPIO.setup(CS_PIN, GPIO.OUT)

# Function to read analog value from ADC
def read_analog(channel):
    # Configure ADC channel and mode
    adc_command = 0x80 | ((channel & 0x7) << 4)
    spi.xfer2([adc_command, 0x00])  # Send command and dummy byte
    raw_data = spi.xfer2([0x00, 0x00])  # Receive 2 bytes of data (discard first byte)
    value = ((raw_data[0] & 0x0F) << 8) | raw_data[1]  # Combine received bytes

    return value

try:
    while True:
        # Read analog value from ADC channel
        analog_value = read_analog(ADC_CHANNEL)

        # Print the result
        print(f"Analog value: {analog_value}")

except KeyboardInterrupt:
    # Clean up GPIO and close SPI
    GPIO.cleanup()
    spi.close()