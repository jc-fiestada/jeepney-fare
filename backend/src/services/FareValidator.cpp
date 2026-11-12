#include "jeepney/services/FareValidator.hpp"

#include <string>

namespace jeepney {

FareValidator::FareValidator(const RouteCatalog& catalog) : catalog_(catalog) {}

ValidationResult FareValidator::validate(const FareRequest& request) const {
    if (request.mode == FareMode::ByDistance) {
        return validateDistance(request);
    }
    return validateRoute(request);
}

ValidationResult FareValidator::validateDistance(const FareRequest& request) const {
    const int maxDistanceKm = catalog_.totalDistanceKm();
    if (request.distanceKm < kMinDistanceKm || request.distanceKm > maxDistanceKm) {
        return ValidationResult{
            false, "Distance must be between " + std::to_string(kMinDistanceKm) + " and " +
                       std::to_string(maxDistanceKm) + " km."};
    }
    return ValidationResult{};
}

ValidationResult FareValidator::validateRoute(const FareRequest& request) const {
    const auto getOn = catalog_.findStop(request.direction, request.getOn);
    if (!getOn.has_value()) {
        return ValidationResult{false, "Unknown Get On stop for this direction."};
    }

    const auto getOff = catalog_.findStop(request.direction, request.getOff);
    if (!getOff.has_value()) {
        return ValidationResult{false, "Unknown Get Off stop for this direction."};
    }

    if (getOff->km < getOn->km) {
        return ValidationResult{
            false, "Get Off is behind Get On for this direction. Please switch direction."};
    }

    return ValidationResult{};
}

}  