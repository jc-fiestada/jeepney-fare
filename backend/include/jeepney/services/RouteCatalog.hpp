#pragma once

#include <optional>
#include <string>
#include <vector>

#include "jeepney/models/Direction.hpp"
#include "jeepney/models/Stop.hpp"

namespace jeepney {

    class RouteCatalog {
        public:
            RouteCatalog();

            const std::vector<Stop>& stops(Direction direction) const;
            std::optional<Stop> findStop(Direction direction, const std::string& name) const;
            int totalDistanceKm() const;

        private:
            std::vector<Stop> gmaToBinan_;
            std::vector<Stop> binanToGma_;
        };

}