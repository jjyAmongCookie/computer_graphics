#pragma once

#include <QtGui/qimage.h>
#include <stdlib.h>
#include <GL/glew.h>

/*图片读取工具*/
class ImageReader
{
public:
	ImageReader();
	/*
	filename:图片存放路径
	*/
    ImageReader(const char* filename);
	~ImageReader();

	/*是否已经加载到内存*/
	bool IsLoad();

	/*图片数据*/
	QImage image;
	/*绑定到Opengl的图片ID*/
	GLuint texID;

private:
	/*
	加载图片到内存
	filename:图片存放路径
	*/
    bool LoadTexture(const char* filename);

	/*是否已经加载到内存的标志*/
	bool isLoad;
};

