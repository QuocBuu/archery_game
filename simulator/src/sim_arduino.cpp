#include "Arduino.h"
#include "HardwareSerial.h"
#include "SPI.h"
#include "sim_internal.h"

#include <SDL2/SDL.h>

SPIClass SPI;
HardwareSerial Serial2;

static uint8_t gpio_state[256];

static bool i2c_started = false;
static uint8_t i2c_shift = 0;
static uint8_t i2c_bits = 0;

static void i2c_watch_edge(uint8_t pin, uint8_t old_value, uint8_t new_value) {
	if (pin == SIM_PIN_OLED_DATA && gpio_state[SIM_PIN_OLED_CLK] == HIGH) {
		if (old_value == HIGH && new_value == LOW) {
			i2c_started = true;
			i2c_shift = 0;
			i2c_bits = 0;
			sim_oled_i2c_start();
		}
		else if (old_value == LOW && new_value == HIGH) {
			i2c_started = false;
			i2c_shift = 0;
			i2c_bits = 0;
			sim_oled_i2c_stop();
		}
	}

	if (pin == SIM_PIN_OLED_CLK && old_value == LOW && new_value == HIGH && i2c_started) {
		if (i2c_bits < 8) {
			i2c_shift = (uint8_t)((i2c_shift << 1) | (gpio_state[SIM_PIN_OLED_DATA] ? 1u : 0u));
			i2c_bits++;
			if (i2c_bits == 8) {
				sim_oled_write_i2c_byte(i2c_shift);
			}
		}
		else {
			i2c_bits = 0;
			i2c_shift = 0;
		}
	}
}

void sim_gpio_set_pin(uint8_t pin, uint8_t value) {
	uint8_t old_value = gpio_state[pin];
	value = value ? HIGH : LOW;
	if (old_value == value) {
		return;
	}
	gpio_state[pin] = value;
	i2c_watch_edge(pin, old_value, value);
}

uint8_t sim_gpio_get_pin(uint8_t pin) {
	return gpio_state[pin];
}

void pinMode(uint8_t pin, uint8_t mode) {
	(void)mode;
	if (gpio_state[pin] != LOW && gpio_state[pin] != HIGH) {
		gpio_state[pin] = HIGH;
	}
}

void digitalWrite(uint8_t pin, uint8_t value) {
	sim_gpio_set_pin(pin, value);
}

int digitalRead(uint8_t pin) {
	return sim_gpio_get_pin(pin);
}

void delay(uint32_t ms) {
	SDL_Delay(ms);
}

void delayMicroseconds(uint32_t us) {
	SDL_Delay((us + 999u) / 1000u);
}

uint32_t millis(void) {
	return SDL_GetTicks();
}

uint32_t micros(void) {
	return SDL_GetTicks() * 1000u;
}

void yield(void) {}

uint8_t shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) {
	(void)dataPin;
	(void)clockPin;
	(void)bitOrder;
	return 0;
}

void shiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val) {
	for (uint8_t i = 0; i < 8; ++i) {
		uint8_t bit_index = bitOrder == LSBFIRST ? i : (uint8_t)(7u - i);
		digitalWrite(dataPin, (val & (1u << bit_index)) ? HIGH : LOW);
		digitalWrite(clockPin, HIGH);
		digitalWrite(clockPin, LOW);
	}
}

uint16_t makeWord(uint16_t w) { return w; }
uint16_t makeWord(byte h, byte l) { return (uint16_t)((h << 8) | l); }

void randomSeed(unsigned long seed) {
	if (seed) {
		srand((unsigned int)seed);
	}
}

long random(long howbig) {
	return howbig == 0 ? 0 : rand() % howbig;
}

long random(long howsmall, long howbig) {
	if (howsmall >= howbig) {
		return howsmall;
	}
	return random(howbig - howsmall) + howsmall;
}

long map(long x, long in_min, long in_max, long out_min, long out_max) {
	return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
