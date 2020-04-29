cmake_minimum_required(VERSION 3.8)

set(AnimationProject "SkeletalAnimation")

project(${AnimationProject} LANGUAGES CXX C VERSION 0.1)

# Set our include folders as the place to look for library includes
include_directories(${CMAKE_SOURCE_DIR}/ThirdParty/glad/include/)
include_directories(${CMAKE_SOURCE_DIR}/ThirdParty/GLFW/include/)

# Add source subdirectory which contains the source files
add_subdirectory(Source)

add_executable(${AnimationProject}
    ${BASE_SOURCES}
)

source_group(Source FILES ${BASE_SOURCES})

target_link_libraries(${AnimationProject} ${CMAKE_SOURCE_DIR}/ThirdParty/GLFW/lib/glfw3.lib)
