cmake_minimum_required(VERSION 3.15)
project(floop VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Options
option(BUILD_EXAMPLES "Build example applications" ON)
option(BUILD_TESTS "Build tests" OFF)

# Find packages
find_package(Boost REQUIRED COMPONENTS graph)
find_package(spdlog REQUIRED)
find_package(GTest REQUIRED)

file(GLOB SOURCES "src/*.cpp" "src/*.c")
add_library(${PROJECT_NAME} STATIC
  ${SOURCES}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        src
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
        fmt::fmt
        spdlog::spdlog
        Boost::boost
)

# Compiler-specific options
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

install(TARGETS ${PROJECT_NAME}
  EXPORT ${PROJECT_NAME}Targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION lib/cmake/${PROJECT_NAME}
)

# Tests
if(BUILD_TESTS)
  # Enable testing
  enable_testing()

  file(GLOB TEST_SOURCES "tests/*.cpp")
  # Create test executable
  add_executable(${PROJECT_NAME}_tests ${TEST_SOURCES}
  )

  # Link libraries to test executable
  target_link_libraries(${PROJECT_NAME}_tests
    PRIVATE
        Boost::boost
        spdlog::spdlog
        GTest::gtest
        GTest::gtest_main
  )

  # Set include directories for tests
  target_include_directories(${PROJECT_NAME}_tests
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
        ${CMAKE_CURRENT_SOURCE_DIR}/src
  )

  # Register tests with CTest
  include(GoogleTest)
  gtest_discover_tests(${PROJECT_NAME}_tests)

  # Custom target for running tests
  add_custom_target(run_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS ${PROJECT_NAME}_tests
  )
endif()

# Examples
if(BUILD_EXAMPLES)
  add_executable(${PROJECT_NAME}_example examples/main.cpp)
  target_link_libraries(${PROJECT_NAME}_example ${PROJECT_NAME})
endif()
