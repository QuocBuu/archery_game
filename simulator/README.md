# Simulator

Runs the archery game on Linux (SDL2) and in the browser (Emscripten/WebAssembly).
Shares all game sources with the firmware — no separate source list to maintain.

## Commands

```bash
make init        # check required dependencies
make             # build native simulator
make run         # build and launch (requires X11)
make web         # build WebAssembly version
make web-serve   # build and serve at http://127.0.0.1:8000/
make web-deploy  # build and copy to ../docs/simulator/ for GitHub Pages
make clean       # clean native build
make clean-web   # clean web build
make clean-all   # clean everything
```

**Dependencies** — native: `gcc`, `g++`, `pkg-config`, `libsdl2-dev`.

```bash
sudo apt install build-essential pkg-config libsdl2-dev
```

Web: [emsdk](https://emscripten.org/docs/getting_started/downloads.html) — install emsdk, then activate before building:

```bash
source ~/emsdk/emsdk_env.sh
make web
```

**Keyboard controls:** `Up`/`J` = UP, `Down`/`K` = DOWN, `Space`/`Enter` = MODE, `Esc` = quit.

## Source structure

```
simulator/
├── Makefile          # build targets (native + web)
├── simulator.mk      # shared source/flag config, included by Makefile
├── src/              # simulator stubs (replace STM32 hardware)
│   ├── sim_main.cpp      # entry point, SDL event loop
│   ├── sim_platform.cpp  # HAL stubs: EEPROM, flash, GPIO, clocks, …
│   ├── sim_oled.c        # OLED framebuffer → SDL texture (4× scaled)
│   ├── sim_input.c       # SDL keyboard → button signals
│   ├── sim_arduino.cpp   # Arduino API shims (SPI, Wire, …)
│   └── sim_shell.cpp     # shell stub (no UART in sim)
├── inc/              # stub headers that shadow STM32 platform headers
│   ├── Arduino.h         # stub (no sys_cfg.h dependency)
│   ├── io_cfg.h          # pin/peripheral config stub
│   ├── platform.h        # platform identification
│   └── …                 # other STM32 header stubs
└── web/
    └── shell.html    # HTML wrapper for the Emscripten canvas output
```

Game sources live in `../application/sources/` and are pulled in automatically
via `simulator.mk` → `sources/*/Makefile.mk`. Adding a source file there makes
it appear in both the firmware and simulator builds with no extra steps.

## Native vs Web

Both builds compile **identical source code**. The only differences are in toolchain and runtime:

| | Native | Web |
|---|---|---|
| Compiler | `gcc` / `g++` | `emcc` / `em++` (Emscripten) |
| SDL2 | system library | `-sUSE_SDL=2` (Emscripten port) |
| Output | `build_simulator/game_sim` binary | `.html` + `.js` + `.wasm` bundle |
| Game loop | blocking `for(;;)` in `task_run()` | `ASYNCIFY` suspends/resumes the Wasm call stack to avoid blocking the browser main thread |
| HTML shell | — | `web/shell.html` provides the page layout and canvas |

`ASYNCIFY` is the key flag: the game loop calls `SDL_Delay()` in a blocking style.
Emscripten instruments the entire call stack so it can yield to the browser event
loop between frames while still running the same blocking game logic.
