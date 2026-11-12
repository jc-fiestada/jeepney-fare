#pragma once

#include <string>

#include "jeepney/models/Direction.hpp"
#include "jeepney/models/FareMode.hpp"
#include "jeepney/models/PassengerType.hpp"

namespace jeepney {

    struct FareRequest {
        FareMode mode = FareMode::ByRoute;
        PassengerType passengerType = PassengerType::Regular;
        Direction direction = Direction::GmaToBinan;
        std::string getOn;
        std::string getOff;
        int distanceKm = 0;
    };

}