cmake_minimum_required(VERSION 2.8.9)
project(MPU6050Pi)
set(CMAKE_BUILD_TYPE Release)
 
###########
## Build ##
###########
include_directories(
    include
)

# Wildcard for all headers and implementations:
file(GLOB HEADERS "include/*.h")
file(GLOB SOURCES "src/*.cpp")

add_library(
    MPU6050Pi SHARED 
    ${SOURCES}
)
target_link_libraries(
    MPU6050Pi
    libwiringPi.so
    bcm2835
)

# Create executable and its libraries
add_executable(read_MPU6050Pi example/read_MPU6050Pi.cpp)
target_link_libraries(read_MPU6050Pi MPU6050Pi)

add_executable(MPU6050_example_1 example/MPU6050_example_1.cpp)
target_link_libraries(MPU6050_example_1 MPU6050Pi)

add_executable(auto_calibration src/auto_calibration.cpp)
target_link_libraries(auto_calibration MPU6050Pi)

#############
## Install ##
#############

install(TARGETS MPU6050Pi DESTINATION /usr/lib)
install(FILES ${HEADERS} DESTINATION /usr/include)