#include"chessBoard.h"
#include<string>
#include<iostream>
using namespace std;
// 定义将 crossPointState 转换为 char 的函数
char crossPointStateToChar(crossPointState state) {
	switch (state) {
	case isBorder:
		return '3';
	case isEmpty:
		return '2';
	case isWPiece:
		return '1';
	case isBPiece:
		return '0';
	case isForbid:
		return '-';
	default:
		throw std::invalid_argument("Invalid crossPointState for character conversion");
	}
}
matchResult chessBoard::judge(chessPos pos, vector<vector<crossPointState>> boardInfo)
{
	int col = pos.col, row = pos.row;
	if (pos.color == isBLACK) {
		boardInfo[row][col] = isBPiece;
	}
	else boardInfo[row][col] = isWPiece;
	//coutBoard(boardInfo);
	//cout << row << " " << col << " " << endl;
	char ch = crossPointStateToChar(boardInfo[row][col]);
	string board[4];//以最后落子点为中心，向四个方向扫描长度为9的四个字符串
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//四个方向，水平，竖直，正斜，反斜
	int nx, ny, k;
	for (int l = 0; l < 4; l++) {//正方向
		k = 0;
		do {
			nx = row + dir[l][0] * k, ny = col + dir[l][1] * k;
			board[l] = board[l] + crossPointStateToChar(boardInfo[nx][ny]);
			k += 1;
		} while (boardInfo[nx][ny] != isBorder && k <= 4);//反方向
		while (k <= 4) {
			board[l] += '3';
			k += 1;
		}
		k = 1;
		do {
			nx = row - dir[l][0] * k, ny = col - dir[l][1] * k;
			board[l] = crossPointStateToChar(boardInfo[nx][ny]) + board[l];
			k += 1;
		} while (boardInfo[nx][ny] != isBorder && k <= 4);
		while (k <= 4) {
			board[l] = '3' + board[l];
			k += 1;
		}
	}
	int is_ok;//获取窗口句柄
	HWND hnd = GetHWnd();
	if (pos.color == isBLACK) {//最后黑棋落子
		int four = 0, three = 0;
		bool six[4] = { false,false,false,false }, five = false, si = false;
		bool check[4] = { false,false,false,false };//如果一条直线上已经有一个四，则不再检测三
		//初步禁手判断
		for (int i = 0; i < 4; i++) {//先检测有没有长连
			for (string str2 : blackLong) {
				if (board[i].find(str2) != string::npos) {
					six[i] = true;
					si = true;
				}
				break;
			}
		}
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackFive) {
				if (six[i] == false && board[i].find(str2) != string::npos) {
					five = true;
					break;
				}
			}
			for (string str2 : blackSpecial) {
				if (board[i].find(str2) != string::npos) {
					four += 2;
					break;
				}
			}
			for (string str2 : blackFour) {
				if (board[i].find(str2) != string::npos) {
					four++;
					check[i] = true;
					break;
				}
			}
			for (string str2 : blackThreeS) {
				if (board[i].find(str2) != string::npos) {
					check[i] = true;
					break;
				}
			}
			for (string str2 : blackThreeA) {
				if (check[i] == false && board[i].find(str2) != string::npos) {//连活三，递归两个端点是不是禁手
					//找到两个端点
					int idx1, idx2;
					int po = board[i].find("000");
					if (po != string::npos) {
						idx1 = (po - 1) - 4, idx2 = (po + 3) - 4;
					}
					int idx3 = idx1 - 1, idx4 = idx2 + 1;
					int nx1 = row + dir[i][0] * idx1, ny1 = col + dir[i][1] * idx1;
					int nx2 = row + dir[i][0] * idx2, ny2 = col + dir[i][1] * idx2;
					int nx3 = row + dir[i][0] * idx3, ny3 = col + dir[i][1] * idx3;
					int nx4 = row + dir[i][0] * idx4, ny4 = col + dir[i][1] * idx4;
					//cout << "nx1:" << nx1 << " " << ny1 << endl;
					//cout << nx2 << " " << ny2 << endl;
					//cout << idx1 << " " << idx2 << endl;
					//cout << idx3 << " " << idx4 << endl;
					//cout << "board:" << boardInfo[nx3][ny3] << endl;
					//cout << boardInfo[nx4][ny4] << endl;
					if (boardInfo[nx3][ny3] != isEmpty) {
						//cout << 1 << endl;
						if (judge({ ny2,nx2,isBLACK }, boardInfo) != whiteWin) three++;
					}
					else if (boardInfo[nx4][ny4] != isEmpty) {
						//cout << 2 << endl;
						if (judge({ ny1,nx1,isBLACK }, boardInfo) != whiteWin) three++;
					}
					else if (judge({ ny1,nx1,isBLACK }, boardInfo) != whiteWin || judge({ ny2,nx2,isBLACK }, boardInfo) != whiteWin) {
						//cout << 3 << endl;
						three++;
					}
					break;
				}
			}
			for (string str2 : blackThreeJ) {
				if (check[i] == false && board[i].find(str2) != string::npos) {//跳活三，递归一个中间点是不是禁手
					int idx;
					int po = board[i].find("0020");
					if (po != string::npos) {
						idx = (po + 2) - 4;
					}
					po = board[i].find("0200");
					if (po != string::npos) {
						idx = (po + 1) - 4;
					}
					int nx3 = row + dir[i][0] * idx, ny3 = col + dir[i][1] * idx;
					//cout << nx3 << " " << ny3 << endl;
					//cout << idx << endl;
					if (judge({ ny3,nx3,isBLACK }, boardInfo) != whiteWin) three++;
					break;
				}
			}
		}
		//开始胜利判断
		if (five == true) {
			//is_ok = MessageBox(hnd, "黑棋获胜！（黑棋成五）", "五子棋", MB_OK);
			return blackWin;
		}
		if (si == true) {
			//is_ok = MessageBox(hnd, "白棋获胜！（黑棋长连）", "五子棋", MB_OK);
			return whiteWin;
		}
		if (four >= 2) {
			//is_ok = MessageBox(hnd, "白棋获胜！（黑棋四四）", "五子棋", MB_OK);
			return whiteWin;
		}
		if (three >= 2) {
			//is_ok = MessageBox(hnd, "白棋获胜！（黑棋三三）", "五子棋", MB_OK);
			return whiteWin;
		}
	}
	else if (pos.color == isWHITE) {//最后白棋落子
		for (string str1 : board) {
			for (string str2 : whiteFive) {
				if (str1.find(str2) != string::npos) {
					//弹出窗口，提示用户操作
					//is_ok = MessageBox(hnd, "白棋获胜！（白棋成五）", "五子棋", MB_OK);
					return whiteWin;
				}
			}
		}
	}
	return notEnd;//无人获胜
}


bool chessBoard::judgeForbid(chessPos pos, vector<vector<crossPointState>> boardInfo)
{
	int col = pos.col, row = pos.row;
	char ch = '0';
	int idx = 0;
	string board[4];//以最后落子点为中心，向四个方向扫描长度为9的四个字符串
	for (int i = 0; i < 4; i++) board[i] += ch;
	int dx[8] = { 0,0,1,-1,-1,1,1,-1 }, dy[8] = { -1,1,-1,1,0,0,1,-1 };//八个方向
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 4; j++) {//从中心向四周扫描，防止越界
			ch = crossPointStateToChar(boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j]);
			board[i] = board[i] + ch;
			if (ch == '3') {//如果已经扫描至边界，直接向另一端扫描
				break;
			}
		}
		for (int j = 1; j <= 4; j++) {
			ch = crossPointStateToChar(boardInfo[row + dy[i * 2 + 1] * j][col + dx[i * 2 + 1] * j]);
			board[i] = ch + board[i];
			if (ch == '3') {//如果已经扫描至边界，直接向另一端扫描
				break;
			}
		}
		//cout << row << "   " << col << "   " << idx++ << ":   " << board[i] << endl;
	}
	//cout << "before" << endl;
	//coutBoard();
	int four = 0;
	int three = 0;
	bool check[4] = { false,false,false,false };
	for (int i = 0; i < 4;i++) {
		for (string str2 : blackFour) {
			if (board[i].find(str2) != string::npos) {
				check[i] = true;
			}
		}
		for (string str2 : blackThreeA) {
			if (check[i] == false && board[i].find(str2) != string::npos) {
				three++;
				if (three >= 2) {
					//cout << "three" << endl;
					return true;
				}
				break;
			}
		}
	}
	//cout << "safe" << endl;
	return false;
}
bool chessBoard::preciseJudgeForbid(chessPos pos, vector<vector<crossPointState>>& boardInfo)
{
	const int col = pos.col, row = pos.row;
	char ch;
	const int dx[8] = { 0,0,1,-1,-1,1,1,-1 }, dy[8] = { -1,1,-1,1,0,0,1,-1 };//八个方向
	//先对棋盘上每一个可能是三三禁手的位置进行标注
	vector<vector<bool>> possibleForbid(boardSize + 2, vector<bool>(boardSize + 2, false));
	for (int i = 1; i <= boardSize; i++) {
		for (int j = 1; j <= boardSize; j++) {
			if (boardInfo[i][j] == isEmpty) possibleForbid[i][j] = judgeForbid({ j,i,isBLACK }, boardInfo);
		}
	}
	//对需要判断的位置进行递归判断
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 4; j++) {//从中心向四周扫描，防止越界
			ch = crossPointStateToChar(boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j]);
			if (ch == '3') {//如果已经扫描至边界，直接向另一端扫描
				break;
			}
			if (possibleForbid[row + dy[i * 2] * j][col + dx[i * 2] * j]) {//如果这个位置可能涉及禁手问题
				boardInfo[row][col] = isBPiece;//假设原位置有子
				if (preciseJudgeForbid({ col + dx[i * 2] * j ,row + dy[i * 2] * j,isBLACK }, boardInfo)) {//递归判断下一个位置
					boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j] = isForbid;//修改该位置状态
				}
				else boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j] = isEmpty;
				//boardInfo[row][col] = isEmpty;//恢复原位置状态
			}
		}
		for (int j = 1; j <= 4; j++) {
			ch = crossPointStateToChar(boardInfo[row + dy[i * 2 + 1] * j][col + dx[i * 2 + 1] * j]);
			if (ch == '3') {//如果已经扫描至边界，直接向另一端扫描
				break;
			}
			if (possibleForbid[row + dy[i * 2+1] * j][col + dx[i * 2+1] * j]) {//如果这个位置可能涉及禁手问题
				boardInfo[row][col] = isBPiece;//假设原位置有子
				if (preciseJudgeForbid({ col + dx[i * 2+1] * j ,row + dy[i * 2+1] * j,isBLACK }, boardInfo)) {//递归判断下一个位置
					boardInfo[row + dy[i * 2+1] * j][col + dx[i * 2+1] * j] = isForbid;//修改该位置状态
				}
				else boardInfo[row + dy[i * 2+1] * j][col + dx[i * 2+1] * j] = isEmpty;
				//boardInfo[row][col] = isEmpty;//恢复原位置状态
			}
		}
	}
	//更新完棋盘其他位置的情况后，判断输入位置是否为禁手
	return judgeForbid({ col,row,isBLACK }, boardInfo);
}
void chessBoard::timer()
{

}
matchResult chessBoard::preciseJudge(chessPos pos, vector<vector<crossPointState>> boardInfo)
{	const int col = pos.col, row = pos.row;
	char ch;
	int is_ok;
	const int dx[8] = { 0,0,1,-1,-1,1,1,-1 }, dy[8] = { -1,1,-1,1,0,0,1,-1 };//八个方向
	//获取窗口句柄
	HWND hnd = GetHWnd();
	if (pos.color == isBLACK) {//最后黑棋落子
		//更新棋盘上的禁手情况
		bool bo=preciseJudgeForbid(pos, boardInfo);//此时利用函数更改boardInfo状态，而非利用返回值
		//cout << endl;
		coutBoard(boardInfo);
		//扫描四个方向的字符串
		ch = crossPointStateToChar(boardInfo[row][col]);
		string board[4];//以最后落子点为中心，向四个方向扫描长度为9的四个字符串
		for (int i = 0; i < 4; i++) board[i] += ch;
		for (int i = 0; i < 4; i++) {
			for (int j = 1; j <= 4; j++) {//从中心向四周扫描，防止越界
				ch = crossPointStateToChar(boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j]);
				board[i] = board[i] + ch;
				if (ch == '3') {//如果已经扫描至边界，直接向另一端扫描
					break;
				}
			}
			for (int j = 1; j <= 4; j++) {
				ch = crossPointStateToChar(boardInfo[row + dy[i * 2 + 1] * j][col + dx[i * 2 + 1] * j]);
				board[i] = ch + board[i];
				if (ch == '3') {//如果已经扫描至边界，直接向另一端扫描
					break;
				}
			}
			//cout << board[i] << endl;
		}
		//coutBoard();
		int four = 0;
		int three = 0;
		bool check[4] = { false,false,false,false };
		for (int i = 0; i < 4; i++) {//先检测有没有长连
			for (string str2 : blackLong) {
				if (board[i].find(str2) != string::npos) {
					//弹出窗口，提示用户操作
					is_ok = MessageBox(hnd, "白棋获胜！（黑棋长连）", "五子棋", MB_OK);
					return whiteWin;
				}
			}
		}
		for (int i = 0; i < 4;i++) {
			for (string str2 : blackFive) {
				if (board[i].find(str2) != string::npos) {
					//弹出窗口，提示用户操作
					is_ok = MessageBox(hnd, "黑棋获胜！（黑棋成五）", "五子棋", MB_OK);
					return blackWin;
				}
			}
			for (string str2 : blackSpecial) {
				if (board[i].find(str2) != string::npos) {
					is_ok = MessageBox(hnd, "白棋获胜！（黑棋四四）", "五子棋", MB_OK);
					return whiteWin;
				}
			}
			for (string str2 : blackFour) {
				if (board[i].find(str2) != string::npos) {
					four++;
					check[i] = true;
					break;
				}
			}
			for (string str2 : blackThreeA) {
				if (check[i] == false && board[i].find(str2) != string::npos) {
					three++;
					break;
				}
			}
		}
		if (four >= 2) {
			//弹出窗口，提示用户操作
			is_ok = MessageBox(hnd, "白棋获胜！（黑棋四四）", "五子棋", MB_OK);
			return whiteWin;
		}
		if (three >= 2) {
			//弹出窗口，提示用户操作
			is_ok = MessageBox(hnd, "白棋获胜！（黑棋三三）", "五子棋", MB_OK);
			return whiteWin;
		}
	}
	else if (pos.color == isWHITE) {//最后白棋落子
		//直接扫描四个方向的字符串
		char ch = crossPointStateToChar(boardInfo[row][col]);
		string board[4];//以最后落子点为中心，向四个方向扫描长度为9的四个字符串
		for (int i = 0; i < 4; i++) board[i] += ch;
		for (int i = 0; i < 4; i++) {
			for (int j = 1; j <= 4; j++) {//从中心向四周扫描，防止越界
				ch = crossPointStateToChar(boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j]);
				board[i] = board[i] + ch;
				if (ch == '3') {//如果已经扫描至边界，直接向另一端扫描
					break;
				}
			}
			for (int j = 1; j <= 4; j++) {
				ch = crossPointStateToChar(boardInfo[row + dy[i * 2 + 1] * j][col + dx[i * 2 + 1] * j]);
				board[i] = ch + board[i];
				if (ch == '3') {//如果已经扫描至边界，直接向另一端扫描
					break;
				}
			}
			//cout << board[i] << endl;
		}
		//coutBoard();
		for (string str1 : board) {
			for (string str2 : whiteFive) {
				if (str1.find(str2) != string::npos) {
					//弹出窗口，提示用户操作
					is_ok = MessageBox(hnd, "白棋获胜！（白棋成五）", "五子棋", MB_OK);
					return whiteWin;
				}
			}
		}
	}
	return notEnd;//无人获胜
}