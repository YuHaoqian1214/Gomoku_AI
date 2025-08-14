#include"global.h";//获取全局变量
#include<graphics.h>//引入图形库
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
#include<mmsystem.h>//包含多媒体接口
#pragma comment(lib,"winmm.lib")//处理接口
#define _CRT_SECURE_NO_WARNINGS 1
extern chessPos clickPos;
using namespace std;
vector<vector<vector<int>>> game;
int idx = 0;//用来复盘，会在main和processmouse函数中使用
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
	/*//测试全局变量是否被正确引用
	cout << windowHeight << endl;
	cout << gridSize << endl;*/
	initgraph(windowWidth, windowHeight);//输出一个和棋盘大小相同的图形化窗口
	chess_Board.startMatch();//开始游戏
	//cout << "new game" << endl;
	thread timer(boardTimer);//为棋钟计时
	timer.detach();
	thread mouse(processMouse);//多线程持续检测鼠标是否和界面中的按钮互动
	mouse.detach();
	//int is_ok = 0;
	////获取窗口句柄
	//HWND hnd = GetHWnd();
	////弹出窗口，提示用户操作
	//is_ok = MessageBox(hnd, "检测到局面存档，是否要从该局面开始对局？", "五子棋", MB_YESNO);
	//if (is_ok == IDYES) {
	//	initgraph(windowWidth, windowHeight);
	//	chess_Board.readSave();
	//}
	//else if (is_ok == IDNO) {

	//}
	int is_ok;//获取窗口句柄
	HWND hnd = GetHWnd();
	//文件为.mp3为后缀才可播放，要把音乐拷贝到项目所在文件夹里

	mciSendString("open ./bgm.mp3", NULL, 0, NULL);  //打开音乐
	mciSendString("play ./bgm.mp3 repeat", NULL, 0, NULL);
	while (1) {//持续进行游戏
		if (result == notEnd) {
			while (result == notEnd) {
				switch (currentColor)//轮流走子
				{
				case isWHITE://轮到白棋
					if (white == isPlayerTurn) {//玩家执白
						currentTurn = isPlayerTurn;
					}
					else if (white == isAiTurn) {//ai执白
						currentTurn = isAiTurn;
						clickPos = ai_.go(chess_Board.readBoard());
						if (result == notEnd) chess_Board.go(clickPos);
						currentColor = currentColor == isBLACK ? isWHITE : isBLACK;//交换颜色
					}
					break;
				case isBLACK://轮到黑棋
					if (black == isPlayerTurn) {//玩家执黑
						currentTurn = isPlayerTurn;
					}
					else if (black == isAiTurn) {//ai执黑
						currentTurn = isAiTurn;
						do {
							clickPos = ai_.go(chess_Board.readBoard());
						} while (!chess_Board.isEmptyBoard(clickPos.col, clickPos.row));
						if (result == notEnd) chess_Board.go(clickPos);
						currentColor = currentColor == isBLACK ? isWHITE : isBLACK;//交换颜色
					}
					break;
				default:
					break;
				}
			}
		}
		else if (result == whiteWin) {
			is_ok = MessageBox(hnd, "恭喜白棋获胜！", "五子棋", MB_OK);
			ofstream outStream("res/game.txt", ios::app);
			outStream << -2 << " " << -2 << endl;//输入最后一步
			outStream.close();
			result = review;
		}
		else if (result == blackWin) {
			is_ok = MessageBox(hnd, "恭喜黑棋获胜！", "五子棋", MB_OK);
			ofstream outStream("res/game.txt", ios::app);
			outStream << -2 << " " << -2 << endl;//输入最后一步
			outStream.close();
			result = review;
		}
		else if (result == draw) {
			is_ok = MessageBox(hnd, "双方和棋", "五子棋", MB_OK);
			ofstream outStream("res/game.txt", ios::app);
			outStream << -2 << " " << -2 << endl;//输入最后一步
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
							inStream >> board[j][i];//很神奇，这里得交换ij坐标，不知道为什么，但是能跑
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
	//获取窗口句柄
	HWND hnd = GetHWnd();
	//cout << "processMouse start" << endl;
	while (1) {//时刻获取鼠标信息
		//cout << "getting info" << endl;
		MOUSEMSG msgProcess = GetMouseMsg();
		if (msgProcess.uMsg == WM_LBUTTONDOWN) {//如果按下左键
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
				if (result == notEnd && currentTurn == isPlayerTurn) {//判断是否轮到玩家走棋
					//cout << "player go" << endl;
					clickPos = chess_Board.transXYtoPos(msgProcess.x, msgProcess.y);
					if (chess_Board.isEmptyBoard(clickPos.col, clickPos.row)) {
						chess_Board.go(clickPos);
						currentColor = (currentColor == isBLACK ? isWHITE : isBLACK);//交换颜色
						//cout << "color switch" << endl;
					}
				}
				break;
			case nextBtn:
				if (result == review) {
					idx++;
					if (idx >= game.size()) {
						is_ok = MessageBox(hnd, "已经是最后一步", "五子棋", MB_OK);
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
						is_ok = MessageBox(hnd, "已经是第一步", "五子棋", MB_OK);
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
	//cout << "processMouse end" << endl;//不应该到达这里
	return;
}
void boardTimer() {
	Timer1.refreshBlackTime();
	Timer1.refreshWhiteTime();
	// 开始绘图循环
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
