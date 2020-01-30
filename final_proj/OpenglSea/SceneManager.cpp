#include "SceneManager.h"
#include "BaseModel.h"
#include "Model/waveplantsmodel.h"

extern Float3 cameraPos;

/*###################################################
##  函数：SceneManager
##  函数描述：SceneManager类的构造函数，置空
##  参数描述：无
#####################################################*/
SceneManager::SceneManager()
{
}

/*###################################################
##  函数：~SceneManager
##  函数描述：SceneManager类的析构函数，删除模型释放鱼群，
##          删除半透明物体。
##  参数描述：无
#####################################################*/
SceneManager::~SceneManager()
{
	for (auto cur : modelList)
	{
		delete cur;
	}

	//优先释放，为的是先从translucentModel移除掉鱼
	delete fishEmitter1;
	delete fishEmitter2;
	delete fishEmitter3;

	for (auto cur : translucentModel)
	{
		delete cur.second;
	}

	modelList.clear();
	translucentModel.clear();
}

/*###################################################
##  函数：Init
##  函数描述：SceneManager类的初始化函数，初始化模型、天空和
##          光照、雾等环境，初始化鱼群生成器。
##  参数描述：无
#####################################################*/
void SceneManager::Init()
{
	InitModel();

	InitSky();
	InitLight();
	InitFog();

	//初始化鱼群生成器
	fishEmitter1 = new FishEmitter(10, 100, Float3(-10, 100, 50), Float3(0, 25, 0), 12, 12, 40);
	fishEmitter2 = new FishEmitter(10, 100, Float3(-10, 100, -100), Float3(0, 0, 0), 12, 13, -30);
	fishEmitter3 = new FishEmitter(10, 100, Float3(-10, 100, -100), Float3(0, 0, 0), 12, 14, -60);
}

/*###################################################
##  函数：Update
##  函数描述：更新函数，每次设置天空、更新模型和半透明物体，
##          并更新鱼群位置和渲染模型和半透明物体。
##  参数描述：无
#####################################################*/
void SceneManager::Update()
{
	SetSky();

	for (auto cur : modelList)
	{
		cur->Update();
	}

	map<int, BaseModel*> temp = translucentModel; //防止update()内对列表的更改
	for (auto cur : temp)
	{
		cur.second->Update();
	}

	//鱼群更新位置
	fishEmitter1->Tick();
	fishEmitter2->Tick();
	fishEmitter3->Tick();

	//先渲染实体
	for (auto cur : modelList)
	{
		cur->Render(objList[cur->objId], imageList[cur->imageId]);
	}

	//再渲染透明物体，利用Map的元素自动排序功能，按深度大小绘制透明物体
	for (map<int, BaseModel*>::reverse_iterator cur = translucentModel.rbegin(); cur != translucentModel.rend(); cur++)
	{
		(*cur).second->Render(objList[(*cur).second->objId], imageList[(*cur).second->imageId]);
	}
}

/*###################################################
##  函数：InitModel
##  函数描述：加载模型和图片到内存，并将实物物体和半透明物体加到
##          模型列表中。
##  参数描述：无
#####################################################*/
void SceneManager::InitModel()
{
	//加载模型进内存
    objList.push_back(ObjReader("objMesh/SM_URockB.obj")); //0
    objList.push_back(ObjReader("objMesh/SM_URockC.obj")); //1
    objList.push_back(ObjReader("objMesh/SM_URockD.obj")); //2
    objList.push_back(ObjReader("objMesh/SM_RedAlgaeB.obj"));	//3
    objList.push_back(ObjReader("objMesh/SM_RedAlgaeC.obj"));	//4
    objList.push_back(ObjReader("objMesh/SM_SeaweedA.obj"));	//5
    objList.push_back(ObjReader("objMesh/SM_SeaweedC.obj"));	//6
    objList.push_back(ObjReader("objMesh/SM_SeaweedD.obj"));	//7
    objList.push_back(ObjReader("objMesh/SM_SpongeB.obj"));	//8
    objList.push_back(ObjReader("objMesh/SM_SpongeC.obj"));	//9
    objList.push_back(ObjReader("objMesh/SM_AlgaeA.obj"));	//10
    objList.push_back(ObjReader("objMesh/SM_CorallineB.obj"));	//11
    objList.push_back(ObjReader("objMesh/SM_land.obj")); //12

    //加载图片进内存
    imageList.push_back(ImageReader("Textures/T_URockB_BC.png"));	//0
    imageList.push_back(ImageReader("Textures/T_URockC_BC.png"));	//1
    imageList.push_back(ImageReader("Textures/T_URockD_BC.png"));	//2
    imageList.push_back(ImageReader("Textures/T_FishA_BC.png"));	//3
    imageList.push_back(ImageReader("Textures/T_FishB_BC.png"));	//4
    imageList.push_back(ImageReader("Textures/T_FishC_BC.png"));	//5
    imageList.push_back(ImageReader("Textures/T_RedAlgae_BC.png"));	//6
    imageList.push_back(ImageReader("Textures/T_Seaweed_BC.png"));		//7
    imageList.push_back(ImageReader("Textures/T_Sponge_BC.png"));		//8
    imageList.push_back(ImageReader("Textures/T_SandDunes_BC.png"));		//9
    imageList.push_back(ImageReader("Textures/T_Algae_BC.png"));		//10
    imageList.push_back(ImageReader("Textures/T_Coralline_BC.png"));		//11
    imageList.push_back(ImageReader("Textures/T_FishA_BC.png"));		//12
    imageList.push_back(ImageReader("Textures/T_FishB_BC.png"));		//13
    imageList.push_back(ImageReader("Textures/T_FishC_BC.png"));		//14


	//实体物体, 不需要排序
	modelList.push_back(new BaseModel(Float3(340, 0, -240), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 0, 0));
	modelList.push_back(new BaseModel(Float3(420, 0, -110), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 1, 1));
	modelList.push_back(new BaseModel(Float3(570, 100, -230), Float3(0, 0, 60), Float3(1.0, 1.0, 1.0), 2, 2));
	modelList.push_back(new BaseModel(Float3(1340, 170, 1190), Float3(0, 0, 0), Float3(3.0, 3.0, 3.0), 2, 2));
	modelList.push_back(new BaseModel(Float3(380, 0, 190), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 8, 8));
	modelList.push_back(new BaseModel(Float3(960, 0, -40), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 9, 8));
	modelList.push_back(new BaseModel(Float3(850, 0, 440), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 9, 8));
	modelList.push_back(new BaseModel(Float3(460, 0, 20), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 10, 10));
	modelList.push_back(new BaseModel(Float3(900, 0, -710), Float3(0, 270, 0), Float3(2.0, 2.0, 2.0), 11, 11));
    modelList.push_back(new ModelQuad(Float3(0, 0, 0), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 12, 9));

	//透明物体
    translucentModel.insert(pair<int, BaseModel*>((Float3(420, cameraPos.y, 380) - cameraPos).Length(), new WavePlantMode(Float3(420, -30, 380), Float3(0, 30, 0), Float3(1.0, 1.0, 1.0), 7, 7, 100)));
    translucentModel.insert(pair<int, BaseModel*>((Float3(480, cameraPos.y, 330) - cameraPos).Length(), new WavePlantMode(Float3(480, -30, 330), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 7, 7, 100)));
    translucentModel.insert(pair<int, BaseModel*>((Float3(590, cameraPos.y, -200) - cameraPos).Length(), new WavePlantMode(Float3(590, 230, -200), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 3, 6, 10)));
    translucentModel.insert(pair<int, BaseModel*>((Float3(670, cameraPos.y, -60) - cameraPos).Length(), new WavePlantMode(Float3(670, 10, -60), Float3(0, 60, 0), Float3(1.0, 1.0, 1.0), 6, 7, 30)));
    translucentModel.insert(pair<int, BaseModel*>((Float3(670, cameraPos.y, 260) - cameraPos).Length(), new WavePlantMode(Float3(670, 30, 260), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 5, 7, 30)));
    translucentModel.insert(pair<int, BaseModel*>((Float3(770, cameraPos.y, -620) - cameraPos).Length(), new WavePlantMode(Float3(770, -30, -620), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 5, 7, 30)));
    translucentModel.insert(pair<int, BaseModel*>((Float3(960, cameraPos.y, 740) - cameraPos).Length(), new WavePlantMode(Float3(960, -10, 740), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 3, 6, 10)));
    translucentModel.insert(pair<int, BaseModel*>((Float3(800, cameraPos.y, 90) - cameraPos).Length(), new WavePlantMode(Float3(800, 0, 200), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 4, 6, 10)));
    translucentModel.insert(pair<int, BaseModel*>((Float3(1100, cameraPos.y, 400) - cameraPos).Length(), new WavePlantMode(Float3(1100, -30, 350), Float3(0, 0, 0), Float3(1.0, 1.0, 1.0), 6, 7, 30)));
}

/*###################################################
##  函数：InitSky
##  函数描述：加载图片设置面片纹理，当做天空
##  参数描述：无
#####################################################*/
void SceneManager::InitSky()
{
	//设置天空
	sky = ImageReader("Textures\\sky.png");
}

/*###################################################
##  函数：SetSky
##  函数描述：将图片通过旋转等变换变为真实度感的天空
##  参数描述：无
#####################################################*/
void SceneManager::SetSky()
{
	glPushMatrix();
	glRotatef(90, 1.0, 0.0, 0.0);
	
	GLUquadricObj *quadratic = gluNewQuadric();
	gluQuadricTexture(quadratic, GL_TRUE);
	gluQuadricOrientation(quadratic, GLU_INSIDE);
	gluQuadricNormals(quadratic, GLU_SMOOTH);
	glBindTexture(GL_TEXTURE_2D, sky.texID);
	gluSphere(quadratic, 10000, 30, 30);
	
	glPopMatrix();
}


/*###################################################
##  函数：InitLight
##  函数描述：初始化光照，全局光照
##  参数描述：无
#####################################################*/
void SceneManager::InitLight()
{
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	GLfloat LightAmbient[] = { 0.8f, 0.8f, 0.8f, 1.0f };
    GLfloat LightDiffuse[] = { 0.9f, 0.9f, 0.9f, 1.0f };
	GLfloat LightPosition[] = { 0.0f, 200.0f, 0.0f, 1.0f };

    glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmbient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, LightDiffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, LightPosition);
}

/*###################################################
##  函数：InitFog
##  函数描述：初始化雾，实现远近对比效果
##  参数描述：无
#####################################################*/
void SceneManager::InitFog()
{
	glEnable(GL_FOG);

    GLfloat fogColor[] = { 0.2f, 0.45f, 0.7f, 1.0f};

	glFogi(GL_FOG_MODE, GL_LINEAR); //设置线性雾
	glFogfv(GL_FOG_COLOR, fogColor); //雾气颜色
	glFogf(GL_FOG_START, 0.0);		//雾开始位置距离摄像机距离
    glFogf(GL_FOG_END, 1500);		//雾结束位置距离摄像机距离
}
