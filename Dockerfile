FROM ustclug/debian:bookworm-slim AS build

RUN apt update && apt install -y --no-install-recommends \
    build-essential cmake libsfml-dev libfribidi-dev libtag1-dev && \
    mkdir /workspace
COPY CMakeLists.txt /workspace/
COPY src/ /workspace/src/
COPY include/ /workspace/include/
COPY resources/ /workspace/resources/

WORKDIR /workspace
RUN mkdir build && cd build && \
    cmake -DCMAKE_BUILD_TYPE=Debug -DENABLE_ASAN=ON .. && make -j$(nproc)

FROM ustclug/debian:bookworm-slim
RUN apt update && apt install -y --no-install-recommends \
    libsfml-audio2.5 libsfml-graphics2.5 libsfml-network2.5 \
    libsfml-system2.5 libsfml-window2.5 libfribidi0 libtag1v5 \
    libasan8 libopengl0 \
    pipewire pipewire-alsa alsa-utils \
    libxext6 libxrender1 libxtst6 libxi6 adduser && \
    mkdir /workspace && \
    adduser --disabled-password --gecos "" marsshooter

COPY --from=build /workspace/marsshooter /workspace/marsshooter
COPY data/ /workspace/data/
WORKDIR /workspace

RUN mkdir -p /run/user/1000 && chown marsshooter: /run/user/1000 && chmod 700 /run/user/1000
USER marsshooter:marsshooter

CMD ["/workspace/marsshooter"]
