#ifndef __SPI_H__
#define __SPI_H__

#include <stdint.h>
#include "Arduino.h"

#define SPI_CLOCK_DIV2   2
#define SPI_CLOCK_DIV4   4
#define SPI_CLOCK_DIV8   8
#define SPI_CLOCK_DIV16  16
#define SPI_CLOCK_DIV32  32
#define SPI_CLOCK_DIV64  64
#define SPI_CLOCK_DIV128 128
#define SPI_CLOCK_DIV256 256

#define SPI_MODE0 0x00
#define SPI_MODE1 0x80
#define SPI_MODE2 0x40
#define SPI_MODE3 0xC0

class SPISettings {
public:
	SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)
		: setting_clock(clock), setting_bitorder(bitOrder), setting_datamode(dataMode) {}
	SPISettings() : setting_clock(4000000), setting_bitorder(MSBFIRST), setting_datamode(SPI_MODE0) {}
private:
	uint32_t setting_clock;
	uint8_t setting_bitorder;
	uint8_t setting_datamode;
	friend class SPIClass;
};

class SPIClass {
public:
	SPIClass(void) {}
	SPIClass(uint8_t module) { (void)module; }
	void begin() {}
	void end() {}
	void beginTransaction(SPISettings settings) { (void)settings; }
	void endTransaction(void) {}
	void setBitOrder(uint8_t bitOrder) { (void)bitOrder; }
	void setBitOrder(uint8_t ssPin, uint8_t bitOrder) { (void)ssPin; (void)bitOrder; }
	void setDataMode(uint8_t mode) { (void)mode; }
	void setClockDivider(uint8_t divider) { (void)divider; }
	uint8_t transfer(uint8_t data) { return data; }
	uint16_t transfer16(uint16_t data) { return data; }
	void transfer(void *buf, uint32_t count) { (void)buf; (void)count; }
	void setModule(uint8_t module) { (void)module; }
};

extern SPIClass SPI;

#endif
