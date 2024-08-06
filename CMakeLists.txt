# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Project name
project(Mycache)

# Set the output directory for the shared library
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the source file for the shared library
set(SOURCES
    core.cpp
)

# Add the header file directory (optional, helps with IDEs)
set(HEADERS
    cache/core.h
)

# Create the shared library
add_library(Mycache SHARED ${SOURCES})

# Specify include directories
target_include_directories(Mycache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cache)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    set_target_properties(Mycache PROPERTIES
        WINDOWS_EXPORT_ALL_SYMBOLS ON
        PREFIX ""
        SUFFIX ".dll"
    )
else()
    # Unix-specific settings
    set_target_properties(Mycache PROPERTIES
        PREFIX "lib"
        SUFFIX ".so"
    )
endif()

# Install the shared library and headers (optional)
install(TARGETS Mycache
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS} DESTINATION include/cache)

# Print a message indicating successful configuration
message(STATUS "Configured Cache library")
