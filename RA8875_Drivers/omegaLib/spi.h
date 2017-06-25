/* 
 * SPI class for Omega2
 *
 * Allow compatibility with Mbed code
 * Not all the functionnalities have been ported,
 * just the ones used in the project.
 * 
 * Author : Alexy Torres Aurora Dugo
 */

#ifndef __SPI_H_
#define __SPI_H_

#include "gpio.h"
#include "onion-spi.h"

#define SPI_MODE_DEFAULT 0

class SPI
{
	public:
		SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel = NC_PIN);
		virtual ~SPI(void);

		void format(int bits, int mode = SPI_MODE_DEFAULT);
		void frequency(int hz = 1000000);

		virtual int write(int value);

	private:
		void init(void);

		PinName mosi;
		PinName miso;
		PinName sclk;
		PinName ssel;

		int format_bits;
		int format_mode;

		int frequency_hz;
		int initSeq;

		struct spiParams params;
};

#endif /* __SPI_H_ */
