# Stage 1: Build the application
FROM ubuntu:24.04 AS builder

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    git \
    build-essential \
    cmake \
    libssl-dev \
    zlib1g-dev \
    ninja-build \
    libasio-dev \
    postgresql-client \
    libpq-dev \
    libpqxx-dev

WORKDIR /app
COPY . .
RUN ls -la
RUN ls -la crow
#RUN #git submodule init && git submodule update
RUN git clone --depth 1 https://github.com/CrowCpp/Crow.git crow
RUN mkdir server_build && cd server_build && cmake -G Ninja .. && ninja -j 12


# Stage 2: Run the application
FROM ubuntu:24.04 AS production

RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    libssl-dev \
    zlib1g-dev \
    postgresql-client \
    libpq-dev \
    libpqxx-dev 

RUN ls -la /lib
RUN ls -la /lib/x86_64-linux-gnu
RUN ls -la /usr/lib
RUN ls -la /usr/lib/x86_64-linux-gnu

WORKDIR /app
COPY --from=builder /app/server_build/server .

RUN ldd ./server
# Expose the port your application listens on
EXPOSE 8080 
CMD ["./server"]
