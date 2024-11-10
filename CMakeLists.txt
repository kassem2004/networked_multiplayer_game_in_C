# Minimum CMake version and project name
cmake_minimum_required(VERSION 3.10)
project(BattleshipGame C)

# Specify the C standard
set(CMAKE_C_STANDARD 11)

# Define include directory for headers
include_directories(${PROJECT_SOURCE_DIR}/include)

# Define source files for each executable
set(SERVER_SRC server_main.c server.c game_logic.c)
set(CLIENT_SRC client_main.c client.c game_logic.c)

# Add the 'server' executable
add_executable(server ${SERVER_SRC})

# Add the 'client' executable
add_executable(client ${CLIENT_SRC})
