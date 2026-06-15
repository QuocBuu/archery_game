#ifndef HARDWARE_SERIAL_H
#define HARDWARE_SERIAL_H

#include "Print.h"

class HardwareSerial : public Print {
public:
	void begin() {}
	int available(void) { return 0; }
	int peek(void) { return -1; }
	int read(void) { return -1; }
	int availableForWrite(void) { return 0; }
	void flush(void) {}
	size_t write(uint8_t value) {
		(void)value;
		return 1;
	}
	using Print::write;
	operator bool() { return true; }
};

extern HardwareSerial Serial2;

#endif
