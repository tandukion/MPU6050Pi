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
)

# Create executable and its libraries
# add_executable(
#     test_dualmotor 
#     test/test_dualmotor.cpp
# )
# target_link_libraries(
#     test_dualmotor 
#     motorPi
# )

#############
## Install ##
#############

install(TARGETS MPU6050Pi DESTINATION /usr/lib)
install(FILES ${HEADERS} DESTINATION /usr/include)