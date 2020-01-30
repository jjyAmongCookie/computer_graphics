#ifndef MYFUNCTION_H
#define MYFUNCTION_H
#include"myglwidget.h"

/* 函数：showMatrix
 * 函数描述：显示矩阵
 * 参数描述：
 * m：指向一个一维的大小为16的数组的指针
 */
void showMatrix(GLfloat* m){
    for(int i=0;i<16;i++){
        int row=i/4;
        int col=i%4;
        printf("%f",m[col*4+row]);
        (i+1)%4==0?printf("\n"):printf(" ");
    }

}


/* 函数：myMultiMatrix
 * 函数描述：实现将mat矩阵右乘变换矩阵T
 * 参数描述：
 * result: 结果矩阵
 * mat: 栈顶矩阵
 * T: 变换矩阵
 */
void myMultiMatrix(GLfloat* result,GLfloat* mat,GLfloat* T){
    for(int i=0;i<16;i++){
        int row=i%4;
        int col=i/4;

        result[i]=0;
        for(int j=0;j<4;j++){
            result[i]+=mat[row+j*4]*T[col*4+j];
        }
    }
}


/* 函数：myTranslatef
 * 函数描述：实现沿x,y,z方向平移
 * 参数描述：
 * x: 向x轴平移的距离
 * y: 向y轴平移的距离
 * z: 向z轴平移的距离
 */
void myTranslatef(GLfloat x,GLfloat y,GLfloat z){
    // 获取模型视图矩阵的值
    GLfloat mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX,mat);

    // 需要右乘的变换矩阵
    GLfloat T[16];
    for(int i=0;i<16;i++){
        T[i]=0;
    }
    T[12]=x;T[13]=y;T[14]=z;
    T[0]=1;T[5]=1;T[10]=1;T[15]=1;

    //计算结果矩阵
    GLfloat result[16];

    // 将T右乘栈顶矩阵
    myMultiMatrix(result,mat,T);

    //置入栈顶
    glLoadMatrixf(result);
}


//初始化矩阵为单位矩阵
void initializeMatrix(GLfloat* T){
    //初始化
    for(int i=0;i<16;i++){
        T[i]=0;
    }
    T[0]=1;T[5]=1;T[10]=1;T[15]=1;
}


/* 函数：quaternionRotate
 * 函数描述：使用矩阵乘法绕向量(x,y,z)进行旋转angle°
 */
void myRotatef(GLfloat angle,GLfloat x,GLfloat y, GLfloat z){
    // 获取模型视图矩阵的值
    GLfloat mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX,mat);

    // 需要右乘的变换矩阵
    GLfloat T[16];
    //结果矩阵
    GLfloat result[16];

    // 初始化矩阵为单位矩阵
    initializeMatrix(T);

    // 归一化
    GLfloat len=sqrt(x*x+y*y+z*z);
    x/=len;
    y/=len;
    z/=len;

    // 弧度
    GLfloat radian=angle*M_PI/180.0f;
    T[0]=x*x*(1-cos(radian))+cos(radian);
    T[1]=x*y*(1-cos(radian))+z*sin(radian);
    T[2]=x*z*(1-cos(radian))-y*sin(radian);

    T[4]=x*y*(1-cos(radian))-z*sin(radian);
    T[5]=y*y*(1-cos(radian))+cos(radian);
    T[6]=y*z*(1-cos(radian))+x*sin(radian);

    T[8]=x*z*(1-cos(radian))+y*sin(radian);
    T[9]=y*z*(1-cos(radian))-x*sin(radian);
    T[10]=z*z*(1-cos(radian))+cos(radian);
    // 矩阵乘法
    myMultiMatrix(result,mat,T);
    // 置入栈顶
    glLoadMatrixf(result);
}


/* 函数：quaternionRotate
 * 函数描述：使用四元数绕向量(x,y,z)进行旋转angle°
 */
void quaternionRotate(GLfloat angle,GLfloat x,GLfloat y,GLfloat z){
    // 获取模型视图矩阵的值
    GLfloat mat[16];
    glGetFloatv(GL_MODELVIEW_MATRIX,mat);

    //结果矩阵
    GLfloat result[16];

    // 归一化
    GLfloat len=sqrt(x*x+y*y+z*z);
    x/=len;
    y/=len;
    z/=len;

    GLfloat sine=sin(angle/2*M_PI/180);
    x*=sine;
    y*=sine;
    z*=sine;
    GLfloat w=cos(angle/2*M_PI/180);

    // 旋转矩阵
    GLfloat T[16]={
        1-2*y*y-2*z*z,2*(x*y+w*z),2*(x*z-w*y),0,
        2*(x*y-w*z),1-2*x*x-2*z*z,2*(y*z+w*x),0,
        2*(x*z+w*y),2*(y*z-w*x),1-2*x*x-2*y*y,0,
        0,0,0,1
    };

    myMultiMatrix(result,mat,T);
    glLoadMatrixf(result);
}

/* 函数：myTranslatef
 * 函数描述：实现在(x0,y0)与(x1,y1)间画一条直线
 * 使用了Bresenham算法
 */
void drawline(GLint x0, GLint y0, GLint x1, GLint y1)
{

    GLint x=x0;
    GLint y=y0;
    GLint dx=abs(x1-x0);
    GLint dy=abs(y1-y0);

    // s1是x递增/递减的值
    GLint s1;
    // s2是y递增/递减的值
    GLint s2;
    GLint change;
//  当直线往右边延申
    if(x1>x0){
        s1=1;
    }
    else{
        s1=-1;
    }

//  直线往上面还是下面延申
    if(y1>y0){
        s2=1;
    }
    else{
        s2=-1;
    }

//  判断斜率是否大于1
    if(dy>dx){
        //大于1则交换dx和dy
        GLint temp;
        temp=dx;
        dx=dy;
        dy=temp;
        change=1;
    }
    else{
        change=0;
    }

     GLint p=2*dy-dx;
     glVertex2i(x,y);
//   绘制点
     for(int i=1;i<=dx;i++){
         if(p>0){
             //p>0说明沿着y轴方向需要有变换

             //分斜率大于0还是小于0两种情况
             if(change==0){
                 y=y+s2;
             }
             else{
                 x=x+s1;
             }
             p=p-2*dx;
         }

         if(change==0){
             x=x+s1;
         }
         else{
             y=y+s2;
         }
         p=p+2*dy;
         glVertex2i(x,y);
     }
}


#endif // MYFUNCTION_H
