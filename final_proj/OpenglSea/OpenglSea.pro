QT += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += console qt c++11

DEFINES += QT_DEPRECATED_WARNINGS

LIBS += \
	Glu32.lib \
	OpenGL32.lib

SOURCES += \
    BaseModel.cpp \
    FishEmitter.cpp \
    Model/FishModel.cpp \
    Model/ModelQuad.cpp \
    Model/waveplantsmodel.cpp \
    SceneManager.cpp \
    Tool/ImageReader.cpp \
    Tool/ObjReader.cpp \
    main.cpp \
    myglwidget.cpp

HEADERS += \
    BaseModel.h \
    FishEmitter.h \
    GL/glew.h \
    GL/glut.h \
    GL/glxew.h \
    GL/wglew.h \
    Model/FishModel.h \
    Model/ModelQuad.h \
    Model/waveplantsmodel.h \
    SceneManager.h \
    Tool/ImageReader.h \
    Tool/ObjReader.h \
    myglwidget.h
