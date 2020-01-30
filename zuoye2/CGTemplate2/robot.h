/*###################################################
##  姓名：江金昱
##  文件说明：调用读入的obj模型进行绘制，同时转动手臂和腿
##  包括设置OpenGL渲染环境，设置视口、投影，设置窗口大小改变时的动作
##  渲染OpenGL场景，设置每次部件更新的时候调用的函数
#####################################################*/

#ifndef ROBOT_H
#define ROBOT_H

#include"myglwidget.h"
#include"objloader.h"

// 腿转动的角度
// legAngle[0]为右腿，legAngle[1]为左腿
extern float legAngle[2];
// 手臂转动的角度
// armAngle[0]为右腿，armAngle[1]为左腿
extern float armAngle[2];


/*###################################################
##  函数: DrawRobot
##  函数描述： 绘制机器人的头、身体、腿、手，并且实现腿和手的转动
##  参数描述：
##  x:沿着x轴移动的长度
##  y:沿着y轴移动的长度
##  z:沿着z轴移动的长度
##  objmodel: 载入obj模型后封装的类
##  epsilon: 手臂和腿每次挥动的幅度
#####################################################*/
void DrawRobot(float x, float y, float z,objModel& objmodel,float epsilon)
{
    // 机器人腿的状态
    // true=前,false=后
    static bool leg1 = true;
    static bool leg2 = false;

    // 机器人手臂的状态
    // true=前,false=后
    static bool arm1 = false;
    static bool arm2 = true;

    glPushMatrix();
    // 将机器人放置的位置
    glTranslatef(x, y, z);

    // 头放在身体的正上方
    // 载入机器人的头
    objmodel.objDraw("Head");
    // 载入机器人的身体（大肚皮）
    objmodel.objDraw("Chest_Belly");

    //----------右手------------
    glPushMatrix();
    //如果手臂向前移动,就增大角度,否则减小角度
    if (arm1){
        armAngle[0] = armAngle[0] + epsilon;
    }
    else
        armAngle[0] = armAngle[0] - epsilon;

    //一旦手臂在一个方向上达到了其最大角度值就将其反转
    if (armAngle[0] >= 12.0f)
        arm1 = false;
    if (armAngle[0] <= -12.0f)
        arm1 = true;

    // 外面两个glTranslatef是为了将手臂移动到原点以方便旋转
    glTranslatef(-8.0f,65.0f,0.0f);
    // 绕着x轴旋转，因为是在y-z平面内旋转的
    glRotatef(armAngle[0], 1.0f, 0.0f, 0.0f);
    glTranslatef(8.0f,-65.0f,0.0f);
    // 载入机器人的右手
    objmodel.objDraw("RightArm");
    glPopMatrix();
    //-----------------------------


    //-------------左手-------------
    glPushMatrix();
    //如果手臂向前移动,就增大角度,否则减小角度
    if (arm2)
        armAngle[1] = armAngle[1] + epsilon;
    else
        armAngle[1] = armAngle[1] - epsilon;
    //一旦手臂在一个方向上达到了其最大角度值就将其反转
    if (armAngle[1] >= 12.0f)
        arm2 = false;
    if (armAngle[1] <= -12.0f)
        arm2 = true;
    // 外面两个glTranslatef是为了将手臂移动到原点以方便旋转
    glTranslatef(11.0f,65.0f,0.0f);
    // 绕着x轴旋转，因为是在y-z平面内旋转的
    glRotatef(armAngle[1], 1.0f, 0.0f, 0.0f);
    glTranslatef(-11.0f,-65.0f,0.0f);
    // 载入机器人的左手
    objmodel.objDraw("LeftArm");
    glPopMatrix();
    //-----------------------------


    //---------右腿------------
    glPushMatrix();
    //如果腿向前移动,就增大角度,否则就减小角度
    if (leg1)
        legAngle[0] = legAngle[0] + epsilon;
    else
        legAngle[0] = legAngle[0] - epsilon;
    //一旦腿在一个方向达到了最大的角度值,就将其反转
    if (legAngle[0] >= 12.0f)
        leg1 = false;
    if (legAngle[0] <= -12.0f)
        leg1 = true;

    // 外面两个glTranslatef是为了将右腿移动到原点以方便旋转
    glTranslatef(-19.0f,64.0f,0.0f);
    // 绕着x轴旋转，因为是在y-z平面内旋转的
    glRotatef(legAngle[0], 1.0f, 0.0f, 0.0f);
    glTranslatef(19.0f,-64.0f,0.0f);
    // 画右腿
    objmodel.objDraw("RightLeg");
    glPopMatrix();
    //------------------------


    //----------左腿-----------
    glPushMatrix();
    //如果腿向前移动,就增大角度,否则就减小角度
    if (leg2)
        legAngle[1] = legAngle[1] + epsilon;
    else
        legAngle[1] = legAngle[1] - epsilon;
    //一旦腿在一个方向达到了最大的角度值,就将其反转
    if (legAngle[1] >= 12.0f)
        leg2 = false;
    if (legAngle[1] <= -12.0f)
        leg2 = true;
    // 外面两个glTranslatef是为了将左腿移动到原点以方便旋转
    glTranslatef(22.0f,64.0f,0.0f);
    // 绕着x轴旋转，因为是在y-z平面内旋转的
    glRotatef(legAngle[1], 1.0f, 0.0f, 0.0f);
    glTranslatef(-22.0f,-64.0f,0.0f);
    // 画左腿
    objmodel.objDraw("LeftLeg");
    glPopMatrix();
    //------------------------


    glPopMatrix();
}

#endif // ROBOT_H
