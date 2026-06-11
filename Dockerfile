# Development/build container for C++ Qt5 + CUDA fractal application
# For production, pre-build locally and use a two-stage approach to copy the binary

FROM nvidia/cuda:12.4.1-devel-ubuntu22.04

ENV DEBIAN_FRONTEND=noninteractive

# Install build essentials and dependencies
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    curl \
    git \
    ninja-build \
    pkg-config \
    unzip \
    zip \
    libxinerama-dev \
    libxcursor-dev \
    xorg-dev \
    libglu1-mesa-dev \
    libqt5core5a \
    libqt5gui5 \
    libqt5qml5 \
    libqt5quick5 \
    libglew-dev \
    libglfw3-dev \
    libgl1-mesa-dev \
    libxkbcommon-dev \
    libx11-dev \
    libxext-dev \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
WORKDIR /opt/vcpkg
RUN git clone --depth 1 https://github.com/Microsoft/vcpkg.git . && \
    ./bootstrap-vcpkg.sh -disableMetrics

ENV PATH="/opt/vcpkg:${PATH}"

# Set up application directory
WORKDIR /app

# Copy source code
COPY . .

# Build instructions (user builds with: docker build -t fractal-julia . && docker run fractal-julia)
RUN mkdir -p build

WORKDIR /app/build

# Set environment for headless/GPU execution
ENV QT_QPA_PLATFORM=offscreen
ENV LD_LIBRARY_PATH=/usr/lib/x86_64-linux-gnu:$LD_LIBRARY_PATH
ENV CMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake

# Default command for manual builds
CMD ["/bin/bash"]
