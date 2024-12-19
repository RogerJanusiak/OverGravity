cmake_minimum_required(VERSION 3.22)

if(WIN32)
project(Hyper_Gravity)

set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT_NAME} src/Player.cpp
        src/Texture.cpp
        src/Platform.cpp
        includes/Robor.h
        src/Entity.cpp
        includes/Entity.h
        src/Bullet.cpp
        includes/Bullet.h
        includes/Spawn.h
        includes/Bullet.h
        src/main.cpp
        src/UI.cpp
        includes/UI.h
        src/main.cpp
        includes/State.h
        src/Sound.cpp
        includes/Sound.h
        src/Roborto.cpp
        src/Roborto.cpp
        includes/Roborto.h
        src/Animations.cpp
        includes/Animations.h
        src/Weapon.cpp
        includes/Weapon.h
        src/Menu.cpp
        includes/Menu.h
        src/Enemy.cpp
        includes/Enemy.h
        includes/Robro.h
        includes/Romo.h
        includes/Roo.h)

# --- SDL2 SETUP ---
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/CMAKE/cmakepc )
set(SDL2_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_TTF_PATH "SDL2/x86_64-w64-mingw32")
set(SDL2_MIXER_PATH "SDL2/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_mixer REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
        ${SDL2_IMAGE_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR})

set(SDL_LIBRARIES ${SDL_LIBRARIES} SDL2main SDL2_image-static SDL2-static SDL2_ttf-static SDL2_mixer-static)

target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
        ${SDL2_IMAGE_LIBRARIES}
        ${SDL2_TTF_LIBRARIES} ${SDL2_MIXER_LIBRARIES} -static  )
# -fsanitize=address
# Specify the resource folder
set(RESOURCE_SOURCE_DIR "${CMAKE_SOURCE_DIR}/resources")

# Copy resources from source to build directory
file(COPY ${RESOURCE_SOURCE_DIR} DESTINATION ${CMAKE_BINARY_DIR}/)
endif()

if(APPLE)
    cmake_minimum_required(VERSION 3.22)

    Message("")
    Message( STATUS "CMAKE_BINARY_DIR: " ${CMAKE_BINARY_DIR} )
    Message( STATUS "CMAKE_SOURCE_DIR: " ${CMAKE_SOURCE_DIR} )
    Message( STATUS "CMAKE_COMMAND: " ${CMAKE_COMMAND} )
    Message( STATUS "CMAKE_VERSION: " ${CMAKE_VERSION} )
    Message( STATUS "CMAKE_C_COMPILER: " ${CMAKE_C_COMPILER} )
    Message( STATUS "CMAKE_PROJECT_NAME: " ${CMAKE_PROJECT_NAME} )
    Message( STATUS "CMAKE_FINDCMAKE: " ${CMAKE_SOURCE_DIR}/CMAKE )

    project(Hyper_Gravity)

    # ------- Set Vars ------- #

    set(CMAKE_CXX_STANDARD 23)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMAKE")
    set(SOURCE_FILES    src/Entities/Player.cpp
            src/TEng/Texture.cpp
            src/HEng/Platform.cpp
            includes/Entities/Robor.h
            src/Entities/Entity.cpp
            includes/Entities/Entity.h
            src/Entities/Bullet.cpp
            includes/Entities/Bullet.h
            includes/HEng/Spawn.h
            includes/Entities/Bullet.h
            src/main.cpp
            includes/HEng/State.h
            src/TEng/Sound.cpp
            includes/TEng/Sound.h
            src/Entities/Roborto.cpp
            includes/Entities/Roborto.h
            src/Utils/Animations.cpp
            includes/Utils/Animations.h
            src/HEng/Weapon.cpp
            includes/HEng/Weapon.h
            src/TEng/Menu.cpp
            includes/TEng/Menu.h
            src/Entities/Enemy.cpp
            includes/Entities/Enemy.h
            includes/Entities/Robro.h
            includes/Entities/Romo.h
            includes/Entities/Roo.h
            src/HEng/MainMenu.cpp
            includes/HEng/MainMenu.h
            src/Utils/Input.cpp
            includes/Utils/Input.h
            src/HEng/Run.cpp
            includes/HEng/Run.h
            src/HEng/Wave.cpp
            includes/HEng/Wave.h
			src/HEng/Level.cpp
			includes/HEng/Level.h
            includes/HEng/WaveController.h
            src/HEng/WaveController.cpp)

    Message("")
    Message( STATUS "SOURCE entry point : " ${SOURCE_FILES} )
    Message("")

    # ------- End Set Vars --- #

    # ------- Executable ----- #

    ADD_EXECUTABLE(Hyper_Gravity ${SOURCE_FILES})

    # ------- End Executable - #

    # ------- Finds ---------- #

    FIND_PACKAGE(SDL2 REQUIRED)
    Message("")
    Message( STATUS "FINDING SDL2" )
    IF (${SDL2_FOUND})
        Message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
        Message( STATUS "SDL2_INCLUDE_DIR:" ${SDL2_INCLUDE_DIR})
        Message( STATUS "SDL2_LIBRARY: " ${SDL2_LIBRARY})
    ELSE()
        Message( STATUS "SDL2_FOUND: " ${SDL2_FOUND})
        Message( FATAL_ERROR "SDL2 NOT FOUND" )
    ENDIF()

    FIND_PACKAGE(SDL2_IMAGE)
    Message("")
    Message( STATUS "FINDING SDL2_IMAGE" )
    Message( STATUS "SDL2_IMAGE_FOUND: " ${SDL2_IMAGE_FOUND})
    IF (${SDL2_IMAGE_FOUND})
        Message( STATUS "SDL2_IMAGE_INCLUDE_DIR: " ${SDL2_IMAGE_INCLUDE_DIR})
        Message( STATUS "SDL2_IMAGE_LIBRARY: " ${SDL2_IMAGE_LIBRARY})
    ELSE()
        Message( FATAL_ERROR "SDL2_IMAGE NOT FOUND" )
    ENDIF()

    FIND_PACKAGE(SDL2TTF)
    Message("")
    Message( STATUS "FINDING SDL2TTF" )
    Message( STATUS "SDL2TTF_FOUND: " ${SDL2TTF_FOUND} )
    IF (${SDL2TTF_FOUND})
        Message( STATUS "SDL2TTF_INCLUDE_DIR: " ${SDL2TTF_INCLUDE_DIR})
        Message( STATUS "SDL2TTF_LIBRARY: " ${SDL2TTF_LIBRARY})
    ELSE()
        Message( FATAL_ERROR "SDL2TTF NOT FOUND" )
    ENDIF()
    Message("")

    FIND_PACKAGE(SDL2Mixer)

    Message( STATUS "FINDING SDL2Mixer" )
    Message( STATUS "SDL2Mixer_FOUND: " ${SDL2Mixer_FOUND} )

    Message("")

    file(COPY resources DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})

    # ------- End Finds ------ #

    # ------- Inc & Link ---- #

    INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2Mixer_INCLUDE_DIR})
    TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARY} ${SDL2TTF_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2Mixer_LIBRARY} )

    # ------- End ----------- #



endif ()