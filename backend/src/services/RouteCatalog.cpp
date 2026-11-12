#include "jeepney/services/RouteCatalog.hpp"

#include <algorithm>
#include <array>
#include <cstddef>

namespace jeepney {

namespace {

constexpr std::array<const char*, 17> kStopNames = {
    "G.M.A.", "Granville", "Bancal", "Beatway", "Global", "Southcoast",
    "Carbon", "Zigzag", "Quiary Checkpoint", "Waller Mart", "Petron",
    "Golden Mile", "Motorola", "Soro soro", "Napocor", "Olivarez", "Biñan"};

}  

RouteCatalog::RouteCatalog() {
    const std::size_t last = kStopNames.size() - 1;
    for (std::size_t km = 0; km <= last; ++km) {
        gmaToBinan_.push_back(Stop{kStopNames[km], static_cast<int>(km)});
        binanToGma_.push_back(Stop{kStopNames[last - km], static_cast<int>(km)});
    }
}

const std::vector<Stop>& RouteCatalog::stops(Direction direction) const {
    return direction == Direction::GmaToBinan ? gmaToBinan_ : binanToGma_;
}

std::optional<Stop> RouteCatalog::findStop(Direction direction, const std::string& name) const {
    const std::vector<Stop>& list = stops(direction);
    const auto match = std::find_if(list.begin(), list.end(),
                                    [&name](const Stop& stop) { return stop.name == name; });
    if (match == list.end()) {
        return std::nullopt;
    }
    return *match;
}

int RouteCatalog::totalDistanceKm() const {
    return gmaToBinan_.back().km;
}

} 