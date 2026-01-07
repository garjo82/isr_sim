#include "RadarSensor.h"

#include "../sim/Simulation.h"
#include "../sim/World.h"
#include "../sim/Drone.h"
#include "../sim/Target.h"
#include "../sim/Pose.h"

#include <algorithm>
#include <cmath>

static double Clamp01(double v) {
    return std::max(0.0, std::min(1.0, v));
}

RadarSensor::RadarSensor(RadarConfig cfg)
    : cfg_(std::move(cfg)),
    rng_(cfg_.seed),
    n_range_(0.0, cfg_.sigma_range_m),
    n_bearing_(0.0, cfg_.sigma_bearing_rad),
    n_snr_(0.0, cfg_.snr_noise_sigma),
    u01_(0.0, 1.0) {
}

double RadarSensor::ComputeConfidence(double range_m, double bearing_rad) {
    // Pseudo-SNR model: decreases with range and off-axis angle, plus noise.
    const double snr =
        cfg_.snr0
        - cfg_.snr_range_drop * range_m
        - cfg_.snr_angle_drop * std::abs(bearing_rad)
        + n_snr_(rng_);

    // Logistic mapping from SNR -> confidence in [0,1]
    const double x = cfg_.conf_alpha * (snr - cfg_.snr_threshold);
    const double conf = 1.0 / (1.0 + std::exp(-x));
    return Clamp01(conf);
}

bool RadarSensor::ShouldMiss(double range_m) {
    double p = cfg_.p_miss_base + cfg_.p_miss_range_slope * range_m;
    p = Clamp01(p);
    return (u01_(rng_) < p);
}

Detection RadarSensor::MakeDetectionFromMeasurement(
    double drone_x, double drone_y, double drone_heading,
    double range_m, double bearing_rad,
    double confidence, bool is_false_positive) {

    // Convert polar -> Cartesian in drone/sensor frame
    const double x_rel = range_m * std::cos(bearing_rad);
    const double y_rel = range_m * std::sin(bearing_rad);

    // Rotate into world frame using drone heading, then translate
    const double ch = std::cos(drone_heading);
    const double sh = std::sin(drone_heading);

    const double x_world = drone_x + ch * x_rel - sh * y_rel;
    const double y_world = drone_y + sh * x_rel + ch * y_rel;

    Detection d;
    d.sensor_name = cfg_.name;
    d.detection_id = next_detection_id_++;
    d.x_world = x_world;
    d.y_world = y_world;
    d.range_m = range_m;
    d.bearing_rad = bearing_rad;
    d.confidence = Clamp01(confidence);
    d.is_false_positive = is_false_positive;
    return d;
}

Detection RadarSensor::MakeFalsePositive(
    double drone_x, double drone_y, double drone_heading) {

    // Sample a random clutter return in sensor frame within FOV/range.
    const double half_fov = 0.5 * cfg_.fov_rad;

    // Uniform in range (simple). You could switch to sqrt(U)*R for uniform area later.
    const double r = u01_(rng_) * cfg_.max_range_m;
    const double theta = (-half_fov) + u01_(rng_) * (2.0 * half_fov);

    // Low confidence false positive
    const double conf = u01_(rng_) * cfg_.false_conf_max;

    return MakeDetectionFromMeasurement(
        drone_x, drone_y, drone_heading,
        r, theta,
        conf, true);
}

std::vector<Detection> RadarSensor::Update(const Simulation& sim) {
    std::vector<Detection> out;

    // Find first drone (prototype assumption: 1 drone)
    const Drone* drone = nullptr;
    Pose dp;

    for (const auto& sp : sim.world().entities()) {
        if (auto d = dynamic_cast<const Drone*>(sp.get())) {
            drone = d;
            dp = d->pose();
            break;
        }
    }
    if (!drone) return out;

    const double half_fov = 0.5 * cfg_.fov_rad;

    // False positives (clutter)
    if (u01_(rng_) < cfg_.p_false_alarm_per_update) {
        out.push_back(MakeFalsePositive(dp.x, dp.y, dp.heading));
    }

    // For each truth target, produce a detection if visible and not missed
    for (const auto& sp : sim.world().entities()) {
        const auto* tgt = dynamic_cast<const Target*>(sp.get());
        if (!tgt) continue;

        const Pose& tp = tgt->pose();

        // Relative vector in world frame
        const double dx = tp.x - dp.x;
        const double dy = tp.y - dp.y;

        // Rotate world delta into drone/sensor frame (rotate by -heading)
        const double ch = std::cos(dp.heading);
        const double sh = std::sin(dp.heading);

        const double x_rel = ch * dx + sh * dy;
        const double y_rel = -sh * dx + ch * dy;

        const double r_true = std::sqrt(x_rel * x_rel + y_rel * y_rel);
        const double theta_true = std::atan2(y_rel, x_rel);

        // Range/FOV gating
        if (r_true > cfg_.max_range_m) continue;
        if (std::abs(theta_true) > half_fov) continue;

        // Miss/dropout model
        if (ShouldMiss(r_true)) continue;

        // Add measurement noise in polar space
        double r_meas = r_true + n_range_(rng_);
        double theta_meas = theta_true + n_bearing_(rng_);

        if (r_meas < 0.0) r_meas = 0.0;

        const double conf = ComputeConfidence(r_true, theta_true);

        out.push_back(MakeDetectionFromMeasurement(
            dp.x, dp.y, dp.heading,
            r_meas, theta_meas,
            conf, false));
    }

    return out;
}
