set(DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(BASE_SOURCES
    ${DIR}/main.cpp
    ${DIR}/Window.h
    ${DIR}/Window.cpp
    ${DIR}/Renderer.h
    ${DIR}/Renderer.cpp
    ${DIR}/RigLoader.h
    ${DIR}/RigLoader.cpp
    ${DIR}/Joint.h
    ${DIR}/Joint.cpp
    ${DIR}/Rig.h
    ${DIR}/Rig.cpp
    ${DIR}/glad.c
    PARENT_SCOPE
)
