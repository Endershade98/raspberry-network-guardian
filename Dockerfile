FROM arm64v8/debian:bullseye-slim

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

COPY . /app

RUN mkdir build && cd build && cmake .. && make

CMD ["./build/network_guardian"]