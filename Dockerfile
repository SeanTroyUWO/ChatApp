# Stage 1: Build the application
FROM gcc:13 AS builder

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    git \
    build-essential \
    cmake \
    libssl-dev \
    zlib1g-dev \
    ninja-build \
    libasio-dev

WORKDIR /app
COPY . .
RUN ls -la
RUN ls -la crow
#RUN #git submodule init && git submodule update
RUN git clone https://github.com/CrowCpp/Crow.git crow
RUN mkdir server_build && cd server_build && cmake .. && ninja -j 12


# Stage 2: Run the application
FROM ubuntu:24.04

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    libssl-dev \
    zlib1g-dev \
    libpq-dev

WORKDIR /app
COPY --from=builder /app/server_build/server .
# Expose the port your application listens on
EXPOSE 8080 
CMD ["./server"]
