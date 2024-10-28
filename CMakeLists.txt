cmake_minimum_required(VERSION 3.5)
project(DeribitOrder)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_VERBOSE_MAKEFILE ON)

# Use libc++ for standard library if needed
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")

# Boost library setup
find_package(Boost 1.66 REQUIRED COMPONENTS system thread filesystem chrono regex)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "Boost libraries not found.")
endif()

# OpenSSL library setup
find_package(OpenSSL REQUIRED)
if(OpenSSL_FOUND)
    include_directories(${OPENSSL_INCLUDE_DIR})
    link_directories(${OPENSSL_LIBRARIES})
else()
    message(FATAL_ERROR "OpenSSL libraries not found.")
endif()

# Hiredis library setup
find_path(HIREDIS_INCLUDE_DIR hiredis/hiredis.h)
find_library(HIREDIS_LIB hiredis)
if(HIREDIS_INCLUDE_DIR AND HIREDIS_LIB)
    include_directories(${HIREDIS_INCLUDE_DIR})
    link_directories(${HIREDIS_LIB})
else()
    message(FATAL_ERROR "Hiredis libraries not found.")
endif()

# CppRestSDK setup
find_path(CPPRESTSDK_INCLUDE_DIR cpprest/json.h)
find_library(CPPRESTSDK_LIB cpprest)
if(CPPRESTSDK_INCLUDE_DIR AND CPPRESTSDK_LIB)
    include_directories(${CPPRESTSDK_INCLUDE_DIR})
    link_directories(${CPPRESTSDK_LIB})
else()
    message(FATAL_ERROR "CppRestSDK not found.")
endif()

# Add executable target
add_executable(DeribitOrder main.cpp)

# Link libraries
target_link_libraries(DeribitOrder PRIVATE
    ${HIREDIS_LIB}
    Boost::system Boost::thread Boost::filesystem
    ${OPENSSL_LIBRARIES}
    ${CPPRESTSDK_LIB}
)