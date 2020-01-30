/*###################################################
##  姓名：江金昱
##  文件说明：实现片元着色器
##  对不在圆内的像素做剔除
##  计算像素点的法线
##  利用法线，灯光方向，视线方向模拟真实世界光照，输出到颜色
##  计算像素深度
#####################################################*/
#version 120

uniform vec4 ambient;   //环境光颜色
uniform vec4 diffuse;   //小球漫反射颜色
uniform vec4 specL;     //高光颜色
uniform float specPow;  //高光渐变曲率
uniform vec4 ligthv;    //灯在视图空间的坐标
uniform vec4 ligthDiffuse;   //灯光颜色

varying vec4 v;         //点在视图空间的坐标

void main()
{
    vec3 N;   //定义法线
    N.xy = gl_PointCoord*2 - vec2(1.0f);     //把纹理坐标从(0,1)范围映射到(1,1)
    float r = dot(N.xy, N.xy); //计算在x,y纹理平面上的点半径

    //超出单位半径剔除片元
    if(r>1.0)
        discard;

    //利用单位向量求z维度的值
    N.z = sqrt(1-r);
    //从纹理坐标转化到真实空间的朝向
    N.y = -N.y;

    vec3 L = normalize(v.xyz - ligthv.xyz);     //灯光的方向
    vec3 E = normalize(v.xyz);  // 得到眼睛看向点的朝向
    vec3 R = normalize(reflect(L,N));  //得到灯光经过该点反射后的方向
 
    vec4 diffuseColor = ligthDiffuse * diffuse * max(dot(N,-L), 0.0);  //得到明暗变化
    vec4 spec = ligthDiffuse * specL *pow(max(dot(R,-E),0.0), specPow); //得到高光

    gl_FragColor = ambient * diffuse + diffuseColor + spec;     //得到最终颜色
    gl_FragDepth = r + v.z;        //深度排序
}
