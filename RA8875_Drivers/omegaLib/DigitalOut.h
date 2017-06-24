/* 
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __DIGITAL_OUT_
#define __DIGITAL_OUT_

#include "gpio.h"

/** A digital output, used for setting the state of a pin
 *
 * Example:
 * @code
 * // Toggle a LED
 * #include "mbed.h"
 *
 * DigitalOut led(LED1);
 *
 * int main() {
 *     while(1) {
 *         led = !led;
 *         wait(0.2);
 *     }
 * }
 * @endcode
 */
class DigitalOut {
 
public:
    /** Create a DigitalOut connected to the specified pin
     *
     *  @param pin DigitalOut pin to connect to
     */
    DigitalOut(PinName pin) {
        /* Init GPIO system */
	Gpio::initialize();
	
	/* Set the pin direction out */
        Gpio::pinMode(pin, GPD_OUTPUT);

	this->m_pin = pin;
        this->m_state = 0;
	Gpio::digitalWrite(pin, m_state);
    }
 
    /** Create a DigitalOut connected to the specified pin
     *
     *  @param pin DigitalOut pin to connect to
     *  @param value the initial pin value
     */
    DigitalOut(PinName pin, int value) {
	/* Init GPIO system */
	Gpio::initialize;

	/* Set the pin direction out */
	Gpio::pinMode(pin, GPD_OUTPUT);

	/* Set pin output value */
	this->m_pin = pin;
	this->m_state = value;
        Gpio::digitalWrite(pin, m_state);
    }
 
    /** Set the output, specified as 0 or 1 (int)
     *
     *  @param value An integer specifying the pin output value,
     *      0 for logical 0, 1 (or any other non-zero value) for logical 1
     */
    void write(int value) {
        this->m_state = value;
        Gpio::digitalWrite(m_pin, m_state);
    }
 
    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    an integer representing the output setting of the pin,
     *    0 for logical 0, 1 for logical 1
     */
    int read() {
        return Gpio::digitalRead(m_pin);
    }
 
    /** Return the output setting, represented as 0 or 1 (int)
     *
     *  @returns
     *    Non zero value if pin is connected to uc GPIO
     *    0 if gpio object was initialized with NC
     */
    int is_connected() {
        return m_pin != NC_PIN;
    }
 
    /** A shorthand for write()
     */
    DigitalOut& operator= (int value) {
        write(value);
        return *this;
    }
 
    DigitalOut& operator= (DigitalOut& rhs) {
        write(rhs.read());
        return *this;
    }
 
    /** A shorthand for read()
     */
    operator int() {
        return read();
    }
 
protected:
    PinName m_pin;
    uint8_t m_state;
};

#endif /* __ DIGITAL_OUT_ */
