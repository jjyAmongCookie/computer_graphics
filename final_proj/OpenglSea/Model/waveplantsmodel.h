/*###################################################
##  姓名: waveplantsmodel.h
##  文件说明：waveplantsmodel.cpp的头文件，定义类
#####################################################*/
#ifndef WAVEPLANTSMODEL_H
#define WAVEPLANTSMODEL_H

#pragma once
#include <BaseModel.h>

/*###################################################
##  类: WavePlantMode
##  类描述： 水草类，定义了水草的渲染相关函数，继承了BaseModel这个基类
#####################################################*/
class WavePlantMode : public BaseModel
{
public:
    /*
    position:模型初始化位置
    rotation:模型初始化旋转
    scale:模型缩放
    objdectId:模型ID号
    imgId:图片ID号
    waveHeight: 水草飘动的范围
    enableBlend：是否开启颜色混合（用于透明渲染
    enableTwoSide：是否开启双面渲染（用于片状物体
    */
    WavePlantMode(Float3 position, Float3 rotation, Float3 scale, int objectId, int imgId, float waveHeight = 0, bool enableBlend = false, bool enableTwoSide = false);
    ~WavePlantMode();

    /*
    重写基类渲染函数，用于渲染面片
    */
    virtual void Render(ObjReader& obj, ImageReader& image);

protected:
    /*每帧更新函数*/
    virtual void Tick();
    int time;
    float waveHeight;
};

#endif // WAVEPLANTSMODEL_H
