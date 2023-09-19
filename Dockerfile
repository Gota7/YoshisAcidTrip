# Fairly recent ubuntu release.
FROM debian:11-slim

# Number of threads used to build.
ENV NUM_CORES=8

# What backend to use. Currently only GL is supported. This option is ignored for 3ds.
ENV BACKEND="GL"

# What OS to use. Supports Linux, Darwin, Windows, and 3ds.
ENV OS="Linux"

# If the build is for debug purposes.
ENV DEBUG=OFF

# If to use tracy.
ENV TRACY=OFF

# If to do a static build.
ENV STATIC_BUILD=ON

# Extra cmake args (for toolchains).
ENV CMAKE_EXTRA=""

# Target specification.
ENV TARGET=LSD

# Install necessary packages.
RUN apt-get update && apt-get install -y build-essential clang cmake libx11-dev libxrandr-dev libxinerama-dev libxcursor-dev libxi-dev git
RUN apt-get install -y gcc-mingw-w64 g++-mingw-w64

# Mount the game files and build.
WORKDIR /YoshisAcidTrip
CMD date > res/BUILDTIME && \
    mkdir -p build && \
    mkdir -p bin && \
    cd build && \
    cmake .. -DBACKEND=${BACKEND} -DOS=${OS} -DSTATIC_BUILD=${STATIC_BUILD} -DDEBUG=${DEBUG} -DTRACY=${TRACY} ${CMAKE_EXTRA} && \
    cmake --build . -j${NUM_CORES} --target ${TARGET} && \
    cd ..