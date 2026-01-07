#define NOMINMAX
#ifdef _WIN32
#include <windows.h>
#endif

#include "AsciiRenderer.h"

#include "../sim/World.h"
#include "../sim/Entity.h"
#include "../sim/Drone.h"
#include "../sim/Target.h"
#include "../sim/Pose.h"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <string>

static double Clamp01(double v) {
    return std::max(0.0, std::min(1.0, v));
}

AsciiRenderer::AsciiRenderer(std::size_t width, std::size_t height, ClearMode mode)
    : width_(width), height_(height), mode_(mode) {
}

std::size_t AsciiRenderer::ToCol(double x, const RenderBounds& b) const {
    if (b.max_x <= b.min_x) return 0;
    const double t = (x - b.min_x) / (b.max_x - b.min_x);
    const double tc = Clamp01(t);
    const std::size_t col = static_cast<std::size_t>(tc * (width_ - 1));
    return std::min(col, width_ - 1);
}

std::size_t AsciiRenderer::ToRow(double y, const RenderBounds& b) const {
    if (b.max_y <= b.min_y) return 0;
    const double t = (y - b.min_y) / (b.max_y - b.min_y);
    const double tc = Clamp01(t);

    // terminal row 0 is top, but we want max_y at the top visually
    const std::size_t row = static_cast<std::size_t>((1.0 - tc) * (height_ - 1));
    return std::min(row, height_ - 1);
}

bool AsciiRenderer::TryClearAnsi() const {
    // ANSI clear screen + cursor home
    std::cout << "\x1b[2J\x1b[H";
    std::cout.flush();
    return true; // If the host doesn't support ANSI, it will print junk, but output still works.
}

bool AsciiRenderer::TryClearWin32() const {
#ifdef _WIN32
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hConsole == INVALID_HANDLE_VALUE) return false;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (!GetConsoleScreenBufferInfo(hConsole, &csbi)) return false;

    const DWORD cellCount =
        static_cast<DWORD>(csbi.dwSize.X) * static_cast<DWORD>(csbi.dwSize.Y);

    DWORD count;
    COORD homeCoords = { 0, 0 };

    if (!FillConsoleOutputCharacter(hConsole, ' ', cellCount, homeCoords, &count)) return false;
    if (!FillConsoleOutputAttribute(hConsole, csbi.wAttributes, cellCount, homeCoords, &count)) return false;
    if (!SetConsoleCursorPosition(hConsole, homeCoords)) return false;

    return true;
#else
    return false;
#endif
}

bool AsciiRenderer::TryClearScreen() const {
    switch (mode_) {
    case ClearMode::None:
        return false;

    case ClearMode::Win32:
        return TryClearWin32();

    case ClearMode::Ansi:
        // We "assume" true, but note: some hosts will print the escape codes.
        // Still okay — output remains readable (just not animated).
        return TryClearAnsi();

    case ClearMode::Auto:
    default:
#ifdef _WIN32
        // On Windows, prefer Win32 clear (most reliable in legacy consoles)
        if (TryClearWin32()) return true;
        // If Win32 clear isn't available, try ANSI (works in Windows Terminal)
        return TryClearAnsi();
#else
        // On non-Windows, ANSI is the usual approach
        return TryClearAnsi();
#endif
    }
}

bool AsciiRenderer::Render(const World& world,
    const std::vector<Detection>& detections,
    const RenderBounds& bounds,
    int step,
    double time_s) {
    // Build a blank grid
    std::vector<std::string> grid(height_, std::string(width_, ' '));

    auto put = [&](double x, double y, char c) {
        const std::size_t col = ToCol(x, bounds);
        const std::size_t row = ToRow(y, bounds);

        // Priority: Drone > detection > target
        char& cell = grid[row][col];
        if (c == 'D') { cell = 'D'; return; }
        if (c == '*') {
            if (cell != 'D') cell = '*';
            return;
        }
        if (c == 'x') {
            if (cell != 'D' && cell != '*') cell = 'x';
            return;
        }
        if (c == 'T') {
            if (cell == ' ') cell = 'T';
            return;
        }
        };

    // Draw truth entities
    for (const auto& sp : world.entities()) {
        const Entity& e = *sp;
        const Pose& p = e.pose();

        if (dynamic_cast<const Drone*>(&e)) put(p.x, p.y, 'D');
        else if (dynamic_cast<const Target*>(&e)) put(p.x, p.y, 'T');
    }

    // Draw detections
    for (const auto& d : detections) {
        put(d.x_world, d.y_world, d.is_false_positive ? 'x' : '*');
    }

    const bool cleared = TryClearScreen();

    std::cout << "ISR Sim (ASCII debug) | step=" << step
        << " | time_s=" << time_s
        << " | truth_entities=" << world.entities().size()
        << " | detections=" << detections.size()
        << "\n";

    std::cout << "Legend: D=drone, T=truth target, *=detection, x=false positive\n";
    std::cout << "Bounds: x[" << bounds.min_x << "," << bounds.max_x
        << "] y[" << bounds.min_y << "," << bounds.max_y << "]\n\n";

    // Print grid with a simple border
    std::cout << "+" << std::string(width_, '-') << "+\n";
    for (const auto& row : grid) {
        std::cout << "|" << row << "|\n";
    }
    std::cout << "+" << std::string(width_, '-') << "+\n";

    std::cout.flush();
    return cleared;
}
