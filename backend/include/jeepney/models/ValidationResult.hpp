#pragma once

#include <string>

namespace jeepney {

    struct ValidationResult {
        bool isValid = true;
        std::string message;
    };

} 