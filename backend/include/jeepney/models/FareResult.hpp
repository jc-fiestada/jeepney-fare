#pragma once

#include <string>

#include "jeepney/models/PassengerType.hpp"

namespace jeepney {

    struct FareResult {
        int distanceKm = 0;
        PassengerType passengerType = PassengerType::Regular;
        int regularFareCentavos = 0;
        int fareCentavos = 0;
        bool discountApplied = false;
        std::string getOn;
        std::string getOff;
    };

}  