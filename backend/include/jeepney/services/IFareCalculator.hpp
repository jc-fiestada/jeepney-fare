#pragma once

#include "jeepney/models/FareRequest.hpp"
#include "jeepney/models/FareResult.hpp"

namespace jeepney {

    class IFareCalculator {
        public:
            virtual ~IFareCalculator() = default;

            virtual FareResult calculate(const FareRequest& request) const = 0;
        };

}