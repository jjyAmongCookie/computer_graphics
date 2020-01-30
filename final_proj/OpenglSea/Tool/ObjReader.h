#pragma once
#include <stdlib.h>
#include <vector>

using namespace std;

/*
三维数据，可用来表示位置，向量
*/
struct Float3 
{
	float x, y, z;

	Float3()
		: x(0), y(0), z(0)
	{
	}

	Float3(float x0, float y0, float z0)
		:x(x0), y(y0), z(z0)
	{
	}

	/*得到向量长度*/
	float Length()
	{
		return sqrt(x*x + y * y + z * z);
	}

	/*重载 - 运算符*/
	Float3& operator-(const Float3& right)
	{
		x = this->x - right.x;
		y = this->y - right.y;
		z = this->z - right.z;

		return *this;
	}
};

/*一个面内的数据*/
struct Face 
{
	/*位置*/
	int position[3];
	/*法线*/
	int normal[3];
	/*纹理坐标*/
	int Texcoord[3];
};

/*模型读取类*/
class ObjReader
{
public:
	/*
	filename:模型存放路径
	*/
    ObjReader(const char* filename);
	~ObjReader();

	/*是否已经加载到内存*/
	bool IsLoad();

	/*顶点位置数据*/
	vector<Float3> vertexArray;
	/*纹理数据*/
	vector<Float3> texcoordArray;
	/*法线数据*/
	vector<Float3> normalArray;
	/*面的数据*/
	vector<Face> faceArray;
	
private:
    bool ObjLoadModel(const char* filename);

	bool isLoad;
};

