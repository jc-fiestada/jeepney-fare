#include "jeepney/controllers/FareController.hpp"

#include <stdexcept>

#include "json.hpp"

namespace jeepney {

    namespace {

    using json = nlohmann::json;

    constexpr const char* kJsonContentType = "application/json; charset=utf-8";
    constexpr long long kMaxReasonableDistance = 1000000;

    HttpReply errorReply(int status, const std::string& message) {
        return HttpReply{status, json{{"error", message}}.dump()};
    }

    void writeReply(httplib::Response& response, const HttpReply& reply) {
        response.status = reply.status;
        response.set_content(reply.body, kJsonContentType);
    }

    std::string requireString(const json& payload, const char* key) {
        const auto it = payload.find(key);
        if (it == payload.end() || !it->is_string()) {
            throw std::invalid_argument(std::string("Missing or invalid field: ") + key);
        }
        return it->get<std::string>();
    }

    int requireWholeNumber(const json& payload, const char* key) {
        const auto it = payload.find(key);
        if (it == payload.end() || !it->is_number_integer()) {
            throw std::invalid_argument("Distance must be a whole number of kilometers.");
        }
        const long long value = it->get<long long>();
        if (value < -kMaxReasonableDistance || value > kMaxReasonableDistance) {
            throw std::invalid_argument("Distance is out of range.");
        }
        return static_cast<int>(value);
    }

    FareMode parseMode(const std::string& value) {
        if (value == "route") return FareMode::ByRoute;
        if (value == "distance") return FareMode::ByDistance;
        throw std::invalid_argument("Unknown fare mode.");
    }

    Direction parseDirection(const std::string& value) {
        if (value == "gma-to-binan") return Direction::GmaToBinan;
        if (value == "binan-to-gma") return Direction::BinanToGma;
        throw std::invalid_argument("Unknown direction.");
    }

    PassengerType parsePassengerType(const std::string& value) {
        if (value == "regular") return PassengerType::Regular;
        if (value == "student") return PassengerType::Student;
        if (value == "senior") return PassengerType::SeniorCitizen;
        if (value == "pwd") return PassengerType::Pwd;
        throw std::invalid_argument("Unknown passenger type.");
    }

    std::string toString(PassengerType type) {
        switch (type) {
            case PassengerType::Student: return "student";
            case PassengerType::SeniorCitizen: return "senior";
            case PassengerType::Pwd: return "pwd";
            case PassengerType::Regular: break;
        }
        return "regular";
    }

    FareRequest parseRequest(const json& payload) {
        FareRequest request;
        request.mode = parseMode(requireString(payload, "mode"));
        request.passengerType = parsePassengerType(requireString(payload, "passengerType"));

        if (request.mode == FareMode::ByRoute) {
            request.direction = parseDirection(requireString(payload, "direction"));
            request.getOn = requireString(payload, "getOn");
            request.getOff = requireString(payload, "getOff");
        } else {
            request.distanceKm = requireWholeNumber(payload, "distanceKm");
        }
        return request;
    }

    json toJson(const FareResult& result) {
        json body;
        body["distanceKm"] = result.distanceKm;
        body["passengerType"] = toString(result.passengerType);
        body["regularFare"] = result.regularFareCentavos / 100.0;
        body["fare"] = result.fareCentavos / 100.0;
        body["discountApplied"] = result.discountApplied;
        if (!result.getOn.empty()) {
            body["getOn"] = result.getOn;
        }
        if (!result.getOff.empty()) {
            body["getOff"] = result.getOff;
        }
        return body;
    }

    }  

    FareController::FareController(const FareValidator& validator, const IFareCalculator& calculator)
        : validator_(validator), calculator_(calculator) {}

    void FareController::registerRoutes(httplib::Server& server) const {
        server.Post("/api/fare", [this](const httplib::Request& request, httplib::Response& response) {
            writeReply(response, handleFare(request.body));
        });

        server.Get("/api/health", [this](const httplib::Request&, httplib::Response& response) {
            writeReply(response, handleHealth());
        });
    }

    HttpReply FareController::handleFare(const std::string& body) const {
        const json payload = json::parse(body, nullptr, false);
        if (payload.is_discarded() || !payload.is_object()) {
            return errorReply(400, "Request body must be a JSON object.");
        }

        try {
            const FareRequest request = parseRequest(payload);

            const ValidationResult validation = validator_.validate(request);
            if (!validation.isValid) {
                return errorReply(400, validation.message);
            }

            const FareResult result = calculator_.calculate(request);
            return HttpReply{200, toJson(result).dump()};
        } catch (const std::invalid_argument& error) {
            return errorReply(400, error.what());
        } catch (const std::exception&) {
            return errorReply(500, "Unexpected server error.");
        }
    }

    HttpReply FareController::handleHealth() const {
        return HttpReply{200, json{{"status", "ok"}}.dump()};
    }

}