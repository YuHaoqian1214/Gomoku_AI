#pragma once
#include<cmath>
#include<string>
using namespace std;
typedef enum {
	isPlayerTurn = 0,
	isAiTurn = 1
} turn;//记录轮到玩家或ai行棋
typedef enum {
	isWHITE = 1,
	isBLACK = 0
} pieceType;//白恒为1，黑恒为0
typedef enum {
	isBorder = 3,
	isEmpty = 2,
	isWPiece = 1,
	isBPiece = 0,
	isForbid = -1
} crossPointState;//记录棋盘上交叉点的可能状态 forbid代表禁手
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
struct chessPos {//棋子的位置
	int col;//列坐标（1，1）开始
	int row;//行坐标
	pieceType color;//棋子的状态（这里只可能有黑和白，不存在空、禁手等情况）
};
extern int boardSize;//棋盘大小
extern const float scale;//由于系统显示原因，需要一个缩放常数调整窗口大小，正常为1即可
extern const int windowWidth, windowHeight;//窗口大小
extern const int BoardWidth, BoardHeight;//棋盘有效大小
extern const int gapX[2], gapY[2];//记录第一个坐标点距离棋盘顶部和左侧面的距离，单位是像素
extern int style;
extern const int gridSize, pieceSize;
extern const int gridSizeScaled, pieceSizeScaled;//一个棋盘格的大小，也是约一个棋子的大小
extern const int iconSizeScaled;
extern const float gridSizeScaledF, pieceSizeScaledF;//精确棋子大小
extern const int startBtnL, startBtnR;//开始按钮左右界
extern const int startBtnT, startBtnB;//开始按钮上下界
extern const int saveBtnL, saveBtnR;//存盘按钮左右界
extern const int saveBtnT, saveBtnB;//存盘按钮上下界
extern const int readBtnL, readBtnR;//读盘按钮左右界
extern const int readBtnT, readBtnB;//读盘按钮上下界
extern const int exitBtnL, exitBtnR;//退出按钮左右界
extern const int exitBtnT, exitBtnB;//退出按钮上下界
extern turn currentTurn;//目前行棋的角色
extern turn white, black;//黑棋和白棋的角色
extern chessPos clickPos;//上次鼠标点击的坐标
extern pieceType currentColor;//目前行棋的颜色
extern pieceType currentColor;//目前行棋的颜色
extern const string blackLong[1];//黑棋长连
extern const string blackFive[1];//黑棋连五
extern const string blackFour[5];//黑棋成四
extern const string blackSpecial[3];
extern const string blackFourA[1];//黑棋活四
extern const string blackFourD[9];//黑棋眠四
extern const string blackThreeA[2];//黑棋连活三
extern const string blackThreeJ[2];//黑棋跳活三
extern const string blackThreeS[1];//黑棋非活三
extern const string blackThreeD[11];//黑棋眠三
extern const string blackTwoA[6];//黑棋活二
extern const string blackTwoD[12];//黑棋眠二
extern const string whiteFive[1];//白棋连五
extern const string whiteFourA[1];//白棋活四
extern const string whiteFourD[7];//白棋眠四
extern const string whiteThreeA[2];//白棋连活三
extern const string whiteThreeJ[2];//白棋跳活三
extern const string whiteThreeD[9];//白棋眠三
extern const string whiteTwoA[6];//白棋活二
extern const string whiteTwoD[12];//白棋眠二
extern matchResult result;//检测有没有获得胜利
extern int steps;
extern const int limit;//ai的计算层数
extern int timeH, timeM, timeS;//初始的时间
extern const int blackbkxScaled, blackbkyScaled, whitebkxScaled, whitebkyScaled;//棋钟背景左上角的xy坐标
//extern int bkHScaled, bkWScaled;//棋钟背景的大小
extern const int timerbkSizeScaledH;
extern const int timerbkSizeScaledW;
extern const int nextBtnL, nextBtnT;
extern const int nextBtnR, nextBtnB;
extern const int lastBtnL, lastBtnT;
extern const int lastBtnR, lastBtnB;
extern bool isblack;