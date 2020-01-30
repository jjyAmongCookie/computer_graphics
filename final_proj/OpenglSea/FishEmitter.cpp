#include "FishEmitter.h"
#include "SceneManager.h"

/*###################################################
##  函数: FishEmitter
##  函数描述： FishEmitter类的构造函数，使用传入的参数初始化
            成员变量，fishes（鱼群队列）自动初始化为空队列
##  参数描述：
## freTime:每隔这个时间生成一条鱼
## fishNum:场景内允许存在最多多少条由生成器产生的鱼
## pos:生成器位置，鱼初始参考坐标
## rot:生成器旋转度，鱼初始参考旋转角
## objectID:鱼的模型ID号（实际上鱼不需要用到，只是继承了基类的属性
## imgID:鱼模型的图片号
## waveRange:鱼游动的路线偏移角度
#####################################################*/
FishEmitter::FishEmitter(int freTime, int fishNum, Float3 pos, Float3 rot, int objectId, int imgId, float waveRange)
{
	//初始化信息
	time = freTime;
	num = fishNum;
	mPosition = pos;
	mRotation = rot;
	freTimeCount = 0;

	this->objectId = objectId;
	this->imgId = imgId;
	this->waveRange = waveRange;
}

/*###################################################
##  函数: ~FishEmitter
##  函数描述： FishEmitter类的析构函数，释放队列中的鱼群
##  参数描述：无
#####################################################*/
FishEmitter::~FishEmitter()
{	
	//释放鱼
	for (auto cur : fishes)
	{
		delete cur;
	}

	fishes.clear();
}

/*###################################################
##  函数: Tick
##  函数描述： 每帧更新时间计数器，周期性地在鱼群中添加新的鱼，并在鱼的
            数量达到最大后删除最先加入的鱼
##  参数描述：无
#####################################################*/
void FishEmitter::Tick()
{
	//刷新时间计数器
	freTimeCount += 1;
	if (freTimeCount > time)
	{
		freTimeCount = 0;

		//产生一条新鱼
		FishModel* cur = new FishModel(mPosition, mRotation, Float3(1, 1, 1), objectId, imgId);
		cur->Init(waveRange);
		fishes.push_back(cur);
		//如果鱼的数量超出，释放队列最前方的鱼
		if (fishes.size() > num)
		{
			delete fishes.front();
			fishes.pop_front();
		}
	}

}
