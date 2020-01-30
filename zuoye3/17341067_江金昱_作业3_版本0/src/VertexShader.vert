/*###################################################
##  姓名：江金昱
##  文件说明：实现顶点着色器
##  根据距离计算出点精灵的大小
##  依据时间和高度重定义位置
##  并将位置信息输出给片元着色器
#####################################################*/
#version 120
uniform float radius;   //半径
uniform float time;     //时间
uniform float RoundDis;     //环绕中心的半径

varying vec4 v;         //点在视图空间的坐标

void main()
{	
    float angle = time / 30 + gl_Vertex.y;  //按点的不同高度得到角度随时间的变化
    float Xoffset = cos(angle) * RoundDis;    //X方向的偏移量
    float Zoffset = sin(angle) * RoundDis;    //Z方向的偏移量

    vec4 eyePos = gl_ModelViewMatrix * gl_Vertex + vec4(Xoffset, 0, Zoffset, 1); //点坐标在视图空间的坐标

    //利用点坐标在视图的深度和预设的半径大小组合成新的坐标并转化到投影空间
    vec4 PointPos = gl_ProjectionMatrix * vec4(radius, radius, eyePos.z, eyePos.w);

    gl_PointSize = 800 * PointPos.x / PointPos.w; //设置点大小

    v = eyePos;   //传递给片元着色器

    gl_Position = gl_ProjectionMatrix * eyePos;     //设置点坐标
}
