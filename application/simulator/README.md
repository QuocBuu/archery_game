# Hollow Game SDL2 Simulator

Build from the repository root:

```bash
make -C application/simulator
env DISPLAY=:0 SDL_VIDEODRIVER=x11 ./application/simulator/build_make/game_sim
```

Or run directly:

```bash
make -C application/simulator run
```

The simulator creates a single 512x280 SDL window. The top 512x256 area is the
SSD1306-style 128x64 framebuffer scaled by 4, and the bottom line shows the
keyboard controls while the simulation is running.

Keyboard mapping:

- Up arrow or `J`: firmware UP button
- Down arrow or `K`: firmware DOWN button
- Space or Enter: firmware MODE button
- Escape or window close: quit
