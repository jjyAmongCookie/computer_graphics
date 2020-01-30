#include"myfunction.h"
#include <math.h>


MyGLWidget::MyGLWidget(QWidget *parent)
	:QOpenGLWidget(parent),
    scene_id(0)
{
}

MyGLWidget::~MyGLWidget()
{

}

void MyGLWidget::initializeGL()
{
	glViewport(0, 0, width(), height());
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glDisable(GL_DEPTH_TEST);
}

void MyGLWidget::paintGL()
{
	if (scene_id==0) {
		scene_0();
	}
	else {
		scene_1();
	}
}

void MyGLWidget::resizeGL(int width, int height)
{
	glViewport(0, 0, width, height);
	update();
}

void MyGLWidget::keyPressEvent(QKeyEvent *e) {
	if (e->key() == Qt::Key_0) {
		scene_id = 0;
		update();
	}
	else if (e->key() == Qt::Key_1) {
		scene_id = 1;
		update();
	}
}




void MyGLWidget::scene_0()
{
    // 清除颜色缓冲区
    glClear(GL_COLOR_BUFFER_BIT);
    // 投影模式
    glMatrixMode(GL_PROJECTION);
    // 将当前矩阵设置为单位矩阵
    glLoadIdentity();

    //正交平行的视景体
	glOrtho(0.0f, 100.0f, 0.0f, 100.0f, -1000.0f, 1000.0f);

    // 对模型视景的操作
	glMatrixMode(GL_MODELVIEW);
    // 将当前矩阵设置为单位矩阵
	glLoadIdentity();
    // 沿着x，y，z方向平移
    glTranslatef(50.0f, 50.0f, 0.0f);
//    myTranslatef(50.0f,50.0f,0.0f);

	//draw a triangle
    // 保存当前模型视图矩阵
    glPushMatrix();
    // 设置rgb三个通道的颜色
	glColor3f(0.839f, 0.153f, 0.157f);
    // 向三个坐标方向移动
//    glTranslatef(-20.0f, -10.0f, 0.0f);
    myTranslatef(-20.0f, -10.0f, 0.0f);

    // 旋转度数为45°，向x轴逆时针方向旋转，向z轴逆时针旋转
    // 用右手定则判定，手握住坐标轴，四指弯曲的方向是逆时针方向
//    glRotatef(45.0f, 1.0f, 0.0f, 1.0f);
//    myRotatef(45.0f, 1.0f, 0.0f, 1.0f);
    quaternionRotate(45.0f,1.0f,0.0f,1.0f);


	glTranslatef(-50.0f, -30.0f, 0.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(10.0f, 10.0f);
	glVertex2f(50.0f, 50.0f);
	glVertex2f(80.0f, 10.0f);
	glEnd();
    glPopMatrix();

	//draw a quad
	glPushMatrix();
	glColor3f(0.122f, 0.467f, 0.706f);
	glTranslatef(20.0f, 20.0f, 0.0f);
	glRotatef(30.0f, 1.0f, 1.0f, 1.0f);
	glBegin(GL_LINE_LOOP);
	glVertex2f(-20.0f, -20.0f);
	glVertex2f(20.0f, -20.0f);
	glVertex2f(20.0f, 20.0f);
	glVertex2f(-20.0f, 20.0f);
	glEnd();
	glPopMatrix();
}



void MyGLWidget::scene_1()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0f, 5000.0f, 0.0f, 5000.0f, -1000.0f, 1000.0f);

    GLfloat w_ratio=5000.0f/100.0f;
    GLfloat h_ratio=5000.0f/100.0f;

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    myTranslatef(50.0f*w_ratio,50.0f*h_ratio,0.0f);

    glPushMatrix();
    glColor3f(0.839f,0.153f,0.157f);
    myTranslatef(-20.0f*w_ratio,-10.0f*h_ratio,0.0f);
    myRotatef(45.0f,1.0f,0.0f,1.0f);

    myTranslatef(-50.0f*w_ratio,-30.0f*h_ratio,0.0f);
    // 设置点的大小为7个像素
    //开始画点
    glBegin(GL_POINTS);
    //使用Bresenham算法画线
    drawline(10*w_ratio,10*h_ratio,50*w_ratio,50*h_ratio);
    drawline(50*w_ratio,50*h_ratio,80*w_ratio,10*h_ratio);
    drawline(80*w_ratio,10*h_ratio,10*w_ratio,10*h_ratio);
    glEnd();
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.122f, 0.467f, 0.706f);
    myTranslatef(20.0f*w_ratio, 20.0f*h_ratio, 0.0f);
    myRotatef(30.0f, 1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    drawline(-20*w_ratio,-20*h_ratio,20*w_ratio,-20*h_ratio);
    drawline(20*w_ratio,-20*h_ratio,20*w_ratio,20*h_ratio);
    drawline(20*w_ratio,20*h_ratio,-20*w_ratio,20*h_ratio);
    drawline(-20*w_ratio,20*h_ratio,-20*w_ratio,-20*h_ratio);
    glEnd();
    glPopMatrix();

}





