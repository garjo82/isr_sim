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

### io/(Telemetry & Logging)
Handles data persistence and real-time broadcast.
- File IO: GroundTruthLogger and PerceptionLogger generate deterministic CSVs for offline analysis.
- `PerceptionLogger` logs sensor detections
- WebSocket Bridge: Integrated IXWebSocket to provide a live telemetry stream.
- JSON Serialization: Uses nlohmann/json to package world state and sensor detections into a standardized schema for external consumers.

Both can be independently enabled or disabled at runtime.

### render/
Optional ASCII-based renderer and WebGL Visualizer for development and reasoning.
- ASCII Renderer: A local, low-fidelity debug view that runs in the terminal.
- WebGL Visualizer (In Progress): A React/Three.js frontend designed to consume the 10Hz WebSocket stream for high-fidelity 3D situational awareness.

---

## Execution Modes

### The framework is designed to be a dual-purpose tool, toggled via App configuration:  

Headless / Research Mode:  
- Disables all rendering and networking. Runs at maximum CPU frequency to generate large datasets (CSV) for Monte Carlo simulations or model training.

Interactive / Dev Mode:  
- Throttles execution to a deterministic 10Hz heartbeat (matching production autonomy stacks like Aurora). Enables the ASCII renderer and the WebSocket broadcast for live debugging.

---

## Build Instructions

### Requirements
- CMake 3.20+
- C++20-compatible compiler:
	- Windows: Visual Studio 2022 with "Desktop development with C++" workload.
	- Linux: GCC 10+ or Clang 10+
	- macOS: Xcode 12+
- Git

### Build (from repo root)
cmake -B build -S .  
cmake --build build --target isr_sim  

If you run into errors:  
Remove-Item -Recurse -Force build  
cmake -B build -S .  
cmake --build build --target isr_sim  

For Windows users, the easiest way to ensure cl.exe is found is to use the 
Developer PowerShell for VS 2022 instead of standard PowerShell.

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
- [x] C++20 Simulation Core: Deterministic, state-based physics.
- [x] Sensor Modeling: Probabilistic Radar with noise and false positives.
- [x] Multithreaded Telemetry: Live 10Hz WebSocket server integrated into the simulation loop.
- [x] System Observability: Simultaneous CSV logging, ASCII terminal rendering, and JSON broadcasting.
- [ ] 3D Visualization: React + Three.js frontend (Current Active Sprint).

## Tech Stack
- C++20
- CMake
- Visual Studio / MSVC (current dev environment)
- Git

## License
MIT (or TBD)
