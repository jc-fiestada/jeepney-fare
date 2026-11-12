# Jeepney Fare Guide

Fare calculator for the G.M.A. to Biñan jeepney route (Carmona, Philippines).
TypeScript + Parcel frontend, C++ backend (cpp-httplib + nlohmann/json).

## Structure

- `frontend/` TypeScript and HTML source. Parcel builds it into `wwwroot/`.
- `backend/` C++20 REST API that also serves `wwwroot/`.
- `wwwroot/` Parcel output (git-ignored).
- `Dockerfile` multi-stage build used for deployment.

## Requirements

- Node.js 22 LTS
- CMake 3.20+, Ninja, a C++20 compiler (MSYS2 UCRT64 g++ on Windows)
- Docker (only for container testing and deployment)

## Local development

Two terminals, both from the project root.

Backend (port 8080):

    cmake -S backend -B backend/build -G Ninja
    cmake --build backend/build
    .\backend\build\jeepney_server.exe

Frontend dev server (port 1234, proxies /api to 127.0.0.1:8080):

    cd frontend
    npm install
    npm run dev

Open http://localhost:1234

## Production build (without Docker)

    cd frontend
    npm run build
    cd ..
    .\backend\build\jeepney_server.exe

Open http://localhost:8080 (the C++ server serves the pages and the API).

## Docker

    docker build -t jeepney-fare-guide .
    docker run --rm -p 8080:8080 jeepney-fare-guide

## Configuration

| Variable | Default | Purpose |
|---|---|---|
| `PORT` | `8080` | Port to listen on (Render sets this) |
| `WWWROOT_DIR` | `wwwroot` | Folder with the built frontend |

## API

- `POST /api/fare`
  - By route: `{"mode":"route","direction":"gma-to-binan","getOn":"Granville","getOff":"Bancal","passengerType":"student"}`
  - By distance: `{"mode":"distance","distanceKm":8,"passengerType":"regular"}`
- `GET /api/health` returns `{"status":"ok"}`

Passenger types: `regular`, `student`, `senior`, `pwd`.
Directions: `gma-to-binan`, `binan-to-gma`.