
FROM node:22-bookworm-slim AS frontend-build
WORKDIR /src/frontend
COPY frontend/package.json frontend/package-lock.json ./
RUN npm ci
COPY frontend/ ./

RUN npm run build

FROM debian:bookworm-slim AS backend-build
RUN apt-get update \
    && apt-get install -y --no-install-recommends build-essential cmake ninja-build \
    && rm -rf /var/lib/apt/lists/*
WORKDIR /src
COPY backend/ ./backend/
RUN cmake -S backend -B build -G Ninja -DCMAKE_BUILD_TYPE=Release \
    && cmake --build build

FROM debian:bookworm-slim AS final
WORKDIR /app
COPY --from=backend-build /src/build/jeepney_server ./jeepney_server
COPY --from=frontend-build /src/wwwroot ./wwwroot

ENV PORT=8080
EXPOSE 8080

RUN useradd --system --no-create-home appuser
USER appuser

ENTRYPOINT ["./jeepney_server"]