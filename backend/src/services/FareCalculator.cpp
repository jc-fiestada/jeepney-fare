#include "jeepney/services/FareCalculator.hpp"

#include <algorithm>
#include <stdexcept>

namespace jeepney {

FareCalculator::FareCalculator(const RouteCatalog& catalog) : catalog_(catalog) {}

FareResult FareCalculator::calculate(const FareRequest& request) const {
    FareResult result;
    result.distanceKm = resolveDistanceKm(request);
    result.passengerType = request.passengerType;

    const int billedKm = std::max(result.distanceKm, kMinimumBilledKm);
    result.regularFareCentavos = regularFareCentavos(billedKm);
    result.discountApplied = isDiscounted(request.passengerType);
    result.fareCentavos = result.discountApplied
                              ? discountedFareCentavos(result.regularFareCentavos)
                              : result.regularFareCentavos;

    if (request.mode == FareMode::ByRoute) {
        result.getOn = request.getOn;
        result.getOff = request.getOff;
    }
    return result;
}

int FareCalculator::resolveDistanceKm(const FareRequest& request) const {
    if (request.mode == FareMode::ByDistance) {
        return request.distanceKm;
    }

    const auto getOn = catalog_.findStop(request.direction, request.getOn);
    const auto getOff = catalog_.findStop(request.direction, request.getOff);
    if (!getOn.has_value() || !getOff.has_value()) {
        throw std::invalid_argument("Unknown stop.");
    }
    return getOff->km - getOn->km;
}

int FareCalculator::regularFareCentavos(int billedKm) {
    const int extraKm = std::max(0, billedKm - kBaseCoveredKm);
    return kBaseFareCentavos + kPerKmCentavos * extraKm;
}

int FareCalculator::discountedFareCentavos(int regularCentavos) {
    const int exact = regularCentavos * kDiscountNumerator / kDiscountDenominator;
    const int halfStep = kRoundingStepCentavos / 2;
    return ((exact + halfStep) / kRoundingStepCentavos) * kRoundingStepCentavos;
}

bool FareCalculator::isDiscounted(PassengerType type) {
    return type != PassengerType::Regular;
}

}  