/* GPIO Lib not by me, source code
 * at : https://github.com/Patryk27/Omega2-GPIO
 */
#ifndef GPIO_H
#define GPIO_H

#include <cstdint>
#include <stdexcept>

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

enum GpioPinDirection {
    GPD_INPUT = 0,
    GPD_OUTPUT = 1,
};

enum GpioPinValue {
    GPV_LOW = 0,
    GPV_HIGH = 1,
};

enum GpioBitOrder {
    GBO_LSB_FIRST,
    GBO_MSB_FIRST,
};

/* Pins definition */
#define PinName uint8_t

#define NC_PIN -1
#define P0_PIN  0
#define P1_PIN  1
#define P2_PIN  2
#define P3_PIN  3
#define P4_PIN  4
#define P5_PIN  5
#define P6_PIN  6
#define P7_PIN  7
#define P8_PIN  8
#define P9_PIN  9
#define P11_PIN 11
#define P12_PIN 12
#define P13_PIN 13
#define P15_PIN 15
#define P16_PIN 16
#define P17_PIN 17
#define P18_PIN 18
#define P19_PIN 19
#define P45_PIN 45
#define P46_PIN 46

#define I2S_CLK_PIN P3_PIN
#define I2S_WS_PIN  P2_PIN
#define I2S_SDO_PIN P1_PIN
#define I2S_SDI_PIN P0_PIN

#define SERIAL_RX0_PIN P13_PIN
#define SERIAL_TX0_PIN P12_PIN
#define SERIAL_RX1_PIN P46_PIN
#define SERIAL_TX1_PIN P45_PIN

#define SPI_MISO_PIN P9_PIN
#define SPI_MOSI_PIN P8_PIN
#define SPI_CLK_PIN  P7_PIN
#define SPI_CS_PIN   P6_PIN

#define FW_RST_PIN P38_PIN

#define I2C_SCL_PIN P4_PIN
#define I2C_SDA_PIN P5_PIN

class Gpio {

private:
    static const int REG_BLOCK_ADDR = 0x10000000;

    static const int REGISTER_CTRL0_OFFSET = 384;
    static const int REGISTER_CTRL1_OFFSET = 385;
    static const int REGISTER_CTRL2_OFFSET = 386;

    static const int REGISTER_DATA0_OFFSET = 392;
    static const int REGISTER_DATA1_OFFSET = 393;
    static const int REGISTER_DATA2_OFFSET = 394;

    static const int REGISTER_DSET0_OFFSET = 396;
    static const int REGISTER_DSET1_OFFSET = 397;
    static const int REGISTER_DSET2_OFFSET = 398;

    static const int REGISTER_DCLR0_OFFSET = 400;
    static const int REGISTER_DCLR1_OFFSET = 401;
    static const int REGISTER_DCLR2_OFFSET = 402;

    static bool initialized;

protected:
    static uint16_t controlOffset;
    static uint16_t dataOffset;
    static uint16_t dataSetOffset;
    static uint16_t dataClearOffset;

    static uint32_t *baseAddress;

    static void prepareOffsets(int pin);

    static void writeToRegister(uint16_t registerOffset, uint32_t value);

    static uint32_t readFromRegister(uint64_t registerOffset);

public:
    /**
     * Initializes the GPIO subsystem.
     */
    static void initialize();

    /**
     * Changes pin direction; available directions: GPV_INPUT, GPV_OUTPUT.
     * `direction` == 0 is considered to be GPV_INPUT, anything else is GPV_OUTPUT.
     */
    static void pinMode(uint8_t pin, uint8_t direction);

    /**
     * Changes pin value; available values: GPV_LOW, GPV_HIGH.
     * `value` == 0 is considered to be GPV_LOW, anything else is GPV_HIGH.
     */
    static void digitalWrite(uint8_t pin, uint8_t value);

    /**
     * Reads and returns pin value.
     * This function always returns 0 (meaning GPV_LOW) or non-zero (meaning GPV_HIGH).
     */
    static uint8_t digitalRead(uint8_t pin);

    /**
     * Shifts out byte using given data and clock pin.
     * See Arduino documentation for more specific description.
     */
    static void shiftOut(uint8_t dataPin, uint8_t clockPin, GpioBitOrder bitOrder, uint8_t value);

};

#endif //GPIO_H
