set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(BASE_SOURCES
    ${DIR}/main.cpp
    ${DIR}/Window.h
    ${DIR}/Window.cpp
    ${DIR}/Renderer.h
    ${DIR}/Renderer.cpp
    ${DIR}/RigLoader.h
    ${DIR}/RigLoader.cpp
    ${DIR}/Shader.h
    ${DIR}/Shader.cpp
    ${DIR}/Vendor/stbImage/stb_image.h
    ${DIR}/Vendor/stbImage/stb_image.cpp
    ${DIR}/Texture.h
    ${DIR}/Texture.cpp
    ${DIR}/Joint.h
    ${DIR}/Joint.cpp
    ${DIR}/Rig.h
    ${DIR}/Rig.cpp
    ${DIR}/glad.c
    PARENT_SCOPE
)
