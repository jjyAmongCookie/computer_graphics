/*###################################################
##  姓名：江金昱
##  文件说明：CBMPLoader类的实现
##  功能：装载BMP文件到内存中
#####################################################*/

#include "BMPLoader.h"
#include <Windows.h>
#include <stdio.h>
#include<iostream>

/*###################################################
##  函数: CBMPLoader
##  函数描述： CBMPLoader类的构造函数
##  参数描述：无
#####################################################*/
CBMPLoader::CBMPLoader()
{
    imageWidth = 0;
    imageHeight = 0;
}

/*###################################################
##  函数: CBMPLoader
##  函数描述： CBMPLoader类的析构函数
##  参数描述：无
#####################################################*/
CBMPLoader::~CBMPLoader()
{

}

/*###################################################
##  函数: Loadbitmap
##  函数描述： 加载file所指向的纹理图片到内存中
##  参数描述：
##  fiLe: 想要加载的纹理图片的路径
#####################################################*/
bool CBMPLoader::Loadbitmap(const char *file)
{
    // 文件指针
    FILE *pFile;
    // 创建位图文件信息和位图文件头结构
    BITMAPINFOHEADER bitmapInfoHeader;
    BITMAPFILEHEADER header;
    // 用于将图像颜色从BGR变换到RGB
    unsigned char textureColors = 0;
    // 打开文件,并检查错误
    errno_t err = fopen_s(&pFile, file, "r");
    if( err != 0)
    {
        return false;
    }
    // 读入位图文件头信息
    fread(&header, sizeof(BITMAPFILEHEADER), 1, pFile);
    // 检查该文件是否为位图文件
    if(header.bfType != BITMAP_ID)
    {
        // 若不是位图文件,则关闭文件并返回
        fclose(pFile);
        return false;
    }
    // 读入位图文件信息
    fread(&bitmapInfoHeader, sizeof(BITMAPINFOHEADER), 1, pFile);
    // 保存图像的宽度和高度
    imageWidth = bitmapInfoHeader.biWidth;
    imageHeight = bitmapInfoHeader.biHeight;
    // 确保读取数据的大小
    if(bitmapInfoHeader.biSizeImage == 0)
        bitmapInfoHeader.biSizeImage = bitmapInfoHeader.biWidth *bitmapInfoHeader.biHeight * 3;
    // 将指针移到数据开始位置
    fseek(pFile, header.bfOffBits, SEEK_SET);
    // 分配内存
    image = new unsigned char[bitmapInfoHeader.biSizeImage];
    // 检查内存分配是否成功
    if(!image)
    {
        delete[] image;
        fclose(pFile);
        return false;
    }
    // 读取图像数据
    fread(image, 1, bitmapInfoHeader.biSizeImage, pFile);
    // 将图像颜色数据格式进行交换,由BGR转换为RGB
    for(int index = 0; index < (int)bitmapInfoHeader.biSizeImage; index+=3)
    {
        textureColors = image[index];
        image[index] = image[index + 2];
        image[index + 2] = textureColors;
    }
    // 关闭文件
    fclose(pFile);
    isLoaded = true;
    // 成功返回
    return true;
}
