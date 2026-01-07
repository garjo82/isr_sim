#pragma once

#include <cstdint>
#include <string>

struct RadarConfig {
    // Identity
    std::string name = "radar_front";

    // Geometry
    double max_range_m = 60.0;            // “ish” for a small ISR-ish demo
    double fov_rad = 2.09439510239;       // 120 deg

    // Measurement noise (applied to range/bearing)
    double sigma_range_m = 0.5;
    double sigma_bearing_rad = 0.0174532925199; // 1 deg

    // Dropouts (misses)
    double p_miss_base = 0.02;
    double p_miss_range_slope = 0.003;    // +0.003 per meter (clamped)

    // False positives / clutter (per Update call)
    double p_false_alarm_per_update = 0.02; // ~2% chance per tick at dt=0.1 => ~0.2/sec

    // Confidence model: pseudo-SNR -> logistic
    double snr0 = 20.0;                   // nominal near-range SNR
    double snr_range_drop = 0.20;         // SNR drops with range (per meter)
    double snr_angle_drop = 2.0;          // SNR drops with |bearing| (per rad)
    double snr_threshold = 10.0;          // around here confidence ~0.5
    double snr_noise_sigma = 1.5;         // random SNR variation
    double conf_alpha = 0.8;              // logistic steepness

    // False positives confidence cap
    double false_conf_max = 0.30;

    // Deterministic reproducibility
    std::uint32_t seed = 456;
};
