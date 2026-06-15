#include "sim_internal.h"

#include <SDL2/SDL.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __EMSCRIPTEN__
#include <emscripten/emscripten.h>
#endif

enum {
	SIM_BUTTON_DOWN_INDEX = 0,
	SIM_BUTTON_UP_INDEX = 1,
	SIM_BUTTON_MODE_INDEX = 2,
	SIM_BUTTON_COUNT = 3
};

static bool running = true;
static bool pressed[SIM_BUTTON_COUNT];

void sim_input_init(void) {
	running = true;
	for (uint8_t i = 0; i < SIM_BUTTON_COUNT; ++i) {
		pressed[i] = false;
	}
}

static void set_key(SDL_Scancode scancode, bool is_pressed) {
	switch (scancode) {
	case SDL_SCANCODE_DOWN:
	case SDL_SCANCODE_K:
		pressed[SIM_BUTTON_DOWN_INDEX] = is_pressed;
		break;
	case SDL_SCANCODE_UP:
	case SDL_SCANCODE_J:
		pressed[SIM_BUTTON_UP_INDEX] = is_pressed;
		break;
	case SDL_SCANCODE_RETURN:
	case SDL_SCANCODE_SPACE:
		pressed[SIM_BUTTON_MODE_INDEX] = is_pressed;
		break;
	default:
		break;
	}
}

void sim_input_poll(void) {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_QUIT:
			running = false;
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
				running = false;
			}
			else if (!event.key.repeat) {
				set_key(event.key.keysym.scancode, true);
			}
			break;
		case SDL_KEYUP:
			set_key(event.key.keysym.scancode, false);
			break;
		default:
			break;
		}
	}
}

bool sim_input_get_running(void) {
	return running;
}

void sim_input_request_quit(void) {
	running = false;
}

int sim_input_read_button(uint8_t button_id) {
	switch (button_id) {
	case 1:
		return pressed[SIM_BUTTON_MODE_INDEX] ? 0 : 1;
	case 2:
		return pressed[SIM_BUTTON_UP_INDEX] ? 0 : 1;
	case 3:
		return pressed[SIM_BUTTON_DOWN_INDEX] ? 0 : 1;
	default:
		return 1;
	}
}

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
void sim_web_button_set(int button_id, int is_pressed) {
	bool value = is_pressed != 0;

	switch (button_id) {
	case 1:
		pressed[SIM_BUTTON_MODE_INDEX] = value;
		break;
	case 2:
		pressed[SIM_BUTTON_UP_INDEX] = value;
		break;
	case 3:
		pressed[SIM_BUTTON_DOWN_INDEX] = value;
		break;
	default:
		break;
	}
}
#endif
