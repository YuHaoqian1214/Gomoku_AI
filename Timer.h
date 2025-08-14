#pragma once
#include <easyx.h>
#include<cmath>
#include"global.h"
// 时间类处理倒计时操作
class Timer
{
public:
    Timer();
    void refreshBlackTime();
    void refreshWhiteTime();
public:
    int bHour = timeH, bMinute = timeM, bSecond = timeS, bmm = 0;
    int wHour = timeH, wMinute = timeM, wSecond = timeS, wmm = 0;
    //bool blackOn = false, whiteOn = false;
private:
    IMAGE last, next;
    //const int Hx = 1598, colonH = 1638, Mx = 1696, colonM = 1769, Sx = 1827, sy = 114514;
    const int HxScaled = round(1598 * scale), colonHScaled = round(1638 * scale), MxScaled = round(1680 * scale),
        colonMScaled = round(1769 * scale), SxScaled = round(1815 * scale);
    //const int whitey = 1255, blacky = 184;
    const int whiteyScaled = round(1225 * scale), blackyScaled = round(155 * scale);
    const int textSizeScaled = round(48 * scale);
};


