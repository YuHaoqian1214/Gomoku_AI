#include"global.h";//��ȡȫ�ֱ���
#include<graphics.h>//����ͼ�ο�
#include"chessBoard.h"
#include"player.h"
#include"ai.h"
#include"Timer.h"
#include<iostream>
#include<Windows.h>
#include<vector>
#include<utility>
#include<thread>
#include<cstdlib>
#include<fstream>
#include<graphics.h>
#include<stdio.h>
#include<conio.h>
#include<mmsystem.h>//������ý��ӿ�
#pragma comment(lib,"winmm.lib")//����ӿ�
#define _CRT_SECURE_NO_WARNINGS 1
extern chessPos clickPos;
using namespace std;
vector<vector<vector<int>>> game;
int idx = 0;//�������̣�����main��processmouse������ʹ��
IMAGE img_Board;
Timer Timer1;
chessBoard chess_Board;
player player_;
ai ai_;
void putimagePNG(int x, int y, IMAGE* picture);
void processMouse();
void play();
void boardTimer();
int main() {
	if (style == 1) {
		loadimage(&img_Board, "res/board2.png", windowWidth, windowHeight);
	}
	else {
		loadimage(&img_Board, "res/board.png", windowWidth, windowHeight);
	}
	/*//����ȫ�ֱ����Ƿ���ȷ����
	cout << windowHeight << endl;
	cout << gridSize << endl;*/
	initgraph(windowWidth, windowHeight);//���һ�������̴�С��ͬ��ͼ�λ�����
	chess_Board.startMatch();//��ʼ��Ϸ
	//cout << "new game" << endl;
	thread timer(boardTimer);//Ϊ���Ӽ�ʱ
	timer.detach();
	thread mouse(processMouse);//���̳߳����������Ƿ�ͽ����еİ�ť����
	mouse.detach();
	//int is_ok = 0;
	////��ȡ���ھ��
	//HWND hnd = GetHWnd();
	////�������ڣ���ʾ�û�����
	//is_ok = MessageBox(hnd, "��⵽����浵���Ƿ�Ҫ�Ӹþ��濪ʼ�Ծ֣�", "������", MB_YESNO);
	//if (is_ok == IDYES) {
	//	initgraph(windowWidth, windowHeight);
	//	chess_Board.readSave();
	//}
	//else if (is_ok == IDNO) {

	//}
	int is_ok;//��ȡ���ھ��
	HWND hnd = GetHWnd();
	//�ļ�Ϊ.mp3Ϊ��׺�ſɲ��ţ�Ҫ�����ֿ�������Ŀ�����ļ�����

	mciSendString("open ./bgm.mp3", NULL, 0, NULL);  //������
	mciSendString("play ./bgm.mp3 repeat", NULL, 0, NULL);
	while (1) {//����������Ϸ
		if (result == notEnd) {
			while (result == notEnd) {
				switch (currentColor)//��������
				{
				case isWHITE://�ֵ�����
					if (white == isPlayerTurn) {//���ִ��
						currentTurn = isPlayerTurn;
					}
					else if (white == isAiTurn) {//aiִ��
						currentTurn = isAiTurn;
						clickPos = ai_.go(chess_Board.readBoard());
						if (result == notEnd) chess_Board.go(clickPos);
						currentColor = currentColor == isBLACK ? isWHITE : isBLACK;//������ɫ
					}
					break;
				case isBLACK://�ֵ�����
					if (black == isPlayerTurn) {//���ִ��
						currentTurn = isPlayerTurn;
					}
					else if (black == isAiTurn) {//aiִ��
						currentTurn = isAiTurn;
						do {
							clickPos = ai_.go(chess_Board.readBoard());
						} while (!chess_Board.isEmptyBoard(clickPos.col, clickPos.row));
						if (result == notEnd) chess_Board.go(clickPos);
						currentColor = currentColor == isBLACK ? isWHITE : isBLACK;//������ɫ
					}
					break;
				default:
					break;
				}
			}
		}
		else if (result == whiteWin) {
			is_ok = MessageBox(hnd, "��ϲ�����ʤ��", "������", MB_OK);
			ofstream outStream("res/game.txt", ios::app);
			outStream << -2 << " " << -2 << endl;//�������һ��
			outStream.close();
			result = review;
		}
		else if (result == blackWin) {
			is_ok = MessageBox(hnd, "��ϲ�����ʤ��", "������", MB_OK);
			ofstream outStream("res/game.txt", ios::app);
			outStream << -2 << " " << -2 << endl;//�������һ��
			outStream.close();
			result = review;
		}
		else if (result == draw) {
			is_ok = MessageBox(hnd, "˫������", "������", MB_OK);
			ofstream outStream("res/game.txt", ios::app);
			outStream << -2 << " " << -2 << endl;//�������һ��
			outStream.close();
			result = review;
		}
		else if (result == review) {
			IMAGE last, next;
			putimage(0, 0, &img_Board);
			if (style == 1) {
				loadimage(&last, "res/last.png", timerbkSizeScaledW, timerbkSizeScaledH);
				loadimage(&next, "res/next.png", timerbkSizeScaledW, timerbkSizeScaledH);
			}
			else {
				loadimage(&last, "res/last2.png", timerbkSizeScaledW, timerbkSizeScaledH);
				loadimage(&next, "res/next2.png", timerbkSizeScaledW, timerbkSizeScaledH);
			}
			putimage(blackbkxScaled, blackbkyScaled, &last);
			putimage(whitebkxScaled, whitebkyScaled, &next);
			idx = 0;
			vector<vector<int>> board(boardSize + 2, vector<int>(boardSize + 2, 2));
			game.clear();
			ifstream inStream("res/game.txt");
			int x, y;
			game.push_back(board);
			while (inStream >> x >> y) {
				if (x >= 0) {
					board[x][y] = (isblack ? 0 : 1);
					isblack = !isblack;
					game.push_back(board);
				}
				else if (x == -3) {
					for (int i = 1; i <= boardSize; i++) {
						for (int j = 1; j <= boardSize; j++) {
							inStream >> board[j][i];//�����棬����ý���ij���꣬��֪��Ϊʲô����������
						}
					}
					game.push_back(board);
				}
			}
			while (result == review) Sleep(1000);
		}
	}
	mouse.join();
	return 0;
}
void processMouse() {
	if (style == 1) {
		loadimage(&img_Board, "res/board2.png", windowWidth, windowHeight);
	}
	else {
		loadimage(&img_Board, "res/board.png", windowWidth, windowHeight);
	}
	IMAGE last, next;
	if (style == 1) {
		loadimage(&last, "res/last.png", timerbkSizeScaledW, timerbkSizeScaledH);
		loadimage(&next, "res/next.png", timerbkSizeScaledW, timerbkSizeScaledH);
	}
	else {
		loadimage(&last, "res/last2.png", timerbkSizeScaledW, timerbkSizeScaledH);
		loadimage(&next, "res/next2.png", timerbkSizeScaledW, timerbkSizeScaledH);
	}
	int is_ok;
	//��ȡ���ھ��
	HWND hnd = GetHWnd();
	//cout << "processMouse start" << endl;
	while (1) {//ʱ�̻�ȡ�����Ϣ
		//cout << "getting info" << endl;
		MOUSEMSG msgProcess = GetMouseMsg();
		if (msgProcess.uMsg == WM_LBUTTONDOWN) {//����������
			//cout << "Lbtn detected" << endl;
			mouseClick click = player_.returnMouseClick(msgProcess.x, msgProcess.y);
			switch (click)
			{
			case restartBtn:
				chess_Board.startMatch();
				break;
			case saveBtn:
				chess_Board.save();
				break;
			case readBtn:
				Timer1.bHour = timeH, Timer1.bMinute = timeM, Timer1.bSecond = timeS, Timer1.bmm = 0;
				Timer1.wHour = timeH, Timer1.wMinute = timeM, Timer1.wSecond = timeS, Timer1.wmm = 0;
				chess_Board.readSave();
				break;
			case exitBtn:
				chess_Board.leavegame();
				break;
			case chessDown:
				if (result == notEnd && currentTurn == isPlayerTurn) {//�ж��Ƿ��ֵ��������
					//cout << "player go" << endl;
					clickPos = chess_Board.transXYtoPos(msgProcess.x, msgProcess.y);
					if (chess_Board.isEmptyBoard(clickPos.col, clickPos.row)) {
						chess_Board.go(clickPos);
						currentColor = (currentColor == isBLACK ? isWHITE : isBLACK);//������ɫ
						//cout << "color switch" << endl;
					}
				}
				break;
			case nextBtn:
				if (result == review) {
					idx++;
					if (idx >= game.size()) {
						is_ok = MessageBox(hnd, "�Ѿ������һ��", "������", MB_OK);
						idx--;
					}
					else {
						putimage(0, 0, &img_Board);
						putimage(blackbkxScaled, blackbkyScaled, &last);
						putimage(whitebkxScaled, whitebkyScaled, &next);
						for (int i = 1; i <= boardSize; i++) {
							for (int j = 1; j <= boardSize; j++) {
								if (game[idx][i][j] == 0) {
									chess_Board.goWithoutJudge({ i,j,isBLACK });
								}
								else if (game[idx][i][j] == 1) {
									chess_Board.goWithoutJudge({ i,j,isWHITE });
								}
							}
						}
					}
				}
				break;
			case lastBtn:
				if (result == review) {
					idx--;
					if (idx < 0) {
						is_ok = MessageBox(hnd, "�Ѿ��ǵ�һ��", "������", MB_OK);
						idx++;
					}
					else {
						putimage(0, 0, &img_Board);
						putimage(blackbkxScaled, blackbkyScaled, &last);
						putimage(whitebkxScaled, whitebkyScaled, &next);
						for (int i = 1; i <= boardSize; i++) {
							for (int j = 1; j <= boardSize; j++) {
								if (game[idx][i][j] == 0) {
									chess_Board.goWithoutJudge({ i,j,isBLACK });
								}
								else if (game[idx][i][j] == 1) {
									chess_Board.goWithoutJudge({ i,j,isWHITE });
								}
							}
						}
					}
				}
				break;
			case illegal:
				break;
			default:
				break;
			}
		}
	}
	//cout << "processMouse end" << endl;//��Ӧ�õ�������
	return;
}
void boardTimer() {
	Timer1.refreshBlackTime();
	Timer1.refreshWhiteTime();
	// ��ʼ��ͼѭ��
	while (1)
	{
		while (result == notEnd) {
			if (currentColor == isBLACK) {
				Sleep(10);
				Timer1.bmm--;
				if (Timer1.bmm < 0) {
					Timer1.bmm = 99;
					Timer1.bSecond--;
					Timer1.refreshBlackTime();
					Timer1.refreshWhiteTime();
				}
				if (Timer1.bSecond < 0) {
					Timer1.bSecond = 59;
					Timer1.bMinute--;
					Timer1.refreshBlackTime();
				}
				if (Timer1.bMinute < 0) {
					Timer1.bMinute = 59;
					Timer1.bHour--;
					Timer1.refreshBlackTime();
				}
				if (Timer1.bHour <= 0 && Timer1.bMinute <= 0 && Timer1.bSecond <= 0) {
					result = whiteWin;
				}
			}
			if (currentColor == isWHITE) {
				Sleep(10);
				Timer1.wmm--;
				if (Timer1.wmm < 0) {
					Timer1.wmm = 99;
					Timer1.wSecond--;
					Timer1.refreshWhiteTime();
					Timer1.refreshBlackTime();
				}
				if (Timer1.wSecond < 0) {
					Timer1.wSecond = 59;
					Timer1.wMinute--;
					Timer1.refreshWhiteTime();
				}
				if (Timer1.wMinute < 0) {
					Timer1.wMinute = 59;
					Timer1.wHour--;
					Timer1.refreshWhiteTime();
				}
				if (Timer1.wHour <= 0 && Timer1.wMinute <= 0 && Timer1.wSecond <= 0) {
					result = blackWin;
				}
			}
		}
		Timer1.bHour = timeH, Timer1.bMinute = timeM, Timer1.bSecond = timeS;
		Timer1.wHour = timeH, Timer1.wMinute = timeM, Timer1.wSecond = timeS;
		Sleep(1000);
	}
}
