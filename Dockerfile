# Fairly recent release.
FROM debian:11-slim

# Install necessary packages.
RUN apt-get update && apt-get install -y build-essential clang cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev git gcc-mingw-w64 g++-mingw-w64 mesa-common-dev