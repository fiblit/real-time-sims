cmake_minimum_required(VERSION 3.5)
project(hw4_5611)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(Libraries/glfw)

#[[
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(Libraries/assimp)
]]

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++14")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(Headers/
                    #[[Libraries/assimp/include/]]
                    Libraries/glad/include/
                    Libraries/glfw/include/
                    Libraries/glm/
                    Libraries/stb/)

file(GLOB LIBRARY_SOURCES Libraries/glad/src/glad.c)
file(GLOB PROJECT_HEADERS Headers/*.hpp)
file(GLOB PROJECT_SOURCES Sources/*.cpp)
file(GLOB PROJECT_SHADERS Shaders/*.comp
                          Shaders/*.frag
                          Shaders/*.geom
                          Shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          README.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Libraries" FILES ${LIBRARY_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\"
                -DDEBUG)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${LIBRARY_SOURCES})

target_link_libraries(${PROJECT_NAME} #[[assimp]] glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
