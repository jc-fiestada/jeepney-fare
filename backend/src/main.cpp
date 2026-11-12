#include "httplib.h"

#include <cstdlib>
#include <filesystem>
#include <iostream>
#include <string>

#include "jeepney/controllers/FareController.hpp"
#include "jeepney/services/FareCalculator.hpp"
#include "jeepney/services/FareValidator.hpp"
#include "jeepney/services/RouteCatalog.hpp"

namespace {

    constexpr int kDefaultPort = 8080;
    constexpr const char* kDefaultStaticDir = "wwwroot";

    int readPort() {
        const char* raw = std::getenv("PORT");
        if (raw == nullptr || *raw == '\0') {
            return kDefaultPort;
        }
        try {
            const int port = std::stoi(raw);
            if (port > 0 && port <= 65535) {
                return port;
            }
        } catch (const std::exception&) {
        }
        std::cerr << "Invalid PORT value, using " << kDefaultPort << ".\n";
        return kDefaultPort;
    }

    std::string readStaticDir() {
        const char* raw = std::getenv("WWWROOT_DIR");
        if (raw == nullptr || *raw == '\0') {
            return kDefaultStaticDir;
        }
        return raw;
    }

    }

    int main() {
        const std::string host = "0.0.0.0";
        const int port = readPort();
        const std::string staticDir = readStaticDir();

        jeepney::RouteCatalog catalog;
        jeepney::FareValidator validator(catalog);
        jeepney::FareCalculator calculator(catalog);
        jeepney::FareController controller(validator, calculator);

        httplib::Server server;
        controller.registerRoutes(server);

        if (std::filesystem::is_directory(staticDir) && server.set_mount_point("/", staticDir)) {
            std::cout << "Serving static files from: " << staticDir << '\n';
        } else {
            std::cerr << "Warning: static folder '" << staticDir
                    << "' not found. Only the API is available.\n";
        }

        std::cout << "Jeepney Fare Guide listening on http://" << host << ':' << port << '\n';
        if (!server.listen(host, port)) {
            std::cerr << "Failed to start the server on port " << port << ".\n";
            return 1;
        }
        return 0;
}