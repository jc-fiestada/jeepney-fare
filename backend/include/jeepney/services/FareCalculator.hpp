#pragma once

#include "jeepney/services/IFareCalculator.hpp"
#include "jeepney/services/RouteCatalog.hpp"

namespace jeepney {

    class FareCalculator : public IFareCalculator {
        public:
            explicit FareCalculator(const RouteCatalog& catalog);

            FareResult calculate(const FareRequest& request) const override;

        private:
            int resolveDistanceKm(const FareRequest& request) const;
            static int regularFareCentavos(int billedKm);
            static int discountedFareCentavos(int regularCentavos);
            static bool isDiscounted(PassengerType type);

            static constexpr int kBaseFareCentavos = 1400;
            static constexpr int kBaseCoveredKm = 4;
            static constexpr int kPerKmCentavos = 200;
            static constexpr int kMinimumBilledKm = 1;

            static constexpr int kDiscountNumerator = 4;
            static constexpr int kDiscountDenominator = 5;
            static constexpr int kRoundingStepCentavos = 25;

            const RouteCatalog& catalog_;
        };

}