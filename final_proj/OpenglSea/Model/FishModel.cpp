#include "FishModel.h"
#include "SceneManager.h"

extern SceneManager scene;
extern Float3 cameraPos;

FishModel::FishModel(Float3 position, Float3 rotation, Float3 scale, int objectId, int imgId, bool enableBlend, bool enableTwoSide)
	: BaseModel(position, rotation, scale, objectId, imgId, enableBlend, enableTwoSide)
{
	//随机化初始坐标
	mPosition.y = mPosition.y + rand() % 300;
	mPosition.z = mPosition.z + rand() % 300;

	//随机化速度
	speed = rand()% 5 / 10.0 + 2;

	//初始化数据
	time = 0;
	angle = 0;
	this->waveRange = 60;

	//随机化大小
	float s = (float)(rand() % 5) / 5 + 1.0;
	mScale = Float3(s, s, s);

    //计算在XZ平面上与摄像机的距离
	Float3 p1 = mPosition;
	p1.y = cameraPos.y;
	disFromCamera = (p1 - cameraPos).Length();

	//根据与摄像机的距离加入到透明渲染队列，如果队列上已经有相同距离的物体存在，则将距离+1
	while (scene.translucentModel.count(disFromCamera))
	{
		disFromCamera++;
	}
	scene.translucentModel.insert(pair<int, BaseModel*>(disFromCamera, this));

}


FishModel::~FishModel()
{
	//释放场景内的自己
	scene.translucentModel.erase(disFromCamera);
}

void FishModel::Init(float waveRange)
{
	//设置游动偏移
	this->waveRange = waveRange;
}

void FishModel::Render(ObjReader& obj, ImageReader& image)
{
	//渲染，如果图片没有加载就停止
	if (!image.IsLoad())
		return;

	// 改变位置
	glPushMatrix();
	glTranslatef(mPosition.x, mPosition.y, mPosition.z);
	glRotatef(mRotation.y + angle, 0.0, 1.0, 0.0);
	glScalef(mScale.x, mScale.y, mScale.z);

	//一个面的顶点
	GLfloat vertices[][3] =
	{
		{-10.0f, 10.0f, 0.0f}, {-10.0f, -10.0f, 0.0f},
		{ 10.0f, 10.0f, 0.0f}, { 10.0f, -10.0f, 0.0f}
	};

	//开启混合，设定颜色混合模式
	glEnable(GL_BLEND);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glDepthMask(false);	//设置深度缓冲区只读

	//绑定图片
	glBindTexture(GL_TEXTURE_2D, image.texID);

	//绘制面
	GLfloat n[] = { 0, 0, 1 };
	glNormal3fv(n);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3fv(vertices[0]);

	glTexCoord2f(0, 1);
	glVertex3fv(vertices[1]);

	glTexCoord2f(1, 1);
	glVertex3fv(vertices[3]);

	glTexCoord2f(1, 0);
	glVertex3fv(vertices[2]);
	glEnd();

	glPopMatrix();
}

void FishModel::Tick()
{
	//计时器+1
	time += 1;

	//刷新渲染列表
	scene.translucentModel.erase(disFromCamera);	//移除之前的

	Float3 p1 = mPosition;
	p1.y = cameraPos.y;
	disFromCamera = (p1 - cameraPos).Length(); //更新深度值
	while (scene.translucentModel.count(disFromCamera)) //判断是否存在相同深度
	{
		disFromCamera++;
	}
	scene.translucentModel.insert(pair<int, BaseModel*>(disFromCamera, this));


	//计算当前鱼群角度转变
	angle = sin(time/120.0f) * waveRange;
	float angle2 = (angle + mRotation.y) * 3.14 / 180.0;
	
	//游动方向
	Float3 dir(cos(angle2), 0, -sin(angle2));

	//更新位置
	mPosition = Float3(mPosition.x + dir.x * speed, mPosition.y + dir.y * speed, mPosition.z + dir.z * speed);
}
