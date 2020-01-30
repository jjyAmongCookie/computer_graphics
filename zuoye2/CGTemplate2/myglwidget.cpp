/*###################################################
##  姓名：江金昱
##  文件说明：实现myglwidget.h
##  包括设置OpenGL渲染环境，设置视口、投影，设置窗口大小改变时的动作
##  渲染OpenGL场景，设置每次部件更新的时候调用的函数
#####################################################*/

#include "myglwidget.h"

// 机器人绕着原点转动的幅度
float angle = 0.0f;

float legAngle[2] = { 0.0f,0.0f };
float armAngle[2] = { 0.0f,0.0f };

#include<iostream>
#include<wingdi.h>
#include "robot.h"
#include"objloader.h"
#include<Windows.h>
using namespace std;


static objModel objmodel = objModel();
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
}


/*###################################################
##  函数: ~MyGLWidget
##  函数描述： ~MyGLWidget类的析构函数，删除timer
##  参数描述： 无
#####################################################*/
MyGLWidget::~MyGLWidget()
{
    delete this->timer;
}


/*###################################################
##  函数: initializeGL
##  函数描述： 初始化绘图参数，如视窗大小、背景色等，启用深度测试，加载纹理图片，绑定纹理ID，加载obj模型
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
    glViewport(0, 0, width(), height());
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glDisable(GL_DEPTH_TEST);
    // 加载纹理图片到内存中
    image.Loadbitmap("D:/computer_graphics/project/test_function/baymax/EyesWhite2.bmp");
    // 生成纹理ID并绑定glTexutre2D
    makeImage(&image);
    // 加载obj模型的顶点数据
    objmodel.loadObjModel("D:/computer_graphics/project/test_function/baymax/Bigmax_White_OBJ2.obj");
}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
             设置照相机，视景体，初始化加载obj模型的类，调用画机器人的模型
             实现机器人绕着原点运动
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
    // 启动深度测试
    glEnable(GL_DEPTH_TEST);
    //设置清理色为黑色
    //清理颜色/深度缓存
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // 设置光照
    GLfloat light_position[] = {0.0f, 100.0f, 100.0f, 1.0f}; //光源的位置在世界坐标系圆心，齐次坐标形式
    GLfloat light_ambient[]   = {1.0f, 1.0f, 1.0f, 1.0f}; //RGBA模式的环境光，为0
    GLfloat light_diffuse[]   = {1.0f, 1.0f, 1.0f, 1.0f}; //RGBA模式的漫反射光，全白光
    GLfloat light_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};  //RGBA模式下的镜面光 ，全白光
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);
    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

    //开启灯光
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);

    // 清理obj文件的顶点坐标数量，纹理坐标数量，法线坐标数量为0
    vnum=0;
    cnum=0;
    nnum=0;

    // 设置投影矩阵
    glMatrixMode(GL_PROJECTION);
    // 将当前矩阵设置为单位矩阵
    glLoadIdentity();
    //正交平行的视景体
    float w=100.0f;
    glOrtho(-w,w,-w,w,-w,w);
    gluLookAt(0,20,40,0,0,0,0,1,0);

    // 设置模型视图矩阵
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // 每次更新视图，递增旋转角度
    float epsilon=0.5f;
    angle = angle + epsilon;
    // 旋转一圈后角度变回为0
    if (angle >= 360.0f)
        angle = 0.0f;
    glPushMatrix();
    glLoadIdentity();

    //----------先平移后旋转，表现为代码上为先写glRotatef，再写glTranslatef----------
    // 实现绕着原点旋转
    glRotatef(angle, 0.0f, 1.0f, 0.0f);//绕其y轴旋转机器人
    glTranslatef(-30.f,0.0f,0.0f);
    // 绘制机器人
    DrawRobot(0.0f, 0.0f, 0.0f,objmodel,epsilon);
    glPopMatrix();
    glFlush();
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

