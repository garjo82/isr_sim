#pragma once

#include <cstddef>
#include <vector>

#include "../sensors/Detection.h"

class World;

struct RenderBounds {
    double min_x = 0.0;
    double max_x = 0.0;
    double min_y = 0.0;
    double max_y = 0.0;
};

enum class ClearMode {
    Auto,
    None,
    Ansi,
    Win32
};

class AsciiRenderer {
public:
    AsciiRenderer(std::size_t width = 80,
        std::size_t height = 24,
        ClearMode mode = ClearMode::Auto);

    void SetClearMode(ClearMode mode) { mode_ = mode; }
    void SetFrameDelayMs(int ms) { frame_delay_ms_ = ms; }
    int FrameDelayMs() const { return frame_delay_ms_; }

    // Returns true if the renderer cleared the screen (i.e., in-place animation likely)
    bool Render(const World& world,
        const std::vector<Detection>& detections,
        const RenderBounds& bounds,
        int step,
        double time_s);

private:
    std::size_t width_;
    std::size_t height_;
    ClearMode mode_;
    int frame_delay_ms_ = 60; // default “nice” animation delay when clearing works

    std::size_t ToCol(double x, const RenderBounds& b) const;
    std::size_t ToRow(double y, const RenderBounds& b) const;

    bool TryClearScreen() const;
    bool TryClearWin32() const;
    bool TryClearAnsi() const;
};
