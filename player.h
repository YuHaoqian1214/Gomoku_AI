#pragma once
#include<graphics.h>
#include"global.h"
class player
{
public:
	bool isValidClick (short x, short y);//检测玩家点击是否有效
	mouseClick returnMouseClick (short x,short y);
	//chessPos go();//只发射信号，不控制棋盘
};

