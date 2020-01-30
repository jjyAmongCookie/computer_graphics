#pragma once
#include "BaseModel.h"


/*
继承模型类，画一个四方面片，用于模拟鱼
*/
class FishModel :
	public BaseModel
{
public:
	/*
	position:模型初始化位置
	rotation:模型初始化旋转
	scale:模型缩放
	objdectId:模型ID号
	imgId:图片ID号
	enableBlend：是否开启颜色混合（用于透明渲染
	enableTwoSide：是否开启双面渲染（用于片状物体
	*/
	FishModel(Float3 position, Float3 rotation, Float3 scale, int objectId, int imgId, bool enableBlend = false, bool enableTwoSide = false);
	~FishModel();

	/*
	重写基类渲染函数，用于渲染面片
	*/
	virtual void Render(ObjReader& obj, ImageReader& image);

	/*每帧更新函数*/
	virtual void Tick();

	/*设置鱼群游动幅度*/
	void Init(float waveRange);

protected:
	/*旋转角度，用于变换*/
	float angle;

	/*游动速度*/
	float speed;
	/*时间计时器*/
	int time;
	/*游动幅度范围*/
	float waveRange;
	/*在XZ平面上与相机的距离*/
	int disFromCamera;
};

