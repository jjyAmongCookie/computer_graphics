#include "BaseModel.h"

BaseModel::~BaseModel()
{

}

BaseModel::BaseModel(Float3 position, Float3 rotation, Float3 scale, int objectId, int imgId, bool enableBlend, bool enableTwoSide)
{
	//初始化位置，旋转，大小信息
	mPosition = position;
	mRotation = rotation;
	mScale = scale;

	//指定模型，图片ID
	objId = objectId;
	imageId = imgId;

	//初始化是否混合
	isEnableBlend = enableBlend;
	//初始化是否开启双面
	isEnableTwoSide = enableTwoSide;
}

void BaseModel::Update()
{
	//调用虚函数
	Tick();
}

void BaseModel::Render(ObjReader& obj, ImageReader& image)
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

	if (isEnableBlend) //绘制透明物体时
	{
		glEnable(GL_BLEND);
		glDepthMask(false);	//设置深度缓冲区只读
	}
	else           //绘制不透明物体
	{
		glDepthMask(true); //设置深度缓冲区可写
		glDisable(GL_BLEND);
	}

	//是否开启双面渲染
	if (isEnableTwoSide)
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	else
		glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

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

			GLfloat texCoord[3] = { obj.texcoordArray[face.Texcoord[i] - 1].x, obj.texcoordArray[face.Texcoord[i] - 1].y, obj.texcoordArray[face.Texcoord[i] - 1].z };
			glTexCoord3fv(texCoord);

			GLfloat position[3] = { obj.vertexArray[face.position[i] - 1].x, obj.vertexArray[face.position[i] - 1].y, obj.vertexArray[face.position[i] - 1].z };
			glVertex3fv(position);
		}

		glEnd();
	}
	
	glPopMatrix();
}