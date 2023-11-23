cmake_minimum_required(VERSION 3.8)
project(MBISTPart)
# SET(CMAKE_BUILD_TYPE "DEBUG")
# SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -fPIC -pthread")
SET(CMAKE_BUILD_TYPE "RELEASE")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -fPIC -fpermissive -static -pthread")
set(SOURCES MBISTPart.cpp parser.cpp writeInfo.cpp parseFile.cpp BK.cpp genetic.cpp)



add_executable(${PROJECT_NAME} ${SOURCES})
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)
target_link_libraries(MBISTPart   stdc++fs )