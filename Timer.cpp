#include "Timer.h"
#include <thread>
#include <iostream>
#include<string>
#include <graphics.h>
#include<easyx.h>
// 在倒计时中显示小时
using namespace std;

Timer::Timer()
{
	if (style == 1) {
		loadimage(&last, "res/blackbk.png", timerbkSizeScaledW, timerbkSizeScaledH);
		loadimage(&next, "res/whitebk.png", timerbkSizeScaledW, timerbkSizeScaledH);
	}
	else {
		loadimage(&last, "res/blackbk2.png", timerbkSizeScaledW, timerbkSizeScaledH);
		loadimage(&next, "res/whitebk2.png", timerbkSizeScaledW, timerbkSizeScaledH);
	}
}

void Timer::refreshBlackTime()
{
	settextcolor(BLACK);//文字格式为红色
	settextstyle(48, 0, "楷体");//文字的大小，紧凑程度，字体的样式
	setbkmode(TRANSPARENT);
	putimage(blackbkxScaled, blackbkyScaled, &last);
	char h[3] = "", m[3] = "", s[3] = "";
	sprintf_s(h, 3, "%d", bHour);
	sprintf_s(m, 3, "%d", bMinute);
	sprintf_s(s, 3, "%d", bSecond);
	outtextxy(HxScaled, blackyScaled, h);
	outtextxy(colonHScaled, blackyScaled, ":");
	outtextxy(MxScaled, blackyScaled, m);
	outtextxy(colonMScaled, blackyScaled, ":");
	outtextxy(SxScaled, blackyScaled, s);
}
void Timer::refreshWhiteTime()
{
	settextcolor(BLACK);//文字格式为红色
	settextstyle(48, 0, "楷体");//文字的大小，紧凑程度，字体的样式
	setbkmode(TRANSPARENT);
	putimage(whitebkxScaled, whitebkyScaled, &next);
	char h[3] = "00", m[3] = "00", s[3] = "00";
	sprintf_s(h, 3, "%d", wHour);
	sprintf_s(m, 3, "%d", wMinute);
	sprintf_s(s, 3, "%d", wSecond);
	outtextxy(HxScaled, whiteyScaled, h);
	outtextxy(colonHScaled, whiteyScaled, ":");
	outtextxy(MxScaled, whiteyScaled, m);
	outtextxy(colonMScaled, whiteyScaled, ":");
	outtextxy(SxScaled, whiteyScaled, s);
}
