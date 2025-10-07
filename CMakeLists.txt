cmake_minimum_required(VERSION 3.10)
project(QuantPortfolio)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directory
include_directories(include)

# Collect all source files
file(GLOB SOURCES "src/*.cpp")

add_executable(quant_portfolio ${SOURCES})

