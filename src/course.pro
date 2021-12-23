QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    bary.cpp \
    geometry_shader.cpp \
    main.cpp \
    mainwindow.cpp \
    manager.cpp \
    model.cpp \
    pixel_shader.cpp \
    primitive.cpp \
    raythread.cpp \
    raytraycing.cpp \
    texture.cpp \
    vertex_shader.cpp

HEADERS += \
    OBJ_Loader.h \
    bary.h \
    camera.h \
    color_shader.h \
    geometry_shader.h \
    light.h \
    mainwindow.h \
    mat.h \
    model.h \
    primitive.h \
    raythread.h \
    scene_manager.h \
    shaders.h \
    texture.h \
    vec3.h \
    vec4.h \
    vertex.h \
    vertex_shader.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
