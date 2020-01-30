#pragma once
#include <BaseModel.h>

/*
继承模型类，画一个四方面片，用于模拟地面
*/
class ModelQuad : public BaseModel
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
	ModelQuad(Float3 position, Float3 rotation, Float3 scale, int objectId, int imgId, bool enableBlend = false, bool enableTwoSide = false);
	~ModelQuad();

	/*
	重写基类渲染函数，用于渲染面片
	*/
	virtual void Render(ObjReader& obj, ImageReader& image);
	
protected:
	/*每帧更新函数*/
	virtual void Tick();
};

