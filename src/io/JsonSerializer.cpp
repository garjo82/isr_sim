#include "JsonSerializer.h"
#include <nlohmann/json.hpp>
#include "../sim/Entity.h"
#include "../sim/Drone.h"
#include "../sim/Target.h"

using json = nlohmann::json;

std::string JsonSerializer::SerializeFrame(const World& world,
    const std::vector<Detection>& detections,
    double time_s,
    int step) {
    json j;
    j["metadata"] = { {"time", time_s}, {"step", step} };

    // Serialize Entities (Truth)
    j["entities"] = json::array();
    for (const auto& entity_ptr : world.entities()) {
        json e;
        e["id"] = entity_ptr->id();
        // Use RTTI to label types for the Visualizer
        if (dynamic_cast<Drone*>(entity_ptr.get())) e["type"] = "drone";
        else if (dynamic_cast<Target*>(entity_ptr.get())) e["type"] = "target";

        e["x"] = entity_ptr->pose().x;
        e["y"] = entity_ptr->pose().y;
        e["heading"] = entity_ptr->pose().heading;
        j["entities"].push_back(e);
    }

    // Serialize Detections (Perception)
    j["detections"] = json::array();
    for (const auto& d : detections) {
        json det;
        det["sensor"] = d.sensor_name;
        det["id"] = d.detection_id;
        det["x"] = d.x_world;
        det["y"] = d.y_world;
        det["conf"] = d.confidence;
        det["is_fp"] = d.is_false_positive;
        j["detections"].push_back(det);
    }

    return j.dump(); // Returns minimized string for network efficiency
}