#-------------------------------------------------
#
# Project created by QtCreator 2019-04-12T17:39:06
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = designer
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17

LIBS += -lassimp

SOURCES += \
        src/main.cpp \
        src/mainwindow.cpp \
        src/viewportwidget.cpp \
    src/shader.cpp \
    src/texturewidget.cpp \
    src/texture.cpp \
    src/node.cpp \
    src/nodeeditor.cpp \
    src/nodeproperty.cpp \
    src/propertyeditor.cpp \
    src/mesh.cpp

HEADERS += \
        include/mainwindow.h \
        include/viewportwidget.h \
    include/transform.h \
    include/texture.h \
    include/shader.h \
    include/object.h \
    include/mesh.h \
    include/material.h \
    include/light.h \
    include/camera.h \
    include/texturewidget.h \
    include/stb_image.h \
    include/node.h \
    include/nodeeditor.h \
    include/propertyeditor.h \
    include/nodeproperty.h \
    include/color.h

INCLUDEPATH += include/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES = shaders/* compute/*

copydata.commands = $(COPY_DIR) $$PWD/shaders $$PWD/compute $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
