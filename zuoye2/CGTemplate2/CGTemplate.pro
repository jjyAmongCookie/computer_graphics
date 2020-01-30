QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += \
	Glu32.lib \
        OpenGL32.lib \
        -LD:/computer_graphics/project/zuoyeer/CGTemplate2 -lglew32 \
        User32.lib \
        Gdi32.lib \
        -LD:\computer_graphics\project\zuoyeer\CGTemplate2 -lglut \
        -LD:\computer_graphics\project\zuoyeer\CGTemplate2 -lglut32 \

SOURCES += \
    BMPLoader.cpp \
    main.cpp \
    myglwidget.cpp \


HEADERS += \
    BMPLoader.h \
    myglwidget.h \
    objloader.h \
    robot.h
