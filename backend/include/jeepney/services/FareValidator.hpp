#pragma once

#include "jeepney/models/FareRequest.hpp"
#include "jeepney/models/ValidationResult.hpp"
#include "jeepney/services/RouteCatalog.hpp"

namespace jeepney {

    class FareValidator {
        public:
            explicit FareValidator(const RouteCatalog& catalog);

            ValidationResult validate(const FareRequest& request) const;

        private:
            ValidationResult validateDistance(const FareRequest& request) const;
            ValidationResult validateRoute(const FareRequest& request) const;

            static constexpr int kMinDistanceKm = 1;

            const RouteCatalog& catalog_;
        };

}