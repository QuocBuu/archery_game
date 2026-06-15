#include "sim_internal.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

static SDL_Window* window;
static SDL_Renderer* renderer;
static SDL_Texture* texture;

static uint8_t framebuffer[SIM_OLED_WIDTH * SIM_OLED_HEIGHT / 8];
static uint8_t pixels[SIM_OLED_WIDTH * SIM_OLED_HEIGHT * 3];

enum {
	SIM_WINDOW_WIDTH = SIM_OLED_WIDTH * SIM_OLED_SCALE,
	SIM_WINDOW_HEIGHT = SIM_OLED_HEIGHT * SIM_OLED_SCALE
};

static uint8_t col;
static uint8_t page;
static uint8_t col_start;
static uint8_t col_end = SIM_OLED_WIDTH - 1;
static uint8_t page_start;
static uint8_t page_end = 7;
static uint8_t addr_mode = 2;
static bool display_on;
static bool inverted;

static uint8_t command_state;
static uint8_t command_buffer[2];
static uint8_t command_index;

static uint8_t i2c_index;
static uint8_t i2c_control;
static bool i2c_is_data;

static void oled_command(uint8_t value);

void sim_oled_init(void) {
	window = SDL_CreateWindow("AK Base - Oled (128*64) simulator",
							  SDL_WINDOWPOS_CENTERED,
							  SDL_WINDOWPOS_CENTERED,
							  SIM_WINDOW_WIDTH,
							  SIM_WINDOW_HEIGHT,
							  0);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24, SDL_TEXTUREACCESS_STREAMING,
								SIM_OLED_WIDTH, SIM_OLED_HEIGHT);
	memset(framebuffer, 0, sizeof(framebuffer));
}

void sim_oled_i2c_start(void) {
	i2c_index = 0;
}

void sim_oled_i2c_stop(void) {
	i2c_index = 0;
}

static void oled_data(uint8_t value) {
	if (page > 7 || col > SIM_OLED_WIDTH - 1) {
		return;
	}

	framebuffer[(page * SIM_OLED_WIDTH) + col] = value;

	if (addr_mode == 1) {
		if (page >= page_end) {
			page = page_start;
			col = (col >= col_end) ? col_start : (uint8_t)(col + 1u);
		}
		else {
			page++;
		}
	}
	else {
		if (col >= col_end) {
			col = col_start;
			if (addr_mode == 0) {
				page = (page >= page_end) ? page_start : (uint8_t)(page + 1u);
			}
		}
		else {
			col++;
		}
	}
}

void sim_oled_write_i2c_byte(uint8_t byte) {
	if (i2c_index == 0) {
		i2c_index++;
		return;
	}

	if (i2c_index == 1) {
		i2c_control = byte;
		i2c_is_data = (byte & 0x40u) != 0;
		i2c_index++;
		return;
	}

	if (i2c_is_data) {
		oled_data(byte);
	}
	else {
		oled_command(byte);
	}

	if ((i2c_control & 0x80u) != 0) {
		i2c_index = 1;
	}
}

static void finish_command_with_param(uint8_t value) {
	command_buffer[command_index++] = value;

	switch (command_state) {
	case 0x20:
		addr_mode = command_buffer[0] & 0x03u;
		break;
	case 0x21:
		if (command_index < 2) return;
		col_start = command_buffer[0] < SIM_OLED_WIDTH ? command_buffer[0] : 0;
		col_end = command_buffer[1] < SIM_OLED_WIDTH ? command_buffer[1] : (SIM_OLED_WIDTH - 1);
		col = col_start;
		break;
	case 0x22:
		if (command_index < 2) return;
		page_start = command_buffer[0] & 0x07u;
		page_end = command_buffer[1] & 0x07u;
		page = page_start;
		break;
	default:
		break;
	}

	command_state = 0;
	command_index = 0;
}

static bool command_needs_param(uint8_t value) {
	switch (value) {
	case 0x20:
	case 0x81:
	case 0xA8:
	case 0xD3:
	case 0xD5:
	case 0xD9:
	case 0xDA:
	case 0xDB:
	case 0x8D:
		return true;
	case 0x21:
	case 0x22:
		return true;
	default:
		return false;
	}
}

static void oled_command(uint8_t value) {
	if (command_state) {
		finish_command_with_param(value);
		return;
	}

	if (value == 0xAE) {
		display_on = false;
	}
	else if (value == 0xAF) {
		display_on = true;
	}
	else if (value >= 0xB0 && value <= 0xB7) {
		page = value & 0x07u;
		col = col_start;
	}
	else if (value <= 0x0F) {
		col = (uint8_t)((col & 0xF0u) | (value & 0x0Fu));
	}
	else if (value >= 0x10 && value <= 0x1F) {
		col = (uint8_t)(((value & 0x0Fu) << 4) | (col & 0x0Fu));
		if (col >= SIM_OLED_WIDTH) {
			col = SIM_OLED_WIDTH - 1;
		}
	}
	else if (value >= 0x40 && value <= 0x7F) {
	}
	else if (value == 0xA6) {
		inverted = false;
	}
	else if (value == 0xA7) {
		inverted = true;
	}
	else if (command_needs_param(value)) {
		command_state = value;
		command_index = 0;
	}
}

void sim_oled_render(void) {
	for (uint8_t page_index = 0; page_index < 8; ++page_index) {
		for (uint8_t x = 0; x < SIM_OLED_WIDTH; ++x) {
			uint8_t byte = framebuffer[(page_index * SIM_OLED_WIDTH) + x];
			for (uint8_t bit = 0; bit < 8; ++bit) {
				uint8_t y = (uint8_t)((page_index * 8u) + bit);
				bool on = display_on && (((byte >> bit) & 1u) != 0);
				if (inverted && display_on) {
					on = !on;
				}
				uint8_t color = on ? 0xFF : 0x00;
				uint32_t offset = ((uint32_t)y * SIM_OLED_WIDTH + x) * 3u;
				pixels[offset + 0] = color;
				pixels[offset + 1] = color;
				pixels[offset + 2] = color;
			}
		}
	}

	SDL_UpdateTexture(texture, NULL, pixels, SIM_OLED_WIDTH * 3);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	SDL_Rect oled_rect = {0, 0, SIM_WINDOW_WIDTH, SIM_OLED_HEIGHT * SIM_OLED_SCALE};
	SDL_RenderCopy(renderer, texture, NULL, &oled_rect);
	SDL_RenderPresent(renderer);
}


void sim_oled_cleanup(void) {
	if (texture) {
		SDL_DestroyTexture(texture);
		texture = NULL;
	}
	if (renderer) {
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	if (window) {
		SDL_DestroyWindow(window);
		window = NULL;
	}
}
