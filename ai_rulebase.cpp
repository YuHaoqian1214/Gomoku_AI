#include "ai.h"
#include<vector>
#include<random>
#include<iostream>
#include<string>
#include<utility>
chessPos ai::rulebase(int row, int col, int color, vector<vector<char>>& board)
{//color����ո��¹�����ɫ
	color = changeColor(color);
	int maxScore = 0, dx = 0, dy = 0;
	for (int x = 0; x < boardSize; x++) {
		for (int y = 0; y < boardSize; y++) {
			if (board[x][y] == '2'||board[x][y]=='-') {//���������
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
int ai::calculate(int row, int col, int color, int step, int score, vector<vector<char>> board)//step�������,score����Ŀǰ����
{
	if (step >= limit) return score;//����Ѿ��ﵽԤ�����
	else {
		vector<vector<vector<string>>> allstr(boardSize, vector<vector<string>>(boardSize, vector<string>(4, "")));
		if (color == 1) {//��һ���ǰ��壬�ⲽaiִ����
			color = 0;//����ɫ��ת
			board[row][col] = '0';//�����ڸ�λ������Ӳ�����
			refresh(row, col, color, board, allstr);//�������̽��֣�ɨ�����е���ַ�������
			string strs[4] = { allstr[row][col][0],allstr[row][col][1],allstr[row][col][2] ,allstr[row][col][3] };
			if (checkWin(row, col, 0, board, strs)) {//�ȼ���Ƿ�ֱ��ʤ��
				return -1;//�������ֱ�ӻ�ʤ������
			}
			if (!checkAllow(row, col, 0, board)) {//����Ƿ����ӵ�������
				return -2;//����䵽�����ϣ���������
			}
			board[row][col] = '1';//�����ڸ�λ������Ӳ�����
			if (checkWin(row, col, 1, board, strs)) {//�ȼ���Ƿ�ֱ��ʤ��
				return -1;//�����������ʤ�������ӷ���
			}
			board[row][col] = '0';//�����ڸ�λ������Ӳ�����

		}
		else {//ִ����
			color = 1;
			board[row][col] = '1';//�����ڸ�λ�����Ӳ�����
			refresh(row, col, color, board, allstr);//�������̽��֣�ɨ�����е���ַ�������
			for (int x = 0; x < boardSize; x++) {
				for (int y = 0; y < boardSize; y++) {
					string strs[4] = { allstr[x][y][0],allstr[x][y][1],allstr[x][y][2] ,allstr[x][y][3] };
					if (checkWin(x, y, color, board, strs)) {//�ȼ������ֱ��ʤ��
						return -2;
					}
				}
			}
		}
	}
}

bool ai::checkFourA(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//�˾����ֵ���������
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackFourA) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//����������
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
	if (color == 0) {//�˾����ֵ���������
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackFourD) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//����������
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
	if (color == 0) {//�˾����ֵ���������
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackThreeA) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//����������
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
	if (color == 0) {//�˾����ֵ���������
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackThreeJ) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//����������
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
	if (color == 0) {//�˾����ֵ���������
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackThreeD) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//����������
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
	if (color == 0) {//�˾����ֵ���������
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackTwoA) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//����������
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
	if (color == 0) {//�˾����ֵ���������
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackTwoD) {
				if (strs[i].find(str2) != string::npos) {
					return true;
				}
			}
			return false;
		}
	}
	else if (color == 1) {//����������
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
	if (color == 0) {//�˾����ֵ���������
		bool check[4] = { false,false,false,false };//���һ��ֱ�����Ѿ���һ���ģ����ټ����
		bool four = false, three = false;
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackFour) {//����
				if (strs[i].find(str2) != string::npos) {
					check[i] = true;
					four = true;
					break;
				}
			}
			for (string str2 : blackThreeA) {//����
				if (!check[i] && strs[i].find(str2) != string::npos) {
					three = true;
					break;
				}
			}
			if (four && three) return true;
			else return false;
		}
	}
	else if (color == 1) {//����������
		bool check[4] = { false,false,false,false };//���һ��ֱ�����Ѿ���һ���ģ����ټ����
		int four = 0, three = 0;
		for (int i = 0; i < 4; i++) {
			for (string str2 : blackFour) {//����
				if (strs[i].find(str2) != string::npos) {
					check[i] = true;
					four++;
					break;
				}
			}
			for (string str2 : blackThreeA) {//����
				if (!check[i] && strs[i].find(str2) != string::npos) {
					three++;
					break;
				}
			}
			if (four + three >= 2) return true;//����˫�������������Ķ�������������ʤ��һ��
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
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//�ĸ�����ˮƽ����ֱ����б����б
	for (int l = 0; l < 4; l++) {//������
		k = 0;
		do {
			nx = row + dir[l][0] * k, ny = col + dir[l][1] * k;
			strs[l] = strs[l] + board[nx][ny];
			k += 1;
		} while (board[nx][ny] != isBorder && k <= 4);//������
		k = 1;
		do {
			nx = row - dir[l][0] * k, ny = col - dir[l][1] * k;
			strs[l] = board[nx][ny] + strs[l];
			k += 1;
		} while (board[nx][ny] != isBorder && k <= 4);
	}
	if (color == 0) {//�˾����ֵ���������
		//�����ж�	
		board[x][y] = '0';
		if (checkWin(x, y, color, board, strs)) {
			board[x][y] = ch;
			return false;//��������ܹ����壬����Ϊ�˴��ǽ���
		}
		else {
			int four = 0, three = 0;
			bool six[4] = { false,false,false,false }, five = false, si = false;
			bool check[4] = { false,false,false,false };//���һ��ֱ�����Ѿ���һ���ģ����ټ����
			for (int i = 0; i < 4; i++) {//����
				for (string str2 : blackLong) {
					if (strs[i].find(str2) != string::npos) {
						board[x][y] = ch;
						return true;
					}
				}
				for (string str2 : blackSpecial) {//����
					if (strs[i].find(str2) != string::npos) {
						board[x][y] = ch;
						return true;
					}
				}
				for (string str2 : blackFour) {//����
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
					if (check[i] == false && strs[i].find(str2) != string::npos) {//���������ݹ������˵��ǲ��ǽ���
						//�ҵ������˵�
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
					if (check[i] == false && strs[i].find(str2) != string::npos) {//���������ݹ�һ���м���ǲ��ǽ���
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
	else if (color == 1) {//����������
		return false;
	}
}

bool ai::checkWin(int x, int y, int color, vector<vector<char>>& board, string(&strs)[4])
{
	if (color == 0) {//����Ǻ��壬�ȼ��һ�������ǲ��ǳ���
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
	else {//����ǰ��壬ֱ�Ӽ���Ƿ���弴��
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
bool ai::checkAllow(int x, int y, int color, vector<vector<char>>& board)//�鿴�˴��Ƿ�������
{
	if (board[x][y] == '2') {//����Ѿ����ӣ������ǽ���
		return true;
	}
	else {
		return false;
	}
}

void ai::refresh(int row, int col, int color, vector<vector<char>>& board, vector<vector<vector<string>>>& allstr)
{
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//�ĸ�����ˮƽ����ֱ����б����б
	int nx = row, ny = col, k = 0, m = 0;
	//���½������
	for (int l = 0; l < 4; l++) {
		//������
		k = 0, m = 0;
		while (0 <= nx && nx < boardSize && 0 <= ny && ny < boardSize && k <= 4) {
			nx = row + dir[l][0] * k, ny = col + dir[l][1] * k;
			int nx1 = row, ny1 = col;
			string strs[4];
			for (int l = 0; l < 4; l++) {
				//������
				m = 0;
				while (0 <= nx1 && nx1 < boardSize && 0 <= ny1 && ny1 < boardSize && k <= 4) {
					nx1 = row + dir[l][0] * k, ny1 = col + dir[l][1] * k;
					strs[l] = strs[l] + board[nx1][ny1];
					m += 1;
				}
				//������
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
		//������
		k = 1, m = 0;
		while (0 <= nx && nx < boardSize && 0 <= ny && ny < boardSize && k <= 4) {
			nx = row + dir[l][0] * k, ny = col + dir[l][1] * k;
			int nx1 = row, ny1 = col;
			string strs[4];
			for (int l = 0; l < 4; l++) {
				//������
				m = 0;
				while (0 <= nx1 && nx1 < boardSize && 0 <= ny1 && ny1 < boardSize && k <= 4) {
					nx1 = row + dir[l][0] * k, ny1 = col + dir[l][1] * k;
					strs[l] = strs[l] + board[nx1][ny1];
					m += 1;
				}
				//������
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
			//��������,ɨ�����е��ַ�������
			for (int l = 0; l < 4; l++) {
				//������
				k = 0;
				while (0 <= nx && nx < boardSize && 0 <= ny && ny < boardSize && k <= 4) {
					nx = x + dir[l][0] * k, ny = y + dir[l][1] * k;
					allstr[x][y][l] = allstr[x][y][l] + board[nx][ny];

					k += 1;
				}
				//������
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