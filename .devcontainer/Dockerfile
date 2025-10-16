FROM ubuntu:25.10

RUN apt-get update && \
    apt-get install -y build-essential cmake ninja-build llvm clang clang-format git && \
    rm -rf /var/lib/apt/lists/*
