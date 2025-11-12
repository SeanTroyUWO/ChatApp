# Stage 1: Build the application
FROM gcc:13 AS builder

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    git \
    build-essential \
    cmake \
    libssl-dev \
    zlib1g-dev 

WORKDIR /opt
RUN git clone --depth 1 https://github.com/SeanTroyUWO/oatpp.git

# Build OAT++
WORKDIR /opt/oatpp
RUN mkdir build && cd build && cmake .. && make -j 12 install

WORKDIR /opt
RUN git clone --depth 1 https://github.com/SeanTroyUWO/oatpp-postgresql.git

# Build OAT_postgres_client
WORKDIR /opt/oatpp-postgresql
RUN mkdir build && cd build && cmake .. && make -j 12 install

WORKDIR /app
COPY . .
RUN mkdir server_build && cd server_build && cmake .. && make -j 12


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
