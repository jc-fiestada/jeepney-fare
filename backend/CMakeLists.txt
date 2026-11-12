cmake_minimum_required(VERSION 3.20)
project(JeepneyFareGuide LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

add_executable(jeepney_server
  src/main.cpp
  src/controllers/FareController.cpp
  src/services/FareCalculator.cpp
  src/services/FareValidator.cpp
  src/services/RouteCatalog.cpp
)

# "include" holds our own headers, "third_party" holds httplib.h and json.hpp
target_include_directories(jeepney_server PRIVATE include third_party)

find_package(Threads REQUIRED)
target_link_libraries(jeepney_server PRIVATE Threads::Threads)

if(WIN32)
  target_compile_definitions(jeepney_server PRIVATE
    _WIN32_WINNT=0x0A00
    NOMINMAX
    WIN32_LEAN_AND_MEAN
  )
  target_link_libraries(jeepney_server PRIVATE ws2_32)
else()
  # Keep the Linux binary independent of the compiler image's runtime libraries
  target_link_options(jeepney_server PRIVATE -static-libstdc++ -static-libgcc)
endif()