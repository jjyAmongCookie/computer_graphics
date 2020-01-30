#pragma once
#include "Tool/ObjReader.h"
#include <stdlib.h>
#include <GL/glew.h>
#include <Tool/ObjReader.h>
#include <Tool/ImageReader.h>

/*模型基类，用于管理模型*/
class BaseModel
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
	BaseModel(Float3 position, Float3 rotation, Float3 scale, int objectId, int imgId, bool enableBlend = false, bool enableTwoSide = false);

	~BaseModel();

	/*被SceneManger每帧调用*/
	void Update();
	/*
	虚函数每帧调用，渲染函数
	obj:模型文件
	image:图片文件
	*/
	virtual void Render(ObjReader& obj, ImageReader& image);
	/*
	虚函数，每帧调用
	*/
	virtual void Tick() {};

	/*模型位置*/
	Float3 mPosition;
	/*模型旋转*/
	Float3 mRotation;
	/*模型缩放*/
	Float3 mScale;

	/*模型Id号*/
	int objId;
	/*图片Id号*/
	int imageId;
	/*角度转换*/
	float rotationTrans = 3.14 / 180.0;

protected:

	/*是否开启颜色混合*/
	bool isEnableBlend;
	/*是否开启双面渲染*/
	bool isEnableTwoSide;
};

