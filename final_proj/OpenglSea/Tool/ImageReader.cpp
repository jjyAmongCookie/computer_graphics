#include <iostream>
#include <Tool/ImageReader.h>
#include <QtCore/qdir.h>

using namespace std;

ImageReader::ImageReader(const char* filename)
{
	isLoad = false;

	LoadTexture(filename);
}


ImageReader::ImageReader()
{
	isLoad = false;
}

ImageReader::~ImageReader()
{
}


bool ImageReader::LoadTexture(const char* filename)
{
    // 转化成绝对路径
    string dir = QDir::currentPath().toStdString() + "/" + filename;
    const char* s = dir.c_str();

    if (!image.load(s, "PNG"))
	{
        cout << "image : open file failed" << endl;
		return false;
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	image = image.convertToFormat(QImage::Format_RGBA8888);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());

	cout << "load image: " << filename << endl;

	isLoad = true;
	return true;
}



bool ImageReader::IsLoad()
{
	return isLoad;
}
