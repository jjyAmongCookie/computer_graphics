# 作业1（2019.10.12 ~ 2019.11.3 23:59）
使用自行编写的代码重现场景

在作业模板中的`scene_0()`函数使用了OpenGL函数（如，`glBegin(GL_LINE_STRIP)`，`glTranslatef()`及`glRotatef()`等）绘制一个三角形及一个四边形的场景。同学们需要在不使用以上函数的情况下重现同样场景（在模板中的`scene_1()`函数汇总实现）。

## 1.1 实现平移Translation及旋转Rotation变换
实现平移及旋转函数，计算点经过平移、旋转等变换后，于屏幕中所处的位置。

不能使用`glTranslate()`, `glRotate()`, `glMultMatrix()`等函数。
可选：通过矩阵乘法及四元数两种方式实现旋转。

## 1.2 实现直线绘制算法
使用OpenGL实现你的直线光栅化算法。

输入：两个二维平面上的点（由1.1中计算得到）。
输出：在屏幕上输出一条直线。
只能使用整数运算，并且只能使用GL_POINTS作为基本元素。
可选：使用抗锯齿算法或机制对绘制直线进行平滑。

## 1.3 其他注意事项
在报告中写下主要算法，说明多个变换之间的顺序关系对结果的影响，并附上运行结果截图。
`scene_1()` 函数中已经提供一个基本模板，用于对每个坐标对应的像素点着色。
`scene_0()` 及 `scene_1()` 的绘制可通过键盘的”0”,”1”按键进行切换。
在窗口resize过程中，需要保持 `scene_1()` 绘制结果与 `scene_0()` 一致。

## 1.4 Qt相关链接
Qt 5.13.0 下载：https://download.qt.io/official_releases/qt/5.13/5.13.0/

Qt镜像列表（exe for windows）：https://download.qt.io/archive/qt/5.13/5.13.0/qt-opensource-windows-x86-5.13.0.exe.mirrorlist

Qt .pro项目文件变量说明：https://doc.qt.io/qt-5/qmake-variable-reference.html
