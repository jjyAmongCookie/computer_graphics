/*###################################################
##  姓名：江金昱
##  文件说明：载入obj模型，包括读取obj模型，以及各部件的具体绘制
#####################################################*/
#ifndef OBJLOADER_H
#define OBJLOADER_H

#include<myglwidget.h>
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include<vector>
#include"BMPLoader.h"

using namespace std;

// 类型为float的3个元素的向量
struct Float3
{
    float Data[3];
};

// 类型为float的2个元素的向量
struct Float2
{
    float Data[2];
};

// 面信息
struct Face
{
    // 每个面有四个点，每个面存储 位置索引/纹理索引/法线索引
    int vertex[4][3];
};

// 存储材质信息
struct mMaterial
{
    // 反射指数，表示反射高光度，值越高则高光越密集
    float Ns;
    // 材质的环境光
    float Ka[3];
    // 散射光
    float Kd[3];
    // 镜面光
    float Ks[3];
    // 材质名字
    string mTextureName;
    // 纹理的ID
    GLuint mTextureId;
    // 折射值
    float Ni;
    // 滤光透射率
    float Tf[3];
    // 渐隐指数
    float d;
};

// 存储纹理
std::vector<mMaterial> mMtl;

// 表示一定时间读入的顶点坐标数量、纹理坐标数量、法线坐标数量
int vnum=0;
int cnum=0;
int nnum=0;
// 上一个载入的部件名称
string last_part_name="";

// 用于载入BMP的类
CBMPLoader image;

/*###################################################
##  类: List
##  类描述： 模拟列表的插入操作
##  类成员函数描述：
##  Next():返回指向的下一个节点的指针
##  PushBack():在队列末尾插入指针
#####################################################*/
class List
{
public:
    List() :mNext(nullptr) {}
    //下一个节点
    void* mNext;
    template<typename T>
    //得到下一个节点
    T* Next()
    {
        return(T*)mNext;
    }
    //链表尾部插入节点
    void PushBack(List*);
};

/*###################################################
##  函数: PushBack
##  函数描述： 将参数传入的节点置入队列末尾
##  参数描述：
##  node：需要添加进队列末尾的节点
#####################################################*/
void List::PushBack(List* node)
{
    List* mNode = this;
    // 获取指向队列尾的指针
    while (mNode->mNext != nullptr)
    {
        mNode = mNode->Next<List>();
    }
    // 向队列尾部插入节点
    mNode->mNext = node;

}

/*###################################################
##  类：objModel
##  类描述：实现obj文件的加载、obj模型的绘制
##  类成员函数描述：
##  loadObjModel():加载字符串指向的obj文件，读入其中的顶点等数据
##  objDraw():在窗口上绘制字符串所指明的部件名称
#####################################################*/
class objModel :public List
{
public:
    // 类初始化函数
    objModel() {
        vertnum=0;
        coordnum=0;
        nornum=0;
    }
    // 加载字符串指向的obj文件，读入其中的顶点等数据
    void loadObjModel(const char*);
    // 在窗口上绘制字符串所指明的部件名称
    void objDraw(string partname);
    // 法线
    vector<Float3> normal;
    // 纹理坐标
    vector<Float3> texcoord;
    // 点的位置信息
    vector<Float3> position;
    // 面的索引
    vector<Face> face;
    // 当前模型之前的顶点坐标数量
    int vertnum=0;
    // 当前模型之前的纹理坐标数量
    int coordnum=0;
    // 当前模型之前的法线坐标数量
    int nornum=0;
    // 部件名字
    std::string partName;
    // 部件所使用的mtl（材质）文件名字
    std::string mtlName;
    // 是否存在纹理
    bool exist_texture=false;
};


/*###################################################
##  函数: makeImage
##  函数描述： 绑定纹理ID
##  参数描述：
##  m_Texture: 自己创建的纹理类，包含纹理ID，图片加载放置的内存等信息
#####################################################*/
void makeImage(CBMPLoader *m_Texture)
{
    // 设置字节对齐
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // 生成1个纹理ID
    glGenTextures(1, &m_Texture->ID);
    // 将TEXTURE_2D绑定到该ID上
    glBindTexture(GL_TEXTURE_2D, m_Texture->ID);
    // 设置贴图参数
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // 对颜色进行组合
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    // 使用MIP纹理贴图
    gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_Texture->imageWidth, m_Texture->imageHeight,
          GL_RGB, GL_UNSIGNED_BYTE, m_Texture->image);
}


/*###################################################
##  函数: loadObjModel
##  函数描述：加载objFileName指定的obj文件
##  参数描述：
##  objFileName是obj文件的路径
#####################################################*/
void objModel::loadObjModel(const char * objFileName)
{
    objModel* tempModel = new objModel;
    ifstream infile(objFileName,ios::in);
    // 判断文件是否存在
    if(!infile){
        cerr<<"open error."<<endl;
        // 退出程序
        exit(1);
    }
    // 接受无关信息
    string temp;
    // 读取一行的数据
    char szoneLine[256];

    // 循环，当没有读取完毕时
    while(infile)
    {
        infile.getline(szoneLine,256);
//        cout<<szoneLine<<endl;
        // 该行不为空
        if (strlen(szoneLine) > 0)
        {
            // 获取v开头的数据
            if (szoneLine[0] == 'v')
            {
                // 数据存储到stringstream流
                stringstream ssOneLine(szoneLine);
                // 纹理信息
                if (szoneLine[1] == 't')
                {
                    // 总纹理坐标数+1
                    cnum+=1;
                    // 过滤标识符 vt
                    ssOneLine >> temp;
                    // 纹理坐标向量
                    Float3 tempTexcoord;
                    // 存储纹理坐标
                    ssOneLine >> tempTexcoord.Data[0] >> tempTexcoord.Data[1]>>tempTexcoord.Data[2];

                    // 将纹理坐标存入容器
                    tempModel->texcoord.push_back(tempTexcoord);
                    tempModel->coordnum+=1;
                    tempModel->exist_texture=true;
                }
                // 记录的法线信息
                if (szoneLine[1] == 'n')
                {
                    // 总法线坐标数+1
                    nnum+=1;
                    // 接收标识符vn
                    ssOneLine >> temp;
                    // 法线坐标向量
                    Float3 tempNormal;
                    // 存储法线坐标
                    ssOneLine >> tempNormal.Data[0] >> tempNormal.Data[1] >> tempNormal.Data[2];
                    // 将法线信息存入容器
                    tempModel->normal.push_back(tempNormal);
                    tempModel->nornum+=1;
                }
                // 点的位置信息
                if(szoneLine[1]==' ')
                {
                    // 总顶点坐标数+1
                    vnum+=1;
                    ssOneLine >> temp;
                    Float3 tempLocation;
                    ssOneLine >> tempLocation.Data[0] >> tempLocation.Data[1] >> tempLocation.Data[2];
                    tempModel->position.push_back(tempLocation);
                    tempModel->vertnum+=1;
                }
            }
            // 记录面信息
            else if (szoneLine[0] == 'f')
            {
                // 读取一行数据
                stringstream ssOneLine(szoneLine);
                // 接收标识符f
                ssOneLine >> temp;
                // 一行的数据例子:f 1/1/1 2/2/2 3/3/3 4/4/4
                // 分别表示:位置索引/纹理索引/法线索引   四个点构成一个面

                // 接收类似于1/1/1的一组索引
                string vertexStr;
                // 面矩阵
                Face tempFace;
                // 每个面4个点
                for (int i = 0; i < 4; ++i)
                {
                    // 从流中读取点的索引信息
                    ssOneLine >> vertexStr;
                    // 找到第一个'/'的位置,即找到点的位置信息
                    size_t pos = vertexStr.find_first_of('/');
                    // 顶点位置信息
                    string locIndexStr = vertexStr.substr(0, pos);
                    // 找到第二个'/',即找到点的纹理坐标信息
                    size_t pos2 = vertexStr.find_first_of('/', pos + 1);
                    // 如果是双斜线的情况即'//',例如 1//1
                    if (pos2 - pos == 1)
                    {
                        // 获取法线信息
                        string norIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - pos2 - 1);
                        // 将索引信息从 srting转换为int,顶点位置索引信息赋值
                        tempFace.vertex[i][0] = atoi(locIndexStr.c_str());
                        tempFace.vertex[i][2] = atoi(norIndexStr.c_str());
                    }
                    // 单斜线的情况，例如 1/1/1
                    else
                    {
                        // 纹理坐标索引信息
                        string texIndexSrt = vertexStr.substr(pos + 1, pos2 - 1 - pos);
                        // 点的法线信息
                        string norIndexSrt = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2);
                        // 顶点索引信息
                        tempFace.vertex[i][0] = atoi(locIndexStr.c_str());
                        // 纹理索引信息
                        tempFace.vertex[i][1] = atoi(texIndexSrt.c_str());
                        // 法线索引信息
                        tempFace.vertex[i][2] = atoi(norIndexSrt.c_str());
                    }
                }
                tempModel->face.push_back(tempFace);
            }
            // 处理注释的内容，即不同部件
            else if (szoneLine[0] == '#')
            {
                //流读取一行数据
                stringstream ssOneLine(szoneLine);
                //接收#
                ssOneLine >> temp;
                string numtemp;
                // 以下为循环，读取各部件的名称
                while (ssOneLine)
                {
                    ssOneLine >> temp;
                    // 如果该部件是肚子
                    if(temp=="Chest_Belly")
                    {
                        tempModel->partName=last_part_name;
                        tempModel->coordnum = cnum-tempModel->coordnum;
                        tempModel->vertnum = vnum-tempModel->vertnum;
                        tempModel->nornum = nnum-tempModel->nornum;
                        PushBack(tempModel);
                        last_part_name="Chest_Belly";

                        tempModel = new objModel;
                        break;
                    }
                    // 如果该部件是左腿
                    else if(temp=="LeftLeg")
                    {
                        tempModel->partName=last_part_name;
                        tempModel->coordnum = cnum-tempModel->coordnum;
                        tempModel->vertnum = vnum-tempModel->vertnum;
                        tempModel->nornum = nnum-tempModel->nornum;
                        PushBack(tempModel);
                        last_part_name="LeftLeg";
                        tempModel = new objModel;
                        break;
                    }
                    // 如果该部件是左手
                    else if(temp=="LeftArm")
                    {
                        tempModel->partName=last_part_name;
                        tempModel->coordnum = cnum-tempModel->coordnum;
                        tempModel->vertnum = vnum-tempModel->vertnum;
                        tempModel->nornum = nnum-tempModel->nornum;
                        PushBack(tempModel);
                        last_part_name="LeftArm";
                        tempModel = new objModel;
                        break;
                    }
                    // 如果该部件是右手
                    else if(temp=="RightArm")
                    {
                        tempModel->partName=last_part_name;
                        tempModel->coordnum = cnum-tempModel->coordnum;
                        tempModel->vertnum = vnum-tempModel->vertnum;
                        tempModel->nornum = nnum-tempModel->nornum;
                        PushBack(tempModel);
                        last_part_name="RightArm";
                        tempModel = new objModel;
                        break;
                    }
                    // 如果该部件是标记
                    else if(temp=="Stamp"){
                        tempModel->partName=last_part_name;
                        tempModel->coordnum = cnum-tempModel->coordnum;
                        tempModel->vertnum = vnum-tempModel->vertnum;
                        tempModel->nornum = nnum-tempModel->nornum;
                        PushBack(tempModel);
                        last_part_name="Stamp";
                        tempModel = new objModel;
                        break;
                    }
                    // 如果该部件是头
                    else if(temp=="Head"){
                        tempModel->partName=last_part_name;
                        tempModel->coordnum = cnum-tempModel->coordnum;
                        tempModel->vertnum = vnum-tempModel->vertnum;
                        tempModel->nornum = nnum-tempModel->nornum;
                        PushBack(tempModel);
                        last_part_name="Head";
                        tempModel = new objModel;
                        break;
                    }
                    // 如果该部件是右腿
                    else if(temp=="RightLeg")
                    {
                        tempModel->partName=last_part_name;
                        tempModel->coordnum = cnum-tempModel->coordnum;
                        tempModel->vertnum = vnum-tempModel->vertnum;
                        tempModel->nornum = nnum-tempModel->nornum;
                        PushBack(tempModel);
                        last_part_name="RightLeg";
                        tempModel = new objModel;
                        break;
                    }
                    // 获取顶点信息，即有多少个顶点
                    else if (temp == "vertices")
                    {
                        tempModel->vertnum = vnum;
                        vnum+=atoi(numtemp.c_str());
                        break;
                    }
                    // 获取纹理信息，即有多少个纹理
                    else if (temp == "texture")
                    {
                        tempModel->coordnum = cnum;
                        cnum+=atoi(numtemp.c_str());
                        if(atoi(numtemp.c_str())==0){
                            tempModel->exist_texture=false;
                            cout<<tempModel->partName+" not exist texture."<<endl;
                        }
                        else{
                            tempModel->exist_texture=true;
                        }
                        break;
                    }
                    // 获取法线信息，即有多少个法线
                    else if (temp == "vertex")
                    {
                        tempModel->nornum = nnum;
                        nnum+=atoi(numtemp.c_str());
                        break;
                    }
                    numtemp = temp;
                }
            }
            // 获取使用的材质库的名称
            else if (szoneLine[0] == 'u')
            {
                stringstream ssOneLine(szoneLine);
                //temp接收材质名
                ssOneLine >> temp >> temp;
                tempModel->mtlName = temp;
            }
            // 处理材质库信息
            else if (szoneLine[0] == 'm')
            {
                mMaterial *tempMtl=new mMaterial;
                stringstream ssOneLine(szoneLine);
                // 过滤掉mtllib
                ssOneLine >> temp;
                // 获取材质名
                ssOneLine >> temp;
                // 材质文件路径
                string fileName = "D:/computer_graphics/project/test_function/baymax/" + temp;
                const char* mtlFileName = fileName.c_str();
                fstream mtlFile;
                mtlFile.open(mtlFileName);

                // 打开失败
                if (mtlFile.fail()){
                    cout <<"File name: "<<mtlFileName<< ",mtl file open fail" << std::endl;
                    continue;
                }
                // 循环读完整个文件
                while (!mtlFile.eof())
                {
                    string temp;
                    mtlFile >> temp;
                    if (temp == "newmtl")
                    {
                        mMtl.push_back(*tempMtl);
                        tempMtl=new mMaterial;
                        mtlFile >> tempMtl->mTextureName;
                    }
                    // 反射指数
                    else if (temp == "Ns")
                    {
                        mtlFile >> tempMtl->Ns;
                    }
                    // 环境光
                    else if (temp == "Ka")
                    {
                        mtlFile >> tempMtl->Ka[0] >> tempMtl->Ka[1] >> tempMtl->Ka[2];
                    }
                    // 散射光
                    else if (temp == "Kd")
                    {
                        mtlFile >> tempMtl->Kd[0] >> tempMtl->Kd[1] >> tempMtl->Kd[2];
                    }
                    // 镜面光
                    else if (temp == "Ks")
                    {
                        mtlFile >> tempMtl->Ks[0] >> tempMtl->Ks[1] >> tempMtl->Ks[2];
                    }
                }
                mMtl.push_back(*tempMtl);
            }
        }
    }
    tempModel->partName=last_part_name;
    tempModel->coordnum = cnum-tempModel->coordnum;
    tempModel->vertnum = vnum-tempModel->vertnum;
    tempModel->nornum = nnum-tempModel->nornum;
    PushBack(tempModel);
}

/*###################################################
##  函数: objDraw
##  函数描述：加载由参数partname指定的部件，并绘制到屏幕上
##  参数描述：
##  partname:部件的名字，比如手、腿等
#####################################################*/
void objModel::objDraw(string partname)
{
    glEnable(GL_TEXTURE_2D);
    // 存在点
    string thispartname=this->partName;
    if(partname!=thispartname){
        goto l1;
    }
    // 如果顶点大小不为0
    if (position.size() != 0)
    {
        // 存在mtl文件
        if (mtlName.length()!=0)
        {
            // 对该部件对应的纹理贴图进行绘制
            for (auto i = mMtl.begin(); i < mMtl.end(); ++i)
            {
                if (i->mTextureName == mtlName)
                {
                    //设置材质属性
                    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, i->Ka);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, i->Kd);
                    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, i->Ks);
                    glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, i->Ns);
                    glBindTexture(GL_TEXTURE_2D, i->mTextureId);
                    break;
                }
            }

        }
        //允许平滑着色
        glShadeModel(GL_SMOOTH);
        // 绘制物体
        glPushMatrix();
        glColor3f(1.0f,1.0f,1.0f);
        if (exist_texture == false)
        {
            Float3 d;
            d.Data[0]=0;
            d.Data[1]=0;
            d.Data[2]=0;
            for (unsigned int i = 0; i < face.size(); ++i)
            {
                glBegin(GL_POLYGON);
                //第一个点的法线，位置信息
                glNormal3fv(normal[face[i].vertex[0][2] - 1 - nornum].Data);
                glTexCoord3fv(d.Data);
                glVertex3fv(position[face[i].vertex[0][0] - 1 - vertnum].Data);
                //第二个点的法线，位置信息
                glNormal3fv(normal[face[i].vertex[1][2] - 1 - nornum].Data);
                glTexCoord3fv(d.Data);
                glVertex3fv(position[face[i].vertex[1][0] - 1 - vertnum].Data);
                //第三个点的法线，位置信息
                glNormal3fv(normal[face[i].vertex[2][2] - 1 - nornum].Data);
                glTexCoord3fv(d.Data);
                glVertex3fv(position[face[i].vertex[2][0] - 1 - vertnum].Data);
                //第四个点的法线，位置信息
                glNormal3fv(normal[face[i].vertex[3][2] - 1 - nornum].Data);
                glTexCoord3fv(d.Data);
                glVertex3fv(position[face[i].vertex[3][0] - 1 - vertnum].Data);
                glEnd();
            }

        }
        else
        {
            // 绑定纹理
            glBindTexture( GL_TEXTURE_2D, image.ID);
            // 绘制每一个面
            for (unsigned int i = 0; i < face.size(); ++i)
            {
                glBegin(GL_POLYGON);
                //第一个点的法线，纹理，位置信息
                glNormal3fv(normal[face[i].vertex[0][2] - 1 - nornum].Data);
                glTexCoord3fv(texcoord[face[i].vertex[0][1] - 1 - coordnum].Data);
                glVertex3fv(position[face[i].vertex[0][0] - 1 - vertnum].Data);
                //第二个点的法线，纹理，位置信息
                glNormal3fv(normal[face[i].vertex[1][2] - 1 - nornum].Data);
                glTexCoord3fv(texcoord[face[i].vertex[1][1] - 1 - coordnum].Data);
                glVertex3fv(position[face[i].vertex[1][0] - 1 - vertnum].Data);
                //第三个点的法线，纹理，位置信息
                glNormal3fv(normal[face[i].vertex[2][2] - 1 - nornum].Data);
                glTexCoord3fv(texcoord[face[i].vertex[2][1] - 1 - coordnum].Data);
                glVertex3fv(position[face[i].vertex[2][0] - 1 - vertnum].Data);
                //第四个点的法线，纹理，位置信息
                glNormal3fv(normal[face[i].vertex[3][2] - 1 - nornum].Data);
                glTexCoord3fv(texcoord[face[i].vertex[3][1] - 1 - coordnum].Data);
                glVertex3fv(position[face[i].vertex[3][0] - 1 - vertnum].Data);
                glEnd();
            }
        }

        glPopMatrix();
    }
    // 遍历直到队尾
l1: if (Next<objModel>() != nullptr)
    {
        Next<objModel>()->objDraw(partname);
    }
    else
        return;

}


#endif // OBJLOADER_H
