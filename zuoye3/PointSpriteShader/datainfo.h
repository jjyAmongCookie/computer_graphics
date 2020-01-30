/*###################################################
##  姓名：江金昱
##  文件说明：实现Float3
##  用于存储三维向量，可获取向量长度
#####################################################*/
#pragma once

#include <stdlib.h>
#include <math.h>

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
};
