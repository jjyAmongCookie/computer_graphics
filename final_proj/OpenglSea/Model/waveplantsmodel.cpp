/*###################################################
##  姓名: waveplantsmodel.cpp
##  文件说明：实现水草的加入渲染以及实现水草的摆动（几何模型的形变）
#####################################################*/
#include "waveplantsmodel.h"

/*###################################################
##  函数: WavePlantMode
##  函数描述： 构造函数，初始化成员变量
##  参数描述：
##  position:模型初始化位置
##  rotation:模型初始化旋转
##  scale:模型缩放
##  objdectId:模型ID号
##  imgId:图片ID号
##  waveHeight: 水草飘动的范围
##  enableBlend：是否开启颜色混合（用于透明渲染
##  enableTwoSide：是否开启双面渲染（用于片状物体
#####################################################*/
WavePlantMode::WavePlantMode(Float3 position, Float3 rotation, Float3 scale, int objectId, int imgId,float waveHeight, bool enableBlend, bool enableTwoSide)
    :BaseModel(position, rotation, scale, objectId, imgId, enableBlend, enableTwoSide)
{
    time = 0;
    this->waveHeight = waveHeight;
}

/*###################################################
##  函数: WavePlantMode
##  函数描述： 析构函数
#####################################################*/
WavePlantMode::~WavePlantMode()
{
}


/*###################################################
##  函数: Render
##  函数描述： 实现水草的绘制、纹理贴图、水草的飘动（形变）
##  参数描述：
##  obj: 水草的OBJ类
##  image： 载入的水草纹理
#####################################################*/
void WavePlantMode::Render(ObjReader& obj, ImageReader& image)
{
    //如果没有指定模型或者图片即停止渲染
    if (!obj.IsLoad() || !image.IsLoad())
        return;

    //位置矩阵
    GLfloat T[][4] = { {1, 0, 0, 0}, {0, 1, 0, 0}, {0, 0, 1, 0}, {mPosition.x, mPosition.y, mPosition.z, 1} };
    //绕Z轴旋转矩阵
    GLfloat RZ[][4] = { {cos(mRotation.z * rotationTrans), sin(mRotation.z * rotationTrans), 0, 0}, {-sin(mRotation.z* rotationTrans), cos(mRotation.z * rotationTrans), 0, 0}, {0, 0, 1, 0}, {0, 0, 0, 1} };
    //绕X轴旋转矩阵
    GLfloat RX[][4] = { {1, 0, 0, 0}, {0, cos(mRotation.x * rotationTrans), sin(mRotation.x * rotationTrans), 0}, {0, -sin(mRotation.x * rotationTrans), cos(mRotation.x * rotationTrans), 0}, {0, 0, 0, 1} };
    //绕Y轴旋转矩阵
    GLfloat RY[][4] = { {cos(mRotation.y * rotationTrans), 0, -sin(mRotation.y * rotationTrans), 0}, {0, 1, 0, 0}, {sin(mRotation.y * rotationTrans), 0, cos(mRotation.y * rotationTrans), 0}, {0, 0, 0, 1} };
    //缩放矩阵
    GLfloat S[][4] = { {mScale.x, 0, 0, 0}, {0, mScale.y, 0, 0}, {0, 0, mScale.z, 0}, {0, 0, 0, 1} };

    // 改变位置
    glPushMatrix();
    glMultMatrixf(*T);
    glMultMatrixf(*RZ);
    glMultMatrixf(*RX);
    glMultMatrixf(*RY);
    glMultMatrixf(*S);

    //开启混合，设定颜色混合模式
    glEnable(GL_BLEND);
    glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
    glDepthMask(false);	//设置深度缓冲区只读

    //绑定图片
    glBindTexture(GL_TEXTURE_2D, image.texID);
    for (auto face : obj.faceArray)
    {
        //为每个点设置法线，纹理，位置信息
        glBegin(GL_TRIANGLES);

        for (int i = 0; i < 3; i++)
        {
            GLfloat normal[3] = { obj.normalArray[face.normal[i] - 1].x, obj.normalArray[face.normal[i] - 1].y, obj.normalArray[face.normal[i] - 1].z };
            glNormal3fv(normal);

            GLfloat texCoord[3] = { obj.texcoordArray[face.Texcoord[i] - 1].x, obj.texcoordArray[face.Texcoord[i] - 1].y, obj.texcoordArray[face.Texcoord[i] - 1].z};
            glTexCoord3fv(texCoord);
            float y = obj.vertexArray[face.position[i] - 1].y;
            float z = sin(time/12.0f + y) * y / waveHeight + obj.vertexArray[face.position[i] - 1].z;
            GLfloat position[3] = { obj.vertexArray[face.position[i] - 1].x, y, z};
            glVertex3fv(position);
        }

        glEnd();
    }

    glPopMatrix();
}

/*###################################################
##  函数: Tick()
##  函数描述： 递增时间
#####################################################*/
void WavePlantMode::Tick()
{
    time++;
}

