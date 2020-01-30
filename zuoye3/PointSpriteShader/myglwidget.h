#ifndef MYGLWIDGET_H
#define MYGLWIDGET_H

#pragma once

#include <QtGui>
#include <QOpenGLWidget>
#include <QOpenGLFunctions_3_3_Core>

class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core {
    Q_OBJECT

public:
    MyGLWidget(QWidget *parent = nullptr);
    ~MyGLWidget();

protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

    void InitShader(const char *vertexPath, const char *fragmentPath);

    void InitVboVao();

    void Draw();

private:
    QTimer *timer;
    unsigned int ID;

    GLuint vertexShader;
    GLuint fragmentShader;
    GLuint vboId;
    GLuint vaoId;

    int timeCount;

};
#endif // MYGLWIDGET_H
