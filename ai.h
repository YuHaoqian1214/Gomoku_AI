#pragma once
#include"global.h"
#include"ai_jxk.h"
#include<vector>
#include<utility>
class ai
{
public:
	chessPos go(vector<vector<crossPointState>> boardInfo)
	{
		// if (white == isAiTurn) {
			 ////lyx  ai算法
			 //transBoardToqz(boardInfo);
			 ////cout << "before ai" << endl;
			 //chessPos pos = lyxai(qz, 1, 2, boardSize);
			 ////cout << "after ai" << endl;
			 //return pos;
		 //}
		 //else if (black == isAiTurn) {
		 //    //随机落子算法
		 //    random_device rd;
		 //    uniform_int_distribution<>	num(1, boardSize);
		 //    int px, py;
		 //    px = num(rd);
		 //    py = num(rd);
		 //    //std::cout << px << " " << py;
		 //    return chessPos{ px,py,currentColor };//走棋之后返回，结束函数
		 //}
		 //lyxaiV2
		/*transBoardToqz(boardInfo);
		chessPos res = lyxai(qz, 1, 1, boardSize);
		return res;*/
		ai_jxk jxk;
		jxk.inputFromGame(boardInfo);
		pair<int, int> in = jxk.evaluate();
		return { in.second,in.first,currentColor };
	}
	void transBoardToqz(vector<vector<crossPointState>> boardInfo);
	ai();
private:
	chessPos lyxai(int qz[20][20], int p1, int p2, int n);
	bool legalind(int ind1, int ind2, int n);
private:
	bool legalindv2(int ind1, int ind2, int n);
	int lyxaiv2(int qz[20][20], int n, bool IsAIWhite, int count);
	int have_anyone_won(int qz[20][20], int n);
	bool check_long_lian(int qz_copy[20][20], int n, int pi, int pj);
	int check_four(int qz_copy[20][20], int n, int pi, int pj);
	int check_live_three(int qz_copy[20][20], int n, int pi, int pj);
	int check_banned_hand(int qz_copy[20][20], int n, int pi, int pj);
	int check34(int qz_copy[20][20], int n, int pi, int pj);
private:
	chessPos rulebase(int row, int col, int color, vector<vector<char>>& board);//传入最后一个棋子落子点和颜色，ai要下相反的颜色，1为白0为黑
	int calculate(int row, int col, int color, int step, int score, vector<vector<char>> board);
	bool checkFourA(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4]);
	bool checkFourD(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4]);
	bool checkThreeA(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4]);
	bool checkThreeJ(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4]);
	bool checkThreeD(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4]);
	bool checkTwoA(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4]);
	bool checkTwoD(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4]);
	bool checkFourThree(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4]);
	bool checkWin(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4]);	
	bool checkAllow(int x, int y, int color, vector<vector<char>>& board);
//public:
	bool checkForbid(int x, int y, int color, vector<vector<char>>& board);
	void refresh(int row, int col, int color, vector<vector<char>>& board, vector<vector<vector<string>>>& allstr);
	void transBoardToint(vector<vector<crossPointState>>& boardInfo);
	int changeColor(int color);
	pieceType colorToPos(int color);
private:
	int qz[20][20];
private:
//public:
	vector<vector<char>> board;
	vector<vector<vector<string>>> allstr;
	vector<vector<bool>> possible;
};

