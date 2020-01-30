#pragma once
#include <Tool/ObjReader.h>
#include <Tool/ImageReader.h>
#include <Model/FishModel.h>
#include <list>

/*
鱼群生成器
*/
class FishEmitter
{
public:
	/*
	freTime:每隔这个时间生成一条鱼
	fishNum:场景内允许存在最多多少条由生成器产生的鱼
	pos:生成器位置，鱼初始参考坐标
	rot:生成器旋转度，鱼初始参考旋转
	objectID:鱼的模型ID号
	imgID:鱼模型的图片号
	waveRange:鱼游动的路线偏移角度
	*/
	FishEmitter(int freTime, int fishNum, Float3 pos, Float3 rot, int objectId, int imgId, float waveRange = 60);
	~FishEmitter();

	/*每帧更新*/
	void Tick();

	/*生成器位置*/
	Float3 mPosition;
	/*生成器旋转角*/
	Float3 mRotation;

private:
	/*时间计时器*/
	int time;
	/*数量计时器*/
	int num;
	/*每隔这个时间生成一条鱼*/
	int freTimeCount;
	/*鱼的模型ID号*/
	int objectId;
	/*鱼模型的图片号*/
	int imgId;
	/*鱼游动的路线偏移角度*/
	float waveRange;

	/*存放鱼群列表*/
	list<FishModel*> fishes;
};

