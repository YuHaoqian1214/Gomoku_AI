#include "player.h"
#include<cmath>
#include<iostream>
using namespace std;
bool player::isValidClick(short x, short y)
{
	if (x > BoardWidth || y > BoardHeight) return false;//����������̷�Χ��ֱ���ж�Ϊ��Ч
	int col = (x - gapX[style]) / gridSizeScaled;
	int row = (y - gapY[style]) / gridSizeScaled;

	int leftTopPosX = gapX[style] + gridSizeScaled * col;
	int leftTopPosY = gapY[style] + gridSizeScaled * row;
	//std::cout << x << " " << y << " " << col << " " << row << " " << leftTopPosX << " " << leftTopPosY << std::endl;
	float offset = gridSizeScaled * 0.4; // �������ģ����������
	float len;
	int selectPos = false;
	do {
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			selectPos = true;
			break;
		}

		// �������Ͻǵľ���
		len = sqrt((x - leftTopPosX - gridSizeScaled) * (x - leftTopPosX - gridSizeScaled) + (y - leftTopPosY) * (y - leftTopPosY));
		if (len < offset) {
			selectPos = true;
			break;
		}

		// �������½ǵľ���
		len = sqrt((x - leftTopPosX) * (x - leftTopPosX) + (y - leftTopPosY - gridSizeScaled) * (y - leftTopPosY - gridSizeScaled));
		if (len < offset) {
			selectPos = true;
			break;
		}

		// �������½ǵľ���
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
	//�ȼ���Ƿ����˰�ť
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
	//������Ƿ�����Ч����
	else if (isValidClick(x, y)) {
		return chessDown;
	}
	//����γ����װ������ʱ
	//cout << "return mouse: illeagal" << endl;
	return illegal;
}

//chessPos player::go()
//{
//	MOUSEMSG msg;
//	while (1) {//ʱ�̼�����������
//		msg = GetMouseMsg();
//		if (msg.uMsg == WM_LBUTTONDOWN && isValidClick(msg.x, msg.y)) {//����������,
//			//�������Ч��������뽻������ﵽ��ֵ��
//			int col = (round((msg.x - gapX) / gridSizeScaledF)) + 1;
//			int row = (round((msg.y - gapY) / gridSizeScaledF)) + 1;
//			std::cout << "player go:" << col << " " << row << endl;
//			return chessPos{ col,row,currentColor };//����֮�󷵻أ���������
//			/*if (control_.isValidClick(msg.x, msg.y)) {//ʱ�̼��������λ�ã�������ʾ����λ��
//
//			}*/
//		}
//	}
//}