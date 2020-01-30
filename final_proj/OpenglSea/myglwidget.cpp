#include "myglwidget.h"
#include "SceneManager.h"

extern SceneManager scene = SceneManager();
extern Float3 cameraPos = Float3(0, 200, 0);

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
##  函数描述： 初始化绘图参数，如视窗大小、背景色等
##  参数描述： 无
#####################################################*/
void MyGLWidget::initializeGL()
{
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glEnable(GL_DEPTH_TEST); //开启深度，实现遮挡关系
    glShadeModel(GL_SMOOTH); //设置平滑着色
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // 设置颜色混合模式

    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    scene.Init();

}


/*###################################################
##  函数: paintGL
##  函数描述： 绘图函数，实现图形绘制，会被update()函数调用
##  参数描述： 无
#####################################################*/
void MyGLWidget::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)1280 / (GLfloat)720, 1.0, 100000.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, 1, cameraPos.y, 0, 0, 1, 0); //从视点看X轴正方向,y轴方向(0,1,0)是上方向

    glEnable(GL_TEXTURE_2D);
    scene.Update();
    glDisable(GL_TEXTURE_2D);
}


/*###################################################
##  函数: resizeGL
##  函数描述： 当窗口大小改变时调整视窗尺寸
##  参数描述： 无
#####################################################*/
void MyGLWidget::resizeGL(int width, int height)
{
    glViewport (0, 0, (GLsizei) width, (GLsizei) height);
    glMatrixMode (GL_PROJECTION);
    glLoadIdentity ();
    gluPerspective(60.0, (GLfloat) width/(GLfloat) height, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(cameraPos.x, cameraPos.y, cameraPos.z, 1, 200, 0, 0, 1, 0); //从视点看原点,y轴方向(0,1,0)是上方向
}

