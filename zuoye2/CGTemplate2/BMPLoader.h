/*###################################################
##  姓名：江金昱
##  文件说明：CBMPLoader类的接口
##  功能：装载BMP文件到内存中
#####################################################*/

// 位图文件的标志
#define BITMAP_ID 0x4D42

class CBMPLoader
{
public:
    CBMPLoader();
    ~CBMPLoader();
    // 装载一个bmp文件
    bool Loadbitmap(const char *filename);
   // 纹理的ID号
    unsigned int ID;
    // 图像宽度
    int imageWidth;
    // 图像高度
    int imageHeight;
    // 指向图像数据的指针
    unsigned char *image;
    // 是否已加载
    bool isLoaded = false;

};

