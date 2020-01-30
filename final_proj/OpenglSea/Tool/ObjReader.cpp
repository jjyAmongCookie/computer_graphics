#include <iostream>
#include <sstream>
#include <fstream>
#include "Tool/ObjReader.h"
#include <QtCore/qdir.h>

using namespace std;

ObjReader::ObjReader(const char* filename)
{
	isLoad = false;

	ObjLoadModel(filename);
}

ObjReader::~ObjReader()
{

}

bool ObjReader::ObjLoadModel(const char* filename)
{
    // 转化成绝对路径
    string dir = QDir::currentPath().toStdString() + "/" + filename;
    const char* s = dir.c_str();

    ifstream file;
	// 打开文件,并检查错误
    file.open(s, ios_base::in);
	if (!file.is_open())
	{
		cout << "open file failed." << endl;
		return false;
	}

	string temp; // 接受无关信息
	char szoneLine[256]; // 读取一行的数据

	// 循环读取
	while (file)
	{
		file.getline(szoneLine, 256);

		if (strlen(szoneLine) > 0) // 该行不为空
		{
			if (szoneLine[0] == 'v') // 获取v开头的数据
			{
				stringstream ssOneLine(szoneLine); // 数据存储到stringstream流
				if (szoneLine[1] == 't') // 纹理信息
				{
					ssOneLine >> temp; //接收标识符vt

					Float3 tempTexcoord; // 纹理坐标向量
					ssOneLine >> tempTexcoord.x >> tempTexcoord.y >> tempTexcoord.z; // 存储纹理坐标
					tempTexcoord.y = 1 - tempTexcoord.y;	//解决纹理上下翻转问题
					texcoordArray.push_back(tempTexcoord); // 将纹理坐标存入容器
				}
				else if (szoneLine[1] == 'n') // 记录的法线信息
				{
					ssOneLine >> temp; //接收标识符vn

					Float3 tempNormal;  //法线坐标向量
					ssOneLine >> tempNormal.x >> tempNormal.y >> tempNormal.z; // 存储法线坐标
					normalArray.push_back(tempNormal);//将法线信息存入容器
				}
				else if (szoneLine[1] == ' ') // 点的位置信息
				{
					ssOneLine >> temp; //接收标识符v

					Float3 tempLocation; //位置坐标
					ssOneLine >> tempLocation.x >> tempLocation.y >> tempLocation.z; //存储位置坐标
					vertexArray.push_back(tempLocation); // 将位置坐标存入容器
				}
			}
			else if (szoneLine[0] == 'f') //记录面信息
			{
				stringstream ssOneLine(szoneLine); //读取一行数据
				ssOneLine >> temp; //接收标识符f

				// 一行的数据例子:f 1/1/1 2/2/2 3/3/3 
				// 分别表示:位置索引/纹理索引/法线索引   三个点构成一个面
				// 接收类似于1/1/1的一组索引
				string vertexStr;
				Face tempFace; // 面矩阵
				for (int i = 0; i < 3; ++i) // 每个面3个点
				{
					ssOneLine >> vertexStr; // 从流中读取点的索引信息
					size_t pos = vertexStr.find_first_of('/'); // 找到第一个'/'的位置,即找到点的位置信息
					string locIndexStr = vertexStr.substr(0, pos); // 顶点位置信息
					size_t pos2 = vertexStr.find_first_of('/', pos + 1); // 找到第二个'/',即找到点的纹理坐标信息

					tempFace.position[i] = atoi(locIndexStr.c_str()); // 将索引信息从 srting转换为int,顶点位置索引信息赋值

					if (pos2 - pos == 1) // 如果是双斜线的情况即'//',例如 1//1
					{
						string norIndexStr = vertexStr.substr(pos2 + 1, vertexStr.length() - pos2 - 1); // 获取法线信息

						tempFace.normal[i] = atoi(norIndexStr.c_str());
						tempFace.Texcoord[i] = 0; // 标志为0：在后续处理时分配给0号（0,0，0）
					}
					else // 单斜线的情况，例如 1/1/1
					{
						string texIndexSrt = vertexStr.substr(pos + 1, pos2 - 1 - pos); // 点的纹理信息
						string norIndexSrt = vertexStr.substr(pos2 + 1, vertexStr.length() - 1 - pos2); // 点的法线信息

						tempFace.normal[i] = atoi(norIndexSrt.c_str()); //存储纹理索引信息
						tempFace.Texcoord[i] = atoi(texIndexSrt.c_str()); //存储法线索引信息
					}
				}
				faceArray.push_back(tempFace);
			}
		}

	}

	//防止没有纹理坐标的情况发生，给予默认值
	if (texcoordArray.size() == 0)
		texcoordArray.push_back(Float3(0, 0, 0)); 

	cout << "load model: " << filename << endl;

	file.close();
	isLoad = true;
	return true;
}


bool ObjReader::IsLoad()
{
	return isLoad;
}
