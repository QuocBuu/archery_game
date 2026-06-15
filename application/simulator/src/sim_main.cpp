#include "sim_internal.h"

#include <SDL2/SDL.h>
#include <stdint.h>
#include <stdio.h>

#if defined(__EMSCRIPTEN__)
#include <emscripten.h>
#endif

extern "C" int main_app(void);

typedef struct {
	uint32_t last_tick;
} sim_loop_t;

static sim_loop_t sim_loop;

static void sim_run_frame(void* arg) {
	sim_loop_t* loop = (sim_loop_t*)arg;
	uint32_t now = SDL_GetTicks();
	uint32_t elapsed = now - loop->last_tick;
	loop->last_tick = now;
	if (elapsed == 0) {
		elapsed = 1;
	}

	sim_input_poll();
	sim_platform_step(elapsed);
	sim_oled_render();

	if (!sim_input_get_running()) {
		sim_oled_cleanup();
		SDL_Quit();
#if defined(__EMSCRIPTEN__)
		emscripten_cancel_main_loop();
#endif
	}
}

int main(int argc, char** argv) {
	(void)argc;
	(void)argv;

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		fprintf(stderr, "SDL_Init failed: %s\n", SDL_GetError());
		return 1;
	}

	sim_platform_init();
	sim_oled_init();
	sim_input_init();

	main_app();

	sim_loop.last_tick = SDL_GetTicks();
#if defined(__EMSCRIPTEN__)
	emscripten_set_main_loop_arg(sim_run_frame, &sim_loop, 0, 1);
#else
	while (sim_input_get_running()) {
		sim_run_frame(&sim_loop);
		SDL_Delay(16);
	}
#endif
	return 0;
}
