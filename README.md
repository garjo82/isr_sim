# ISR Simulation Framework (C++)

This repository contains a modular C++ simulation framework inspired by an earlier ISR
(Intelligence, Surveillance, Reconnaissance) drone capstone project. The goal of this rewrite
is to evolve that prototype into a clean, simulation-first architecture suitable for systems,
autonomy, and sensor experimentation.

The project emphasizes:
- modern C++ (C++20)
- modular design using CMake targets
- deterministic, headless simulation for analysis
- explicit separation of truth, perception, and visualization
- optional rendering and logging for development and reasoning
- reproducible builds and tooling

---

## Project Goals

- Build a lightweight ISR-style simulation harness
- Support multiple entities (e.g., drones, targets)
- Enable deterministic scenario generation and repeatable runs
- Model realistic sensor behavior distinct from ground truth
- Provide clean separation between simulation, sensing, logging, and visualization
- Serve as a portfolio project for systems, autonomy, and mission-focused roles

---

## Architecture

The codebase is organized into modules:
- src/
	- app/ Application orchestration layer
	- sim/ Core deterministic simulation
	- sensors/ Sensor models and detection logic
	- io/ Logging utilities (ground truth and perception)
	- render/ Optional visualization (development/debug only)

### app/
Application entry point and orchestration layer. Owns:
- scenario configuration
- simulation lifecycle
- wiring of optional consumers (logging, rendering)
The main executable is intentionally thin.

### sim/
Core simulation logic:
- deterministic timestep-based simulation loop
- world state and time management
- entity abstractions (Drone, Target, Pose)
- reproducible behavior via seeded randomness
This layer is **fully headless** and has no dependency on IO or rendering.

### sensors/
Sensor models that operate on world state but produce **no direct side effects**.
Current implementation includes a radar-like sensor with:
- range and field-of-view gating
- probabilistic dropouts
- measurement noise
- confidence estimation
- false positives (clutter)
Sensor output is intentionally separated from ground truth.

### io/
Logging utilities used as optional consumers:
- `GroundTruthLogger` — logs full simulation state
- `PerceptionLogger` — logs sensor detections
Both can be independently enabled or disabled at runtime.

### render/
Optional ASCII-based renderer for development and reasoning.
- Consumes world state and sensor detections
- Portable across platforms
- Gracefully degrades when terminal features are unavailable
- Rendering is **strictly optional** and not required for simulation or logging

---

## Build Instructions

### Requirements
- CMake 3.20+
- C++20-compatible compiler (MSVC, Clang, or GCC)
- Git

### Build (from repo root)
```bash
cmake -S . -B build
cmake --build build

---

## Run

### On Windows (Debug):
.\build\Debug\isr_sim.exe

Running the executable can produce:
- state.csv (ground truth)
- detections.csv (sensor perception)
Logging and rendering are controlled via simple runtime toggles in App.

---

## Current Status
- Deterministic headless simulation core
- Scenario-based, reproducible world generation
- Drone kinematic motion with heading-based dynamics
- Radar-style sensor with noise, confidence, and false positives
- Explicit separation between:
	- ground truth
	- perception output
	- visualization
- Optional ASCII rendering for debugging and reasoning
- Clean modular CMake build
At this stage, the framework functions as a headless ISR simulation harness capable of
producing analyzable data.

## Future Work (Optional Extensions)
- The current system is intentionally minimal. Possible extensions include:
- More realistic drone dynamics and behaviors (e.g., sweep/search patterns)
- Additional sensor types or multi-sensor fusion
- Object tracking and data association
- Graphical (2D/3D) visualization layers
- Offline replay and analysis from logged CSV data
- CLI-based configuration for batch experiments
These are considered incremental engineering extensions, not requirements for the core system.

## Tech Stack
- C++20
- CMake
- Visual Studio / MSVC (current dev environment)
- Git

## License
MIT (or TBD)
