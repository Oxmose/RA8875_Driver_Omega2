/* 
 * SPI class for Omega2
 *
 * Allow compatibility with Mbed code
 * Not all the functionnalities have been ported,
 * just the ones used in the project.
 * 
 * Author : Alexy Torres Aurora Dugo
 */

#include "spi.h"
#include <iostream>

#define PIN_INIT    0x01
#define FORMAT_INIT 0x02
#define FREQ_INIT   0x04

#define COMPLETE_INIT 0x07

SPI::SPI(PinName mosi, PinName miso, PinName sclk, PinName ssel /* = NC_PIN */)
{
	/* Init members */
	this->mosi = mosi;
	this->miso = miso;
	this->sclk = sclk;
	this->ssel = ssel;

	this->initSeq = PIN_INIT;
}
SPI::~SPI(void)
{
}

void SPI::format(int bits, int mode /* = SPI_MODE_DEFAULT */)
{
	if(bits == 8)
		bits = 0;

	this->format_bits = bits;

	switch(mode)
	{
		case 0:
			this->format_mode = SPI_MODE_0;
			break;
		case 1:
			this->format_mode = SPI_MODE_1;
			break;
		case 2:
			this->format_mode = SPI_MODE_2;
			break;
		case 3:
			this->format_mode = SPI_MODE_3;
			break;
		default:
			this->format_mode = SPI_MODE_0;			
	}

	this->initSeq |= FORMAT_INIT;

	/* If init sequence is finished */
	if(initSeq == COMPLETE_INIT)
	{
		init();
	}
}
void SPI::frequency(int hz /* = 1000000 */)
{
	this->frequency_hz = hz;

	this->initSeq |= FREQ_INIT;

	/* If init sequence is finished */
	if(initSeq == COMPLETE_INIT)
	{
		init();
	}
}

void SPI::init(void)
{
	/* Init SPI parameters */
	spiParamInit(&params);

	params.speedInHz = frequency_hz;
	params.bitsPerWord = format_bits;
	params.mode = format_mode;
	
	params.sckGpio  = sclk;
	params.mosiGpio = mosi;
	params.misoGpio = miso;
	if(ssel != NC_PIN)
		params.csGpio   = ssel;
	else
		params.modeBits = SPI_NO_CS;

	std::cout << "PRAM MODE : " << params.modeBits << std::endl;

	params.deviceId = 32766;
	params.busNum = 1;

	/* Check mapping */
	if(spiCheckDevice(params.busNum, params.deviceId, ONION_SEVERITY_DEBUG_EXTRA) != EXIT_SUCCESS)
	{
		std::cout << "[SPI INIT] Error: Spi device not mapped on bus. Canceling Init." << std::endl;
		return;
	}	

	/* Register the device */
	if(spiRegisterDevice(&params) != EXIT_SUCCESS)
	{
		std::cout << "[SPI INIT] Error: Spi device could not be registered. Canceling Init." << std::endl;
		return;
	}

	/* Extra setup */
	if(spiSetupDevice(&params) != EXIT_SUCCESS)
	{
		std::cout << "[SPI INIT] Error : Spi device setup failed. Canceling Init." << std::endl;
		return;
	}

	std::cout << "SPI INIT OK" << std::endl;
}

int SPI::write(int value)
{
	if(initSeq != COMPLETE_INIT)
	{
		std::cout << "[SPI WRITE] Error: SPI is not initialized." << std::endl;
		return -1;
	}
	/* TODO: Check addr value */
	uint8_t txBuffer = value;
	uint8_t rxBuffer;
	int status;
	if((status = spiTransfer(&params, &txBuffer, &rxBuffer, 1)) != EXIT_SUCCESS)
	{
		std::cout << "[SPI WRITE] Error : SPI write failed, code " << status << "." << std::endl;
		return -1;
	}	 

	return rxBuffer;
}
