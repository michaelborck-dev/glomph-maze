# CMakeLists.txt - Modern build system for Glomph Maze
# Copyright 2025, modernization by Claude
# Based on simple.mk

cmake_minimum_required(VERSION 3.15)
project(glomph-maze VERSION 0.7.0 LANGUAGES C)

# C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Generate compile_commands.json for IDEs
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Compiler warnings
if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
    add_compile_options(
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
    )
endif()

# Find curses library
find_package(Curses REQUIRED)

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CURSES_INCLUDE_DIR}
)

# Common source files
set(COMMON_SOURCES
    src/myman.c
    src/utils.c
    src/logic.c
)

# Define size variants with their tile/sprite files
set(SIZE_HUGE_TILES "chr5x3.txt")
set(SIZE_HUGE_SPRITES "spr10x6.txt")
set(SIZE_BIG_TILES "chr4.txt")  # Default
set(SIZE_BIG_SPRITES "spr8.txt")  # Default
set(SIZE_SMALL_TILES "khr2h.txt")
set(SIZE_SMALL_SPRITES "spr2h.txt")
set(SIZE_SQUARE_TILES "khr1.txt")
set(SIZE_SQUARE_SPRITES "spr1.txt")

# Helper macro to create size variant
macro(add_size_variant name size tiles sprites)
    add_executable(${name} ${COMMON_SOURCES})
    
    target_compile_definitions(${name} PRIVATE
        MYMANSIZE="${size}"
        TILEFILE="chr/${tiles}"
        SPRITEFILE="spr/${sprites}"
    )
    
    target_link_libraries(${name} ${CURSES_LIBRARIES})
    
    # Install target
    install(TARGETS ${name} DESTINATION bin)
endmacro()

# Create the four size variants (matches simple.mk)
add_size_variant(hugeman "huge" ${SIZE_HUGE_TILES} ${SIZE_HUGE_SPRITES})
add_size_variant(bigman "big" ${SIZE_BIG_TILES} ${SIZE_BIG_SPRITES})
add_size_variant(smallman "small" ${SIZE_SMALL_TILES} ${SIZE_SMALL_SPRITES})
add_size_variant(squareman "square" ${SIZE_SQUARE_TILES} ${SIZE_SQUARE_SPRITES})

# Default target is bigman (symlink as glomph-maze)
add_custom_target(glomph-maze ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink bigman glomph-maze
    DEPENDS bigman
    COMMENT "Creating glomph-maze symlink to bigman"
)

# Install data files
install(DIRECTORY lvl/ DESTINATION share/glomph-maze-${PROJECT_VERSION}/lvl
    FILES_MATCHING PATTERN "*.txt" PATTERN "*.asc")
install(DIRECTORY chr/ DESTINATION share/glomph-maze-${PROJECT_VERSION}/chr
    FILES_MATCHING PATTERN "*.txt" PATTERN "*.asc")
install(DIRECTORY spr/ DESTINATION share/glomph-maze-${PROJECT_VERSION}/spr
    FILES_MATCHING PATTERN "*.txt" PATTERN "*.asc")

# Install documentation
install(FILES README.md LICENSE AUTHORS THANKS NEWS CHANGELOG.md
    DESTINATION share/doc/glomph-maze-${PROJECT_VERSION})

# Code quality targets
find_program(CLANG_FORMAT clang-format)
if(CLANG_FORMAT)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT} -i
            ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running clang-format"
    )
endif()

find_program(CLANG_TIDY clang-tidy)
if(CLANG_TIDY)
    add_custom_target(lint
        COMMAND ${CLANG_TIDY}
            ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c
            ${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h
            --
            -I${CMAKE_CURRENT_SOURCE_DIR}/inc
            -I${CMAKE_CURRENT_SOURCE_DIR}/mygetopt
            ${CURSES_INCLUDE_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT "Running clang-tidy"
    )
endif()

# Testing support
enable_testing()

# Basic smoke test
add_test(NAME smoke_test_bigman
    COMMAND bigman --help
)
set_tests_properties(smoke_test_bigman PROPERTIES
    PASS_REGULAR_EXPRESSION "Usage:"
)

# Test all variants
add_test(NAME smoke_test_hugeman COMMAND hugeman --help)
add_test(NAME smoke_test_smallman COMMAND smallman --help)
add_test(NAME smoke_test_squareman COMMAND squareman --help)

# Sanitizer build option
option(ENABLE_ASAN "Enable AddressSanitizer" OFF)
if(ENABLE_ASAN)
    if(CMAKE_C_COMPILER_ID MATCHES "GNU|Clang")
        add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer -g)
        add_link_options(-fsanitize=address,undefined)
        message(STATUS "AddressSanitizer enabled")
    else()
        message(WARNING "AddressSanitizer requested but not supported by compiler")
    endif()
endif()

# Debug/Release configurations
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Build type" FORCE)
endif()

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C compiler: ${CMAKE_C_COMPILER_ID} ${CMAKE_C_COMPILER_VERSION}")
message(STATUS "Curses library: ${CURSES_LIBRARIES}")

# Create symlinks to asset directories in build folder for development
add_custom_target(data_symlinks ALL
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets/tiles ${CMAKE_BINARY_DIR}/chr
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets/mazes ${CMAKE_BINARY_DIR}/lvl
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets/sprites ${CMAKE_BINARY_DIR}/spr
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets/sounds ${CMAKE_BINARY_DIR}/sfx
    COMMENT "Creating symlinks to asset directories"
)
