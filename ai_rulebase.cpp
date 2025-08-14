#include "ai.h"
#include<vector>
#include<random>
#include<iostream>
#include<string>
#include<utility>
chessPos ai::rulebase(int row, int col, int color, vector<vector<char>>& board)
{//color代表刚刚下过的颜色
	color = changeColor(color);
	int maxScore = 0, dx = 0, dy = 0;
	for (int x = 0; x < boardSize; x++) {
		for (int y = 0; y < boardSize; y++) {
			if (board[x][y] == '2'||board[x][y]=='-') {//如果能落子
				int score = calculate(x, y, color, 0, 1, board);
				if ((color == 0 && score == -1) || (color == 1 && score == -2)) {
					//cout << -1 << " " << x << " " << y << endl;
					return { x,y,colorToPos(color) };
				}
				if (score > maxScore) {
					maxScore = score;
					dx = x, dy = y;
				}
				else if (score == maxScore) {
					random_device rd;
					mt19937 gen(rd());
					uniform_int_distribution<> dis(0, 100);
					int ran = dis(gen) % 2;
					if (ran == 1) {
						dx = x, dy = y;
					}
				}
			}
		}
	}
	//cout << maxScore << " " << dx << " " << dy << endl;
	return { dx,dy,colorToPos(color) };
}
int ai::calculate(int row, int col, int color, int step, int score, vector<vector<char>> board)//step代表层数,score代表目前分数
{
	if (step >= limit) return score;//如果已经达到预定深度
	else {
		vector<vector<vector<string>>> allstr(boardSize, vector<vector<string>>(boardSize, vector<string>(4, "")));
		if (color == 1) {//上一步是白棋，这步ai执黑棋
			color = 0;//把颜色翻转
			board[row][col] = '0';//假设在该位置落黑子并计算
			refresh(row, col, color, board, allstr);//更新棋盘禁手，扫描所有点的字符串备用
			string strs[4] = { allstr[row][col][0],allstr[row][col][1],allstr[row][col][2] ,allstr[row][col][3] };
			if (checkWin(row, col, 0, board, strs)) {//先检测是否直接胜利
				return -1;//如果黑子直接获胜，落子
			}
			if (!checkAllow(row, col, 0, board)) {//检测是否落子到禁手上
				return -2;//如果落到禁手上，不能落子
			}
			board[row][col] = '1';//假设在该位置落白子并计算
			if (checkWin(row, col, 1, board, strs)) {//先检测是否直接胜利
				return -1;//如果白棋马上胜利，落子防守
			}
			board[row][col] = '0';//假设在该位置落黑子并计算

		}
		else {//执白棋
			color = 1;
			board[row][col] = '1';//假设在该位置落子并计算
			refresh(row, col, color, board, allstr);//更新棋盘禁手，扫描所有点的字符串备用
			for (int x = 0; x < boardSize; x++) {
				for (int y = 0; y < boardSize; y++) {
					string strs[4] = { allstr[x][y][0],allstr[x][y][1],allstr[x][y][2] ,allstr[x][y][3] };
					if (checkWin(x, y, color, board, strs)) {//先检测有无直接胜利
						return -2;
					}
				}
			}
		}
	}
}

bool ai::checkFourA(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//此局面轮到黑棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackFourA) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//最后白棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : whiteFourA) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
}

bool ai::checkFourD(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//此局面轮到黑棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackFourD) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//最后白棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : whiteFourD) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
}

bool ai::checkThreeA(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//此局面轮到黑棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackThreeA) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//最后白棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : whiteThreeA) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
}

bool ai::checkThreeJ(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//此局面轮到黑棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackThreeJ) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//最后白棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : whiteThreeJ) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
}

bool ai::checkThreeD(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//此局面轮到黑棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackThreeD) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//最后白棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : whiteThreeD) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
}

bool ai::checkTwoA(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//此局面轮到黑棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackTwoA) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//最后白棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : whiteTwoA) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
}

bool ai::checkTwoD(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//此局面轮到黑棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackTwoD) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//最后白棋落子
		for (int i = 0; i < 4; i++) {
			for (string str2 : whiteTwoD) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
}

bool ai::checkFourThree(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//此局面轮到黑棋落子
		bool check[4] = { false,false,false,false };//如果一条直线上已经有一个四，则不再检测三
		bool four = false, three = false;
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackFour) {//四四
				if (strs[i].find(str2) != string::npos) {
					check[i] = true;
					four = true;
					break;
				}
			}
			for (string str2 : blackThreeA) {//三三
				if (!check[i] && strs[i].find(str2) != string::npos) {
					three = true;
					break;
				}
			}
			if (four && three) return true;
			else return false;
		}
	}
	else if (color == 1) {//最后白棋落子
		bool check[4] = { false,false,false,false };//如果一条直线上已经有一个四，则不再检测三
		int four = 0, three = 0;
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackFour) {//四四
				if (strs[i].find(str2) != string::npos) {
					check[i] = true;
					four++;
					break;
				}
			}
			for (string str2 : blackThreeA) {//三三
				if (!check[i] && strs[i].find(str2) != string::npos) {
					three++;
					break;
				}
			}
			if (four + three >= 2) return true;//白棋双三，四三，四四都可算作四三获胜的一种
			else return false;
		}
	}
}

bool ai::checkForbid(int x, int y, int color, vector<vector<char>>& board)
{
	char ch = board[x][y];
	int col = y, row = x;
	int nx, ny, k;
	string strs[4];
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//四个方向，水平，竖直，正斜，反斜
	for (int l = 0; l < 4; l++) {//正方向
		k = 0;
		do {
			nx = row + dir[l][0] * k, ny = col + dir[l][1] * k;
			strs[l] = strs[l] + board[nx][ny];
			k += 1;
		} while (board[nx][ny] != isBorder && k <= 4);//反方向
		k = 1;
		do {
			nx = row - dir[l][0] * k, ny = col - dir[l][1] * k;
			strs[l] = board[nx][ny] + strs[l];
			k += 1;
		} while (board[nx][ny] != isBorder && k <= 4);
	}
	if (color == 0) {//此局面轮到黑棋落子
		//禁手判断	
		board[x][y] = '0';
		if (checkWin(x, y, color, board, strs)) {
			board[x][y] = ch;
			return false;//如果黑棋能够成五，则不认为此处是禁手
		}
		else {
			int four = 0, three = 0;
			bool six[4] = { false,false,false,false }, five = false, si = false;
			bool check[4] = { false,false,false,false };//如果一条直线上已经有一个四，则不再检测三
			for (int i = 0; i < 4; i++) {//长连
				for (string str2 : blackLong) {
					if (strs[i].find(str2) != string::npos) {
						board[x][y] = ch;
						return true;
					}
				}
				for (string str2 : blackSpecial) {//四四
					if (strs[i].find(str2) != string::npos) {
						board[x][y] = ch;
						return true;
					}
				}
				for (string str2 : blackFour) {//四四
					if (strs[i].find(str2) != string::npos) {
						four++;
						check[i] = true;
						if (four >= 2) {
							board[x][y] = ch;
							return true;
						}
						break;
					}
				}
				for (string str2 : blackThreeS) {
					if (strs[i].find(str2) != string::npos) {
						check[i] = true;
						break;
					}
				}
				for (string str2 : blackThreeA) {
					if (check[i] == false && strs[i].find(str2) != string::npos) {//连活三，递归两个端点是不是禁手
						//找到两个端点
						int idx1, idx2;
						int po = strs[i].find("000");
						if (po != string::npos) {
							idx1 = (po - 1) - 4 + (9 - strs[i].length()), idx2 = (po + 3) - 4 + (9 - strs[i].length());
						}
						int idx3 = idx1 - 1, idx4 = idx2 + 1;
						int nx1 = x + dir[i][0] * idx1, ny1 = y + dir[i][1] * idx1;
						int nx2 = x + dir[i][0] * idx2, ny2 = y + dir[i][1] * idx2;
						int nx3 = x + dir[i][0] * idx3, ny3 = y + dir[i][1] * idx3;
						int nx4 = x + dir[i][0] * idx4, ny4 = y + dir[i][1] * idx4;
						//cout << "nx1:" << nx1 << " " << ny1 << endl;
						//cout << nx2 << " " << ny2 << endl;
						//cout << idx1 << " " << idx2 << endl;
						//cout << idx3 << " " << idx4 << endl;
						//cout << "board:" << boardInfo[nx3][ny3] << endl;
						//cout << boardInfo[nx4][ny4] << endl;
						if (board[nx3][ny3] != isEmpty) {
							//cout << 1 << endl;
							if (checkForbid( ny2,nx2,0, board) != whiteWin) three++;
						}
						else if (board[nx4][ny4] != isEmpty) {
							//cout << 2 << endl;
							if (checkForbid( ny1,nx1,0,board) != whiteWin) three++;
						}
						else if (checkForbid( ny1,nx1,0, board) != whiteWin || checkForbid( ny2,nx2,0, board) != whiteWin) {
							//cout << 3 << endl;
							three++;
						}
						break;
					}
				}
				for (string str2 : blackThreeJ) {
					if (check[i] == false && strs[i].find(str2) != string::npos) {//跳活三，递归一个中间点是不是禁手
						int idx;
						int po = strs[i].find("0020");
						if (po != string::npos) {
							idx = (po + 2) - 4 + (9 - strs[i].length());
						}
						po = strs[i].find("0200");
						if (po != string::npos) {
							idx = (po + 1) - 4 + (9 - strs[i].length());
						}
						int nx5 = x + dir[i][0] * idx, ny5 = y + dir[i][1] * idx;
						//cout << nx5 << " " << ny5 << endl;
						//cout << idx << endl;
						if (checkForbid( ny5,nx5,0, board) != whiteWin) three++;
						break;
					}
				}
				if (three >= 2) {
					board[x][y] = ch;
					return true;
				}
			}
			board[x][y] = ch;
			return false;
		}
	}
	else if (color == 1) {//最后白棋落子
		return false;
	}
}

bool ai::checkWin(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//如果是黑棋，先检测一个方向是不是长连
		bool si = false, five = false;
		bool six[4] = { false,false,false,false };
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackLong) {
				if (strs[i].find(str2) != string::npos) {
					six[i] = true;
					si = true;
				}
			}
			for (string str2 : blackLong) {
				if (!six[i] && strs[i].find(str2) != string::npos) {
					five = true;
				}
			}
		}
		if (five) return true;
		else return false;
	}
	else {//如果是白棋，直接检测是否成五即可
		for (int i = 0; i < 4; i++) {
			for (string str2 : whiteFive) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
		}
		return false;
	}
}
bool ai::checkAllow(int x, int y, int color, vector<vector<char>>& board)//查看此处是否能落子
{
	if (board[x][y] == '2') {//如果已经有子，或者是禁手
		return true;
	}
	else {
		return false;
	}
}

void ai::refresh(int row, int col, int color, vector<vector<char>>& board, vector<vector<vector<string>>>& allstr)
{
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//四个方向，水平，竖直，正斜，反斜
	int nx = row, ny = col, k = 0, m = 0;
	//更新禁手情况
	for (int l = 0; l < 4; l++) {
		//正方向
		k = 0, m = 0;
		while (0 <= nx && nx < boardSize && 0 <= ny && ny < boardSize && k <= 4) {
			nx = row + dir[l][0] * k, ny = col + dir[l][1] * k;
			int nx1 = row, ny1 = col;
			string strs[4];
			for (int l = 0; l < 4; l++) {
				//正方向
				m = 0;
				while (0 <= nx1 && nx1 < boardSize && 0 <= ny1 && ny1 < boardSize && k <= 4) {
					nx1 = row + dir[l][0] * k, ny1 = col + dir[l][1] * k;
					strs[l] = strs[l] + board[nx1][ny1];
					m += 1;
				}
				//反方向
				m = 1;
				while (0 <= nx1 && nx1 < boardSize && 0 <= ny1 && ny1 < boardSize && k <= 4) {
					nx1 = row - dir[l][0] * k, ny1 = col - dir[l][1] * k;
					k += 1;
				}
			}
			if (checkForbid(nx1, ny1, color, board)) {
				board[nx1][ny1] = '-';
			}
			k += 1;
		}
		//反方向
		k = 1, m = 0;
		while (0 <= nx && nx < boardSize && 0 <= ny && ny < boardSize && k <= 4) {
			nx = row + dir[l][0] * k, ny = col + dir[l][1] * k;
			int nx1 = row, ny1 = col;
			string strs[4];
			for (int l = 0; l < 4; l++) {
				//正方向
				m = 0;
				while (0 <= nx1 && nx1 < boardSize && 0 <= ny1 && ny1 < boardSize && k <= 4) {
					nx1 = row + dir[l][0] * k, ny1 = col + dir[l][1] * k;
					strs[l] = strs[l] + board[nx1][ny1];
					m += 1;
				}
				//反方向
				m = 1;
				while (0 <= nx1 && nx1 < boardSize && 0 <= ny1 && ny1 < boardSize && k <= 4) {
					nx1 = row - dir[l][0] * k, ny1 = col - dir[l][1] * k;
					k += 1;
				}
			}
			if (checkForbid(nx1, ny1, color, board)) {
				board[nx1][ny1] = '-';
			}
			k += 1;
		}
	}
	nx = row, ny = col;
	for (int x = 0; x < boardSize; x++) {
		for (int y = 0; y < boardSize; y++) {
			//遍历棋盘,扫描所有的字符串备用
			for (int l = 0; l < 4; l++) {
				//正方向
				k = 0;
				while (0 <= nx && nx < boardSize && 0 <= ny && ny < boardSize && k <= 4) {
					nx = x + dir[l][0] * k, ny = y + dir[l][1] * k;
					allstr[x][y][l] = allstr[x][y][l] + board[nx][ny];

					k += 1;
				}
				//反方向
				k = 1;
				while (0 <= nx && nx < boardSize && 0 <= ny && ny < boardSize && k <= 4) {
					nx = x - dir[l][0] * k, ny = y - dir[l][1] * k;
					allstr[x][y][l] = board[nx][ny] + allstr[x][y][l];
					k += 1;
				}
			}
		}
	}
}

void ai::transBoardToint(vector<vector<crossPointState>>& boardInfo)
{
	for (int i = 1; i <= boardSize; i++) {
		for (int j = 1; j <= boardSize; j++) {
			switch (boardInfo[i][j]) {
			case isEmpty:
				board[i - 1][j - 1] = '2';
				break;
			case isWHITE:
				board[i - 1][j - 1] = '1';
				break;
			case isBLACK:
				board[i - 1][j - 1] = '0';
				break;
			case isForbid:
				board[i - 1][j - 1] = '-';
				break;
			default:
				cout << "fail trans to qz" << endl;
				break;
			}
		}
	}
	return;
}

int ai::changeColor(int color)
{
	if (color == 0) return 1;
	else return 0;
}

pieceType ai::colorToPos(int color)
{
	switch (color) {
	case 0:
		return isBLACK;
		break;
	case 1:
		return isWHITE;
		break;
	default:
		cout << "error: piecetype trans" << endl;
	}
}

ai::ai() {
	allstr.assign(boardSize, vector<vector<string>>(boardSize, vector<string>(4, "")));
	board.assign(boardSize, vector<char>(boardSize, '0'));
	possible.assign(boardSize, vector<bool>(boardSize, false));
}