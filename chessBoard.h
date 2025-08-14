#pragma once
#include<graphics.h>
#include<vector>
#include<iostream>
#include<fstream>
#include"global.h"
class chessBoard
{
public:
	void selectColor();//选择人机对战时人类颜色
	void selectMethod();//选择是人机还是双人对战
	void putImage();//贴PNG图
	void startMatch();//开始游戏，执行各类初始化
	chessPos transXYtoPos(short x, short y);//把鼠标坐标转化为chesspos结构
	void go(chessPos pos);//在棋盘上落子
	void goWithoutJudge(chessPos pos);//在棋盘上落子，但不判断胜负
	bool isEmptyBoard(int x, int y);//判断落子位置是否为空
	void save();//存盘
	void readSave();//读盘
	void leavegame();//退出游戏
	void printBoard();//读盘的时候按照坐标在棋盘上显示每颗棋子
	void coutBoard(vector<vector<crossPointState>> boardInfo);//在控制台输出棋盘，用来调试
	void coutBoard();
	vector<vector<crossPointState>> readBoard();//用于获取棋盘状态
	void init();//启动程序时的初始化
	chessBoard();
	matchResult judge(chessPos pos, vector<vector<crossPointState>> boardInfo);//棋盘裁判
	matchResult preciseJudge(chessPos pos, vector<vector<crossPointState>> boardInfo);//（已经不准确）
	bool judgeForbid(chessPos pos, vector<vector<crossPointState>> boardInfo);//已经不需要
	bool preciseJudgeForbid(chessPos pos, vector<vector<crossPointState>>& boardInfo);//已经不准确
	void timer();
private:
	IMAGE img_Black;
	IMAGE img_White;
	IMAGE img_Board;
	IMAGE img_icon;
	vector<vector<crossPointState>> boardInfo;//存储棋盘数据
	chessPos lastPos;
};


