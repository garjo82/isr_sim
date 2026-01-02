# ISR Simulation Framework (C++)

This repository contains a modular C++ simulation framework inspired by an earlier ISR
(Intelligence, Surveillance, Reconnaissance) drone capstone project. The goal of this rewrite
is to evolve that prototype into a clean, simulation-first architecture suitable for systems,
autonomy, and sensor experimentation.

The project emphasizes:
- modern C++ (C++20)
- modular design using CMake targets
- deterministic, headless simulation for analysis
- optional visualization for development and reasoning
- reproducible builds and tooling

---

## Project Goals

- Build a lightweight ISR-style simulation harness
- Support multiple entities (e.g., drones, targets)
- EEnable deterministic scenario generation and repeatable runs
- Provide clear separation between simulation, logging, and visualization
- Serve as a portfolio project for systems, autonomy, and mission-focused roles

---

## Architecture

The codebase is organized into modules:
- src/
- app/
Application orchestration layer. Owns scenario setup, simulation execution, and logging.
The main executable is intentionally thin.
- sim/
	Core simulation logic:
	- deterministic simulation loop
	- world state and time management
	- entity abstractions (Drone, Target, Pose)
- io/
Logging and output utilities.
Currently includes CSV-based state and event logging for headless analysis.
- sensors/
Sensor interfaces and detection logic (prototype proximity detection implemented via logging).
- render/
Optional rendering layer (planned).
Rendering will be decoupled from simulation and used strictly for development and debugging.

Each module is built as its own CMake library and linked into a single executable.

---

## Build Instructions

### Requirements
- CMake 3.20+
- C++20-compatible compiler (MSVC, Clang, or GCC)
- Git

### Build (from repo root)
- cmake -S . -B build
- cmake --build build

---

## Run

### On Windows (Debug):
.\build\Debug\isr_sim.exe

Running the executable produces CSV output files (state.csv, events.csv) containing
deterministic simulation results suitable for offline analysis or replay.

---

## Current Status
- Deterministic simulation core implemented
- World and entity update loop complete
- Scenario-based random target generation (seeded and reproducible)
- Drone kinematic motion with heading-based movement
- Headless CSV logging for:
-- per-step world state
-- proximity detection events
- Clean, multi-target CMake build with explicit dependency management

At this stage, the framework functions as a headless ISR simulation harness capable of
producing analyzable data.

## Next steps:
- Line-by-line internal code review and cleanup
- Add optional 2D rendering layer for development/debugging:
-- drone visualization with heading
-- target visualization
-- world bounds
-- optional detection radius
- Implement higher-level drone search behaviors (e.g., sweep patterns)
- Add replay/visualization from logged CSV data
- Introduce lightweight CLI configuration for batch experiments
Rendering will remain optional and strictly decoupled from core simulation logic.

## Tech Stack
- C++20
- CMake
- Visual Studio / MSVC (current dev environment)
- Git

## License
MIT (or TBD)
