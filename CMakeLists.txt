# Toplevel CMake file for the kernel project

cmake_minimum_required (VERSION 2.6)

project (KERNEL C ASM)
set (CMAKE_ASM_COMPILER /usr/bin/nasm)

include_directories (inc)

add_subdirectory (src)
