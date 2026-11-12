#pragma once

#include <string>

#include "httplib.h"
#include "jeepney/services/FareValidator.hpp"
#include "jeepney/services/IFareCalculator.hpp"

namespace jeepney {

struct HttpReply {
    int status = 200;
    std::string body;
};

class FareController {
    public:
        FareController(const FareValidator& validator, const IFareCalculator& calculator);

        void registerRoutes(httplib::Server& server) const;

    private:
        HttpReply handleFare(const std::string& body) const;
        HttpReply handleHealth() const;

        const FareValidator& validator_;
        const IFareCalculator& calculator_;
    };

}