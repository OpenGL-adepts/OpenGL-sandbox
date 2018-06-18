cmake_minimum_required(VERSION 3.0)
project(OpenGL-sandbox)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(OpenGL-sandbox/Vendor/glfw)

option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(OpenGL-sandbox/Vendor/assimp)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /std:c++17")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++17")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(OpenGL-sandbox/Headers/
                    OpenGL-sandbox/Vendor/assimp/include/
                    OpenGL-sandbox/Vendor/glad/include/
                    OpenGL-sandbox/Vendor/glfw/include/
                    OpenGL-sandbox/Vendor/glm/
                    OpenGL-sandbox/Vendor/stb/
					OpenGL-sandbox/Vendor/imgui/
					OpenGL-sandbox/Vendor/json/)

file(GLOB VENDORS_SOURCES OpenGL-sandbox/Vendor/glad/src/glad.c
						  OpenGL-sandbox/Vendor/imgui/*.cpp)
						  
file(GLOB PROJECT_HEADERS OpenGL-sandbox/Headers/*.hpp)
file(GLOB PROJECT_SOURCES OpenGL-sandbox/Sources/*.cpp
						  OpenGL-sandbox/Sources/effects/*.cpp)
file(GLOB PROJECT_SHADERS OpenGL-sandbox/Shaders/*.comp
                          OpenGL-sandbox/Shaders/*.frag
                          OpenGL-sandbox/Shaders/*.geom
                          OpenGL-sandbox/Shaders/*.vert)
file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("Headers" FILES ${PROJECT_HEADERS})
source_group("Shaders" FILES ${PROJECT_SHADERS})
source_group("Sources" FILES ${PROJECT_SOURCES})
source_group("Vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME} assimp glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
                      )
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
