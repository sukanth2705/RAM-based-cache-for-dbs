# CMake minimum version
cmake_minimum_required(VERSION 3.10)

# Project name
project(Mycache)

# Set the output directory for the shared library
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add the source files for the shared library
set(SOURCES
    core.cpp
    server.cpp
)

# Add the header file directory (optional, helps with IDEs)
set(HEADERS
    cache/core.h
    cache/server.h
)

# Create the shared library
add_library(Mycache SHARED ${SOURCES})

# Specify include directories for the library
target_include_directories(Mycache PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cache)

# Add the main source file
add_executable(my_own_redis service.cpp)

# Specify include directories for the executable
target_include_directories(my_own_redis PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/cache)

# Link the executable with the Mycache library
target_link_libraries(my_own_redis PRIVATE Mycache)

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

# Find and link the threading library
find_package(Threads REQUIRED)
target_link_libraries(my_own_redis PRIVATE ${CMAKE_THREAD_LIBS_INIT})
target_link_libraries(Mycache PRIVATE ${CMAKE_THREAD_LIBS_INIT})

# Install the shared library and headers
install(TARGETS Mycache
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(FILES ${HEADERS} DESTINATION include/cache)

# Print a message indicating successful configuration
message(STATUS "Configured Cache library and application")

# Install the executable
install(TARGETS my_own_redis DESTINATION /usr/local/bin)

# Install systemd service file
install(FILES my-own-redis.service DESTINATION /etc/systemd/system)
