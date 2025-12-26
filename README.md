# ISR Simulation Framework (C++)

This repository contains a modular C++ simulation framework inspired by an earlier ISR
(Intelligence, Surveillance, Reconnaissance) drone capstone project. The goal of this rewrite
is to evolve that prototype into a clean, simulation-first architecture suitable for systems,
autonomy, and sensor experimentation.

The project emphasizes:
- modern C++ (C++20)
- modular design using CMake targets
- separation of simulation logic from rendering
- reproducible builds and tooling

---

## Project Goals

- Build a lightweight ISR-style simulation harness
- Support multiple entities (e.g., drones, targets)
- Enable pluggable sensors and detection logic
- Keep rendering optional and decoupled from core simulation
- Serve as a portfolio project for systems and autonomy roles

---

## Architecture

The codebase is organized into modules:
src/
app/ - Application entry point
sim/ - Simulation core (world, stepping, entities)
render/ - Rendering layer (OpenGL planned)
sensors/ - Sensor interfaces and implementations
io/ - Logging and I/O utilities
util/ - Shared helpers

Each module is built as its own CMake library and linked into a single executable.

---

## Build Instructions

### Requirements
- CMake 3.20+
- C++20-compatible compiler (MSVC, Clang, or GCC)
- Git

### Build (from repo root)
cmake -S . -B build
cmake --build build

---

## Run

### On Windows (Debug):
.\build\Debug\isr_sim.exe

---

## Current Status
- Early scaffolding phase:
- CMake multi-target build in place
- Simulation loop stubbed
- Module structure established

## Next steps:
- Add Entity and World abstractions
- Fixed-timestep simulation loop
- Sensor interfaces
- Optional OpenGL renderer

## Tech Stack
- C++20
- CMake
- Visual Studio / MSVC (current dev environment)
- Git

## License
MIT (or TBD)
