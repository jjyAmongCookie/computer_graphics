#pragma once
#include <map>
#include <vector>
#include <Tool/ObjReader.h>
#include <Tool/ImageReader.h>
#include <BaseModel.h>
#include <Model/ModelQuad.h>
#include <FishEmitter.h>

#define WINDOW_WIDTH 1280	
#define WINDOW_HEIGHT 720

/*
*场景管理器：统管场景渲染
*/
class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	/*初始化场景设置*/
	void Init();

	/*每帧调用，先更新场景物体的位置，再进行渲染*/
	void Update();
	
	/*存储透明渲染队列*/
	map<int, BaseModel*> translucentModel;

private:
	/*初始化天空*/
	void InitSky();
	/*初始化灯光*/
	void InitLight();
	/*初始化雾*/
	void InitFog();

	/*渲染天空*/
	void SetSky();
	
	/*初始化场景模型*/
	void InitModel();

	ImageReader sky;

	/*鱼群生成器*/
	FishEmitter* fishEmitter1;
	FishEmitter* fishEmitter2;
	FishEmitter* fishEmitter3;

	/*存储模型文件*/
	vector<ObjReader> objList;
	/*存储图片文件*/
	vector<ImageReader> imageList;

	/*存储实体模型*/
	vector<BaseModel*> modelList;
};

