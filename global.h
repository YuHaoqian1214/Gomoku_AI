#pragma once
#include<cmath>
#include<string>
using namespace std;
typedef enum {
	isPlayerTurn = 0,
	isAiTurn = 1
} turn;//��¼�ֵ���һ�ai����
typedef enum {
	isWHITE = 1,
	isBLACK = 0
} pieceType;//�׺�Ϊ1���ں�Ϊ0
typedef enum {
	isBorder = 3,
	isEmpty = 2,
	isWPiece = 1,
	isBPiece = 0,
	isForbid = -1
} crossPointState;//��¼�����Ͻ����Ŀ���״̬ forbid�������
typedef enum {
	chessDown = 0,
	restartBtn = 1,
	saveBtn = 2,
	exitBtn = 3,
	illegal = 4,
	readBtn = 5,
	lastBtn = 6,
	nextBtn = 7
}mouseClick;
typedef enum {
	blackWin = 0,
	whiteWin = 1,
	draw = 2,
	notEnd = 3,
	review = 4,
	pause = 5
}matchResult;
struct chessPos {//���ӵ�λ��
	int col;//�����꣨1��1����ʼ
	int row;//������
	pieceType color;//���ӵ�״̬������ֻ�����кںͰף������ڿա����ֵ������
};
extern int boardSize;//���̴�С
extern const float scale;//����ϵͳ��ʾԭ����Ҫһ�����ų����������ڴ�С������Ϊ1����
extern const int windowWidth, windowHeight;//���ڴ�С
extern const int BoardWidth, BoardHeight;//������Ч��С
extern const int gapX[2], gapY[2];//��¼��һ�������������̶����������ľ��룬��λ������
extern int style;
extern const int gridSize, pieceSize;
extern const int gridSizeScaled, pieceSizeScaled;//һ�����̸�Ĵ�С��Ҳ��Լһ�����ӵĴ�С
extern const int iconSizeScaled;
extern const float gridSizeScaledF, pieceSizeScaledF;//��ȷ���Ӵ�С
extern const int startBtnL, startBtnR;//��ʼ��ť���ҽ�
extern const int startBtnT, startBtnB;//��ʼ��ť���½�
extern const int saveBtnL, saveBtnR;//���̰�ť���ҽ�
extern const int saveBtnT, saveBtnB;//���̰�ť���½�
extern const int readBtnL, readBtnR;//���̰�ť���ҽ�
extern const int readBtnT, readBtnB;//���̰�ť���½�
extern const int exitBtnL, exitBtnR;//�˳���ť���ҽ�
extern const int exitBtnT, exitBtnB;//�˳���ť���½�
extern turn currentTurn;//Ŀǰ����Ľ�ɫ
extern turn white, black;//����Ͱ���Ľ�ɫ
extern chessPos clickPos;//�ϴ������������
extern pieceType currentColor;//Ŀǰ�������ɫ
extern pieceType currentColor;//Ŀǰ�������ɫ
extern const string blackLong[1];//���峤��
extern const string blackFive[1];//��������
extern const string blackFour[5];//�������
extern const string blackSpecial[3];
extern const string blackFourA[1];//�������
extern const string blackFourD[9];//��������
extern const string blackThreeA[2];//����������
extern const string blackThreeJ[2];//����������
extern const string blackThreeS[1];//����ǻ���
extern const string blackThreeD[11];//��������
extern const string blackTwoA[6];//������
extern const string blackTwoD[12];//�����߶�
extern const string whiteFive[1];//��������
extern const string whiteFourA[1];//�������
extern const string whiteFourD[7];//��������
extern const string whiteThreeA[2];//����������
extern const string whiteThreeJ[2];//����������
extern const string whiteThreeD[9];//��������
extern const string whiteTwoA[6];//������
extern const string whiteTwoD[12];//�����߶�
extern matchResult result;//�����û�л��ʤ��
extern int steps;
extern const int limit;//ai�ļ������
extern int timeH, timeM, timeS;//��ʼ��ʱ��
extern const int blackbkxScaled, blackbkyScaled, whitebkxScaled, whitebkyScaled;//���ӱ������Ͻǵ�xy����
//extern int bkHScaled, bkWScaled;//���ӱ����Ĵ�С
extern const int timerbkSizeScaledH;
extern const int timerbkSizeScaledW;
extern const int nextBtnL, nextBtnT;
extern const int nextBtnR, nextBtnB;
extern const int lastBtnL, lastBtnT;
extern const int lastBtnR, lastBtnB;
extern bool isblack;