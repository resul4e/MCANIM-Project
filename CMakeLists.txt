cmake_minimum_required(VERSION 3.8)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# Include the INSTALL target in the build step
set(CMAKE_VS_INCLUDE_INSTALL_TO_DEFAULT_BUILD 1)

# Set executable directory
set(BINARY_DIR ${CMAKE_SOURCE_DIR}/Binary)

set(AnimationProject "SkeletalAnimation")

project(${AnimationProject} LANGUAGES CXX C VERSION 0.1)
set(CMAKE_CXX_STANDARD 17)

# Set our include folders as the place to look for library includes
include_directories(${CMAKE_SOURCE_DIR}/ThirdParty/glad/include/)
include_directories(${CMAKE_SOURCE_DIR}/ThirdParty/GLFW/include/)
include_directories(${CMAKE_SOURCE_DIR}/ThirdParty/assimp/include/)
include_directories(${CMAKE_SOURCE_DIR}/ThirdParty/glm/include/)

# Add source subdirectory which contains the source files
add_subdirectory(Source)

add_executable(${AnimationProject}
    ${BASE_SOURCES}
    ${ANIMATION_BASE_SOURCES}
    ${ANIMATION_SKINNING_SOURCES}
    ${RENDERING_SOURCES}
    ${IMGUI_SOURCES}
)

source_group(Source FILES ${BASE_SOURCES})
source_group(Source\\Animation FILES ${ANIMATION_BASE_SOURCES})
source_group(Source\\Animation\\Skinning FILES ${ANIMATION_SKINNING_SOURCES})
source_group(Source\\Rendering FILES ${RENDERING_SOURCES})
source_group(Source\\ImGui FILES ${IMGUI_SOURCES})

target_link_libraries(${AnimationProject} ${CMAKE_SOURCE_DIR}/ThirdParty/GLFW/lib/glfw3.lib)
target_link_libraries(${AnimationProject} ${CMAKE_SOURCE_DIR}/ThirdParty/assimp/lib/assimp.lib)

set_target_properties(${AnimationProject} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${BINARY_DIR}/$<CONFIG>/)

# Install binaries
install(FILES ThirdParty/assimp/bin/assimp.dll DESTINATION ${BINARY_DIR}/$<CONFIG>/)
