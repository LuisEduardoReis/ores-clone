cmake_minimum_required(VERSION 3.9)
project(OresClone)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${OresClone_SOURCE_DIR}/cmake")
set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

set(CMAKE_EXE_LINKER_FLAGS "-static-libgcc -static-libstdc++")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR})

add_executable(OresClone main.cpp Game.cpp Game.h Graphics/Graphics.cpp Graphics/Graphics.h Constants.h Util/Util.h Level/Level.cpp Level/Level.h Level/TileType.cpp Level/TileType.h Graphics/TextureRegion.h Util/Coord.h Input.cpp Input.h Util/Coord.cpp Entities/TileEntity.cpp Entities/TileEntity.h Graphics/Font.cpp Graphics/Font.h Level/TileScore.cpp Level/TileScore.h Entities/Entity.h Entities/Entity.cpp Entities/Resource.cpp Entities/Resource.h Level/LevelTileMethods.cpp)

target_link_libraries(OresClone
                        ${SDL2_LIBRARY}
                        ${SDL2_IMAGE_LIBRARIES})
file(COPY res DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})