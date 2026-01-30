cmake_minimum_required(VERSION 4.1)
project(steganography C)

set(CMAKE_C_STANDARD 11)

add_executable(steganography main.c
        stego.h
        stego.c)
