# Web Simulator

This directory contains the HTML shell for the browser simulator.

Build with Emscripten:

```bash
emcmake cmake -B application/simulator/web/build application/simulator
cmake --build application/simulator/web/build
```

The generated files are placed in:

```text
application/simulator/web/build/dist/
├── game_sim.html
├── game_sim.js
└── game_sim.wasm
```

Run locally:

```bash
python3 -m http.server 8000 -d application/simulator/web/build/dist
```

Then open:

```text
http://localhost:8000/
```
