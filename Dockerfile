# Stage 1: Build the application
FROM gcc:12.1.0 AS builder
WORKDIR /app
COPY . .
RUN g++ -O3 main.cpp -o my_app_executable # Adjust this command to your build process

# Stage 2: Run the application
FROM ubuntu:22.04
WORKDIR /app
COPY --from=builder /app/my_app_executable .
# Expose the port your application listens on
EXPOSE 8080 
CMD ["./my_app_executable"]
