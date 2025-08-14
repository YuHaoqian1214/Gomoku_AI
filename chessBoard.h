#pragma once
#include<graphics.h>
#include<vector>
#include<iostream>
#include<fstream>
#include"global.h"
class chessBoard
{
public:
	void selectColor();//ѡ���˻���սʱ������ɫ
	void selectMethod();//ѡ�����˻�����˫�˶�ս
	void putImage();//��PNGͼ
	void startMatch();//��ʼ��Ϸ��ִ�и����ʼ��
	chessPos transXYtoPos(short x, short y);//���������ת��Ϊchesspos�ṹ
	void go(chessPos pos);//������������
	void goWithoutJudge(chessPos pos);//�����������ӣ������ж�ʤ��
	bool isEmptyBoard(int x, int y);//�ж�����λ���Ƿ�Ϊ��
	void save();//����
	void readSave();//����
	void leavegame();//�˳���Ϸ
	void printBoard();//���̵�ʱ������������������ʾÿ������
	void coutBoard(vector<vector<crossPointState>> boardInfo);//�ڿ���̨������̣���������
	void coutBoard();
	vector<vector<crossPointState>> readBoard();//���ڻ�ȡ����״̬
	void init();//��������ʱ�ĳ�ʼ��
	chessBoard();
	matchResult judge(chessPos pos, vector<vector<crossPointState>> boardInfo);//���̲���
	matchResult preciseJudge(chessPos pos, vector<vector<crossPointState>> boardInfo);//���Ѿ���׼ȷ��
	bool judgeForbid(chessPos pos, vector<vector<crossPointState>> boardInfo);//�Ѿ�����Ҫ
	bool preciseJudgeForbid(chessPos pos, vector<vector<crossPointState>>& boardInfo);//�Ѿ���׼ȷ
	void timer();
private:
	IMAGE img_Black;
	IMAGE img_White;
	IMAGE img_Board;
	IMAGE img_icon;
	vector<vector<crossPointState>> boardInfo;//�洢��������
	chessPos lastPos;
};


