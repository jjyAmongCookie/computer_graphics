/*###################################################
##  姓名：江金昱
##  文件说明：实现myglwidget.h
##  配置OpenGL渲染环境，包括视口、投影等设置
##  编译链接shader文件
##  应用shader以及VAO参与渲染流程
#####################################################*/

#include "datainfo.h"
#include "GL/glew.h"
#include "myglwidget.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <QtCore/qdir.h>
#include <qopenglextrafunctions.h>

using namespace std;

extern Float3 cameraPos = Float3(0, 0, 0); //定义摄像机坐标
GLfloat vex[][3] = {{0, 20, 0},{0, 40, 0}, {0, 60, 0}, {0, 80, 0}, {0, 100, 0},
                    {0, -20, 0},{0, -40, 0}, {0, -60, 0}, {0, -80, 0}, {0, -100, 0}, {0, 0, 0}};  //定义点坐标

/*###################################################
##  函数: MyGLWidget
##  函数描述： MyGLWidget类的构造函数，实例化定时器timer
##  参数描述：
##  parent: MyGLWidget的父对象
#####################################################*/

MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent)
{
	timer = new QTimer(this); // 实例化一个定时器
	timer->start(16); // 时间间隔设置为16ms，可以根据需要调整
	connect(timer, SIGNAL(timeout()), this, SLOT(update())); // 连接update()函数，每16ms触发一次update()函数进行重新绘图

    this->resize(1280, 720);

    timeCount = 0;
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
	delete this->timer;

    //释放shader
    glDetachShader(ID, vertexShader);
    glDetachShader(ID, fragmentShader);
    glDeleteProgram(ID);
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
    initializeOpenGLFunctions();

    glViewport(0, 0, width(), height());
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glEnable(GL_DEPTH_TEST); //开启深度，实现遮挡关系
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清理缓存

    glEnable(GL_POINT_SPRITE_ARB);  //开启点精灵
    glTexEnvi(GL_POINT_SPRITE_ARB, GL_COORD_REPLACE_ARB, GL_TRUE);  //设置纹理替换模式，为没有纹理单元指定点精灵的纹理坐标替换模式
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE_NV); //开启点在顶点着色器中设置大小

    //加载编译链接Shader文件
    InitShader("VertexShader.vert", "FragmentShader.frag");
    //初始化Vbo,Vao功能
    InitVboVao();
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //设置投影变换
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)1280 / (GLfloat)720, 1.0, 100000.0);

    //设置摄像机位置朝向
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, 1, cameraPos.y, 0, 0, 1, 0); //从视点看X轴正方向,y轴方向(0,1,0)是上方向

    //调用绘制
    Draw();
}


/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

/*###################################################
##  函数: InitShader
##  函数描述： 初始化Shader,加载到内存，编译链接
##  参数描述：
##  vertexPath:顶点着色器基于项目的相对路径
##  fragmentPath:片元着色器基于项目的相对路径
#####################################################*/
void MyGLWidget::InitShader(const char *vertexPath, const char *fragmentPath)
{
     string vertexCode;
     string fragmentCode;
     ifstream vertexShaderFile;
     ifstream fragmentShaderFile;
     stringstream vertexStream;
     stringstream fragmentStream;
     const char *vertexShaderSource;
     const char *fragmentShaderSource;

     // 转化成绝对路径
     string vertexDir = QDir::currentPath().toStdString() + "/" + vertexPath;
     string fragmentDir = QDir::currentPath().toStdString() + "/" + fragmentPath;

     //加载顶点着色器代码
     vertexShaderFile.open(vertexDir);
     vertexStream << vertexShaderFile.rdbuf();
     vertexShaderFile.close();

     //加载片元着色器代码
     fragmentShaderFile.open(fragmentDir);
     fragmentStream << fragmentShaderFile.rdbuf();
     fragmentShaderFile.close();

     //字符转换
     vertexCode = vertexStream.str();
     fragmentCode = fragmentStream.str();
     vertexShaderSource = vertexCode.c_str();
     fragmentShaderSource = fragmentCode.c_str();

     //编译顶点着色器，在控制台输出LOG
     int success;
     char infoLog[512];
     vertexShader = glCreateShader(GL_VERTEX_SHADER);
     glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
     glCompileShader(vertexShader);
     glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
     if(!success)
     {
         glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
         cout << "ERROR::VERTEXSHADER::COMPILATION_FAILED\n" << infoLog << endl;
     }
     else
         cout << "VERTEXSHADER_COMPILATION_SUCCESS" << endl;

     //编译片元着色器，在控制台输出LOG
     fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
     glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
     glCompileShader(fragmentShader);
     glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
     if (!success)
     {
         glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
         cout << "ERROR::FRAGMENTSHADER::COMPILATION_FAILED\n" << infoLog << endl;
     }
     else
         cout << "FRAGMENTSHADER_COMPILATION_SUCCESS" << endl;

     //绑定并链接着色器
     ID = glCreateProgram();
     glAttachShader(ID, vertexShader);
     glAttachShader(ID, fragmentShader);
     glLinkProgram(ID);
     glGetProgramiv(ID, GL_LINK_STATUS, &success);
     if(!success)
     {
         glGetProgramInfoLog(ID, 512, NULL, infoLog);
         cout << "ERROR::LINKING_FAILED\n" << infoLog << endl;
     }
     else
         cout << "LINKING_SUCCESS" << endl;

     glDeleteShader(vertexShader);
     glDeleteShader(fragmentShader);
}

/*###################################################
##  函数: InitVboVao
##  函数描述： 初始化顶点缓冲区与顶点缓冲数组配置
##  参数描述：无
#####################################################*/
void MyGLWidget::InitVboVao()
{
    //生成并绑定相应功能指针
    glGenVertexArrays(1, &vaoId);
    glGenBuffers(1, &vboId);

    //绑定数组指针
    glBindVertexArray(vaoId);
    glBindBuffer(GL_ARRAY_BUFFER, vboId);
    //写入顶点数据
    glBufferData(GL_ARRAY_BUFFER, sizeof (vex), vex, GL_STATIC_DRAW);

    //设置顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    //解绑VAO
    glBindVertexArray(0);
}

/*###################################################
##  函数: Draw
##  函数描述： 使用shader,vao对点进行绘制
##  参数描述：无
#####################################################*/
void MyGLWidget::Draw()
{
    //设置shader参数
    glUseProgram(ID);
    glUniform1f(glGetUniformLocation(ID, "radius"), 15);                        //设置小球半径大小
    glUniform4f(glGetUniformLocation(ID, "ambient"), 0.2f, 0.2f, 0.2f, 1.0f);   //设置环境光
    glUniform4f(glGetUniformLocation(ID, "diffuse"), 0.6, 0.5, 0.1, 1.0);       //小球漫反射颜色
    glUniform4f(glGetUniformLocation(ID, "specL"), 0.5, 0.5, 0.5, 1.0);         //高光颜色
    glUniform1f(glGetUniformLocation(ID, "specPow"), 10);                       //高光渐变曲率
    glUniform1f(glGetUniformLocation(ID, "RoundDis"), 80);                      //设置小球绕轴旋转半径
    glUniform1f(glGetUniformLocation(ID, "time"), timeCount);                   //设置运行时间
    glUniform4f(glGetUniformLocation(ID, "ligthv"), 0, 0, -300, 1);             //灯在视图空间的坐标
    glUniform4f(glGetUniformLocation(ID, "ligthDiffuse"), 1.0, 1.0, 1.0, 1.0);  //灯光颜色

    //时间计数器累加
    timeCount++;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //开始绘制
    glPushMatrix();
    glTranslatef(300, 0, 0);

    glBindVertexArray(vaoId);
    glDrawArrays(GL_POINTS, 0, 12);
    glBindVertexArray(0);

    glPopMatrix();
}
