cmake_minimum_required(VERSION 3.22)
project(PROJECT_NAME)

set(CMAKE_CXX_STANDARD 14)

# Must set the path to the main.cpp, for example: scripts/main.cpp if it is inside a folder
add_executable(${PROJECT_NAME} src/main.cpp
        src/Player.cpp
        src/Texture.cpp
        src/Platform.cpp
        src/Robor.cpp
        includes/Robor.h
        src/Entity.cpp
        includes/Entity.h
        src/Bullet.cpp
        includes/Bullet.h
        src/Spawn.cpp
        includes/Spawn.h
        includes/Bullet.h)

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/modules)
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "SDL2/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR})
set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2main SDL2_image-static SDL2-static SDL2_ttf-static)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES} -static)

