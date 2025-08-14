#include "player.h"
#include<cmath>
#include<iostream>
using namespace std;
bool player::isValidClick(short x, short y)
{
	if (x > BoardWidth || y > BoardHeight) return false;//如果超过棋盘范围，直接判断为无效
	int col = (x - gapX[style]) / gridSizeScaled;
	int row = (y - gapY[style]) / gridSizeScaled;

	int leftTopPosX = gapX[style] + gridSizeScaled * col;
	int leftTopPosY = gapY[style] + gridSizeScaled * row;
	//std::cout << x << " " << y << " " << col << " " << row << " " << leftTopPosX << " " << leftTopPosY << std::endl;
	float offset = gridSizeScaled * 0.4; // 鼠标点击的模糊距离上限
	float len;
	int selectPos = false;
	do {
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			selectPos = true;
			break;
		}

		// 距离右上角的距离
		len = sqrt((x - leftTopPosX - gridSizeScaled) * (x - leftTopPosX - gridSizeScaled) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			selectPos = true;
			break;
		}

		// 距离左下角的距离
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - gridSizeScaled) * (y - leftTopPosY - gridSizeScaled));
		if (len < offset) {
			selectPos = true;
			break;
		}

		// 距离右下角的距离
		len = sqrt((x - leftTopPosX - gridSizeScaled) * (x - leftTopPosX - gridSizeScaled) + (y - leftTopPosY - gridSizeScaled) * (y - leftTopPosY - gridSizeScaled));
		if (len < offset) {
			selectPos = true;
			break;
		}
	} while (0);

	return selectPos;
}

mouseClick player::returnMouseClick(short x,short y)
{
	//cout << "msg detected:" << x << " " << y << endl;
	//先检测是否点击了按钮
	if (x > startBtnL && x<startBtnR && y<startBtnB && y > startBtnT) {
		//cout << "return mouse: restart" << endl;
		return restartBtn;
	}
	else if (x > saveBtnL && x<saveBtnR && y<saveBtnB && y > saveBtnT) {
		//cout << "return mouse: save" << endl;
		return saveBtn;
	}
	else if (x > readBtnL && x < readBtnR && y<readBtnB && y > readBtnT) {
		//cout << "return mouse: read" << endl;
		return readBtn;
	}
	else if (x > exitBtnL && x<exitBtnR && y<exitBtnB && y > exitBtnT) {
		//cout << "return mouse: exit" << endl;
		return exitBtn;
	}
	else if (x > lastBtnL && x < lastBtnR && y<lastBtnB && y > lastBtnT) {
		//cout << "return mouse: last" << endl;
		return lastBtn;
	}
	else if (x > nextBtnL && x < nextBtnR && y<nextBtnB && y > nextBtnT) {
		//cout << "return mouse: next" << endl;
		return nextBtn;
	}
	//随后检测是否是有效落子
	else if (isValidClick(x, y)) {
		return chessDown;
	}
	//将这段程序封装至走棋时
	//cout << "return mouse: illeagal" << endl;
	return illegal;
}

//chessPos player::go()
//{
//	MOUSEMSG msg;
//	while (1) {//时刻检测有无鼠标点击
//		msg = GetMouseMsg();
//		if (msg.uMsg == WM_LBUTTONDOWN && isValidClick(msg.x, msg.y)) {//如果按下左键,
//			//如果是有效点击（距离交叉点距离达到阈值）
//			int col = (round((msg.x - gapX) / gridSizeScaledF)) + 1;
//			int row = (round((msg.y - gapY) / gridSizeScaledF)) + 1;
//			std::cout << "player go:" << col << " " << row << endl;
//			return chessPos{ col,row,currentColor };//走棋之后返回，结束函数
//			/*if (control_.isValidClick(msg.x, msg.y)) {//时刻检测玩家鼠标位置，可以提示落子位置
//
//			}*/
//		}
//	}
//}