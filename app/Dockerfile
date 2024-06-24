# Use a base image with g++ and OpenCV installed
FROM ubuntu:latest

# Install necessary packages
RUN apt-get update && apt-get install -y \
    g++ \
    libopencv-dev \
    pkg-config \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Set the working directory
WORKDIR /app

# Create the bin directory
RUN mkdir -p /app/bin
RUN mkdir -p /app/

# Copy the source code to the container
COPY nearmisses /app/nearmisses
COPY resources /app/resources
COPY src/library /app/library
COPY src/models /app/models
COPY config /app/config
COPY src/main.cpp /app/main.cpp

# Compile the code
RUN g++ -std=c++14 -pthread -o /app/bin/eyeson /app/main.cpp $(pkg-config --cflags --libs opencv4)

# Set the entry point
CMD ["/app/bin/eyeson"]
