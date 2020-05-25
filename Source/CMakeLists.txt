set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(BASE_SOURCES
    ${DIR}/main.cpp
    ${DIR}/Window.h
    ${DIR}/Window.cpp
    ${DIR}/RigLoader.h
    ${DIR}/RigLoader.cpp
    ${DIR}/AnimationLoader.h
    ${DIR}/AnimationLoader.cpp
    ${DIR}/ModelLoader.h
    ${DIR}/ModelLoader.cpp
    PARENT_SCOPE
)

set(ANIMATION_BASE_SOURCES
    ${DIR}/AnimationClip.h
    ${DIR}/AnimationClip.cpp
    ${DIR}/Channel.h
    ${DIR}/Channel.cpp
    ${DIR}/KeyFrame.h
    ${DIR}/KeyFrame.cpp
    ${DIR}/Joint.h
    ${DIR}/Joint.cpp
    ${DIR}/Rig.h
    ${DIR}/Rig.cpp
    PARENT_SCOPE
)

set(RENDERING_SOURCES
    ${DIR}/Renderer.h
    ${DIR}/Renderer.cpp
    ${DIR}/Model.h
    ${DIR}/Model.cpp
    ${DIR}/Shader.h
    ${DIR}/Shader.cpp
    ${DIR}/stb_image.h
    ${DIR}/stb_image.cpp
    ${DIR}/Texture.h
    ${DIR}/Texture.cpp
    ${DIR}/glad.c
    PARENT_SCOPE
)
