#include"chessBoard.h"
#include<string>
#include<iostream>
using namespace std;
// ���彫 crossPointState ת��Ϊ char �ĺ���
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
	string board[4];//��������ӵ�Ϊ���ģ����ĸ�����ɨ�賤��Ϊ9���ĸ��ַ���
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//�ĸ�����ˮƽ����ֱ����б����б
	int nx, ny, k;
	for (int l = 0; l < 4; l++) {//������
		k = 0;
		do {
			nx = row + dir[l][0] * k, ny = col + dir[l][1] * k;
			board[l] = board[l] + crossPointStateToChar(boardInfo[nx][ny]);
			k += 1;
		} while (boardInfo[nx][ny] != isBorder && k <= 4);//������
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
	int is_ok;//��ȡ���ھ��
	HWND hnd = GetHWnd();
	if (pos.color == isBLACK) {//����������
		int four = 0, three = 0;
		bool six[4] = { false,false,false,false }, five = false, si = false;
		bool check[4] = { false,false,false,false };//���һ��ֱ�����Ѿ���һ���ģ����ټ����
		//���������ж�
		for (int i = 0; i < 4; i++) {//�ȼ����û�г���
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
				if (check[i] == false && board[i].find(str2) != string::npos) {//���������ݹ������˵��ǲ��ǽ���
					//�ҵ������˵�
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
				if (check[i] == false && board[i].find(str2) != string::npos) {//���������ݹ�һ���м���ǲ��ǽ���
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
		//��ʼʤ���ж�
		if (five == true) {
			//is_ok = MessageBox(hnd, "�����ʤ����������壩", "������", MB_OK);
			return blackWin;
		}
		if (si == true) {
			//is_ok = MessageBox(hnd, "�����ʤ�������峤����", "������", MB_OK);
			return whiteWin;
		}
		if (four >= 2) {
			//is_ok = MessageBox(hnd, "�����ʤ�����������ģ�", "������", MB_OK);
			return whiteWin;
		}
		if (three >= 2) {
			//is_ok = MessageBox(hnd, "�����ʤ��������������", "������", MB_OK);
			return whiteWin;
		}
	}
	else if (pos.color == isWHITE) {//����������
		for (string str1 : board) {
			for (string str2 : whiteFive) {
				if (str1.find(str2) != string::npos) {
					//�������ڣ���ʾ�û�����
					//is_ok = MessageBox(hnd, "�����ʤ����������壩", "������", MB_OK);
					return whiteWin;
				}
			}
		}
	}
	return notEnd;//���˻�ʤ
}


bool chessBoard::judgeForbid(chessPos pos, vector<vector<crossPointState>> boardInfo)
{
	int col = pos.col, row = pos.row;
	char ch = '0';
	int idx = 0;
	string board[4];//��������ӵ�Ϊ���ģ����ĸ�����ɨ�賤��Ϊ9���ĸ��ַ���
	for (int i = 0; i < 4; i++) board[i] += ch;
	int dx[8] = { 0,0,1,-1,-1,1,1,-1 }, dy[8] = { -1,1,-1,1,0,0,1,-1 };//�˸�����
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 4; j++) {//������������ɨ�裬��ֹԽ��
			ch = crossPointStateToChar(boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j]);
			board[i] = board[i] + ch;
			if (ch == '3') {//����Ѿ�ɨ�����߽磬ֱ������һ��ɨ��
				break;
			}
		}
		for (int j = 1; j <= 4; j++) {
			ch = crossPointStateToChar(boardInfo[row + dy[i * 2 + 1] * j][col + dx[i * 2 + 1] * j]);
			board[i] = ch + board[i];
			if (ch == '3') {//����Ѿ�ɨ�����߽磬ֱ������һ��ɨ��
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
	const int dx[8] = { 0,0,1,-1,-1,1,1,-1 }, dy[8] = { -1,1,-1,1,0,0,1,-1 };//�˸�����
	//�ȶ�������ÿһ���������������ֵ�λ�ý��б�ע
	vector<vector<bool>> possibleForbid(boardSize + 2, vector<bool>(boardSize + 2, false));
	for (int i = 1; i <= boardSize; i++) {
		for (int j = 1; j <= boardSize; j++) {
			if (boardInfo[i][j] == isEmpty) possibleForbid[i][j] = judgeForbid({ j,i,isBLACK }, boardInfo);
		}
	}
	//����Ҫ�жϵ�λ�ý��еݹ��ж�
	for (int i = 0; i < 4; i++) {
		for (int j = 1; j <= 4; j++) {//������������ɨ�裬��ֹԽ��
			ch = crossPointStateToChar(boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j]);
			if (ch == '3') {//����Ѿ�ɨ�����߽磬ֱ������һ��ɨ��
				break;
			}
			if (possibleForbid[row + dy[i * 2] * j][col + dx[i * 2] * j]) {//������λ�ÿ����漰��������
				boardInfo[row][col] = isBPiece;//����ԭλ������
				if (preciseJudgeForbid({ col + dx[i * 2] * j ,row + dy[i * 2] * j,isBLACK }, boardInfo)) {//�ݹ��ж���һ��λ��
					boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j] = isForbid;//�޸ĸ�λ��״̬
				}
				else boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j] = isEmpty;
				//boardInfo[row][col] = isEmpty;//�ָ�ԭλ��״̬
			}
		}
		for (int j = 1; j <= 4; j++) {
			ch = crossPointStateToChar(boardInfo[row + dy[i * 2 + 1] * j][col + dx[i * 2 + 1] * j]);
			if (ch == '3') {//����Ѿ�ɨ�����߽磬ֱ������һ��ɨ��
				break;
			}
			if (possibleForbid[row + dy[i * 2+1] * j][col + dx[i * 2+1] * j]) {//������λ�ÿ����漰��������
				boardInfo[row][col] = isBPiece;//����ԭλ������
				if (preciseJudgeForbid({ col + dx[i * 2+1] * j ,row + dy[i * 2+1] * j,isBLACK }, boardInfo)) {//�ݹ��ж���һ��λ��
					boardInfo[row + dy[i * 2+1] * j][col + dx[i * 2+1] * j] = isForbid;//�޸ĸ�λ��״̬
				}
				else boardInfo[row + dy[i * 2+1] * j][col + dx[i * 2+1] * j] = isEmpty;
				//boardInfo[row][col] = isEmpty;//�ָ�ԭλ��״̬
			}
		}
	}
	//��������������λ�õ�������ж�����λ���Ƿ�Ϊ����
	return judgeForbid({ col,row,isBLACK }, boardInfo);
}
void chessBoard::timer()
{

}
matchResult chessBoard::preciseJudge(chessPos pos, vector<vector<crossPointState>> boardInfo)
{	const int col = pos.col, row = pos.row;
	char ch;
	int is_ok;
	const int dx[8] = { 0,0,1,-1,-1,1,1,-1 }, dy[8] = { -1,1,-1,1,0,0,1,-1 };//�˸�����
	//��ȡ���ھ��
	HWND hnd = GetHWnd();
	if (pos.color == isBLACK) {//����������
		//���������ϵĽ������
		bool bo=preciseJudgeForbid(pos, boardInfo);//��ʱ���ú�������boardInfo״̬���������÷���ֵ
		//cout << endl;
		coutBoard(boardInfo);
		//ɨ���ĸ�������ַ���
		ch = crossPointStateToChar(boardInfo[row][col]);
		string board[4];//��������ӵ�Ϊ���ģ����ĸ�����ɨ�賤��Ϊ9���ĸ��ַ���
		for (int i = 0; i < 4; i++) board[i] += ch;
		for (int i = 0; i < 4; i++) {
			for (int j = 1; j <= 4; j++) {//������������ɨ�裬��ֹԽ��
				ch = crossPointStateToChar(boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j]);
				board[i] = board[i] + ch;
				if (ch == '3') {//����Ѿ�ɨ�����߽磬ֱ������һ��ɨ��
					break;
				}
			}
			for (int j = 1; j <= 4; j++) {
				ch = crossPointStateToChar(boardInfo[row + dy[i * 2 + 1] * j][col + dx[i * 2 + 1] * j]);
				board[i] = ch + board[i];
				if (ch == '3') {//����Ѿ�ɨ�����߽磬ֱ������һ��ɨ��
					break;
				}
			}
			//cout << board[i] << endl;
		}
		//coutBoard();
		int four = 0;
		int three = 0;
		bool check[4] = { false,false,false,false };
		for (int i = 0; i < 4; i++) {//�ȼ����û�г���
			for (string str2 : blackLong) {
				if (board[i].find(str2) != string::npos) {
					//�������ڣ���ʾ�û�����
					is_ok = MessageBox(hnd, "�����ʤ�������峤����", "������", MB_OK);
					return whiteWin;
				}
			}
		}
		for (int i = 0; i < 4;i++) {
			for (string str2 : blackFive) {
				if (board[i].find(str2) != string::npos) {
					//�������ڣ���ʾ�û�����
					is_ok = MessageBox(hnd, "�����ʤ����������壩", "������", MB_OK);
					return blackWin;
				}
			}
			for (string str2 : blackSpecial) {
				if (board[i].find(str2) != string::npos) {
					is_ok = MessageBox(hnd, "�����ʤ�����������ģ�", "������", MB_OK);
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
			//�������ڣ���ʾ�û�����
			is_ok = MessageBox(hnd, "�����ʤ�����������ģ�", "������", MB_OK);
			return whiteWin;
		}
		if (three >= 2) {
			//�������ڣ���ʾ�û�����
			is_ok = MessageBox(hnd, "�����ʤ��������������", "������", MB_OK);
			return whiteWin;
		}
	}
	else if (pos.color == isWHITE) {//����������
		//ֱ��ɨ���ĸ�������ַ���
		char ch = crossPointStateToChar(boardInfo[row][col]);
		string board[4];//��������ӵ�Ϊ���ģ����ĸ�����ɨ�賤��Ϊ9���ĸ��ַ���
		for (int i = 0; i < 4; i++) board[i] += ch;
		for (int i = 0; i < 4; i++) {
			for (int j = 1; j <= 4; j++) {//������������ɨ�裬��ֹԽ��
				ch = crossPointStateToChar(boardInfo[row + dy[i * 2] * j][col + dx[i * 2] * j]);
				board[i] = board[i] + ch;
				if (ch == '3') {//����Ѿ�ɨ�����߽磬ֱ������һ��ɨ��
					break;
				}
			}
			for (int j = 1; j <= 4; j++) {
				ch = crossPointStateToChar(boardInfo[row + dy[i * 2 + 1] * j][col + dx[i * 2 + 1] * j]);
				board[i] = ch + board[i];
				if (ch == '3') {//����Ѿ�ɨ�����߽磬ֱ������һ��ɨ��
					break;
				}
			}
			//cout << board[i] << endl;
		}
		//coutBoard();
		for (string str1 : board) {
			for (string str2 : whiteFive) {
				if (str1.find(str2) != string::npos) {
					//�������ڣ���ʾ�û�����
					is_ok = MessageBox(hnd, "�����ʤ����������壩", "������", MB_OK);
					return whiteWin;
				}
			}
		}
	}
	return notEnd;//���˻�ʤ
}