#include "chessBoard.h"
#include<graphics.h>
#include<cmath>
#include<fstream>
#include<iostream>
#include<mmsystem.h>//������ý��ӿ�
#pragma comment(lib,"winmm.lib")//����ӿ�
void putimagePNG(int x, int y, IMAGE* picture);
// ����>>��ȡ�����
std::istream& operator>>(std::istream& in, crossPointState& state) {
	int value;
	if (in >> value) {  // ���Դ�����������ȡ����
		switch (value) {
		case 3:
			state = isBorder;
			break;
		case 2:
			state = isEmpty;
			break;
		case 1:
			state = isWPiece;
			break;
		case 0:
			state = isBPiece;
			break;
		case -1:
			state = isForbid;
			break;
		default:
			in.setstate(std::ios::failbit); // ������Ϊʧ��״̬
			break;
		}
	}
	return in;
}
std::istream& operator>>(std::istream& in, turn& state) {
	int value;
	if (in >> value) {  // ���Դ�����������ȡ����
		switch (value) {
		case 1:
			state = isAiTurn;
			break;
		case 0:
			state = isPlayerTurn;
			break;
		default:
			in.setstate(std::ios::failbit); // ������Ϊʧ��״̬
			break;
		}
	}
	return in;
}
std::istream& operator>>(std::istream& in, pieceType& state) {
	int value;
	if (in >> value) {  // ���Դ�����������ȡ����
		switch (value) {
		case 1:
			state = isWHITE;
			break;
		case 0:
			state = isBLACK;
			break;
		default:
			in.setstate(std::ios::failbit); // ������Ϊʧ��״̬
			break;
		}
	}
	return in;
}
void chessBoard::selectColor()
{
	//ʹ�öԻ���ѡɫ
	//�������ڣ���ʾ�û�����
	int is_ok;
	//��ȡ���ھ��
	HWND hnd = GetHWnd();
	is_ok = MessageBox(hnd, "�Ƿ�Ҫѡ������(��΢ռ�ţ���", "������",MB_YESNO);
	if (is_ok == IDYES)
	{
		currentTurn = isPlayerTurn;
		currentColor = isBLACK;
		black = isPlayerTurn;
		white = isAiTurn;
		//cout << "select color end" << endl;
		return;
	}
	else if (is_ok == IDNO)
	{
		currentTurn = isAiTurn;
		currentColor = isBLACK;
		black = isAiTurn;
		white = isPlayerTurn;
		//cout << "select color end" << endl;
		return;
	}
	//ʹ��ͼ�λ�����ѡɫ
	//go({ 6,8,isBLACK });
	//go({ 10,8,isWHITE });
	//while (1) {//ѡɫ���ǰʼ�յȴ�
	//	if (clickPos.col == 6 && clickPos.row == 8) {
	//		currentTurn = isPlayerTurn;
	//		currentColor = isBLACK;
	//		black = isPlayerTurn;
	//		white = isAiTurn;
	//		cout << "select color end" << endl;
	//		return;
	//	}
	//	else if (clickPos.col == 10 && clickPos.row == 8) {
	//		currentTurn = isAiTurn;
	//		currentColor = isBLACK;
	//		black = isAiTurn;
	//		white = isPlayerTurn;
	//		cout << "select color end" << endl;
	//		return;
	//	}
	//}
	//cout << "select color end" << endl;
	//return;
}
void chessBoard::selectMethod()
{
	//ʹ�öԻ���ѡɫ
//�������ڣ���ʾ�û�����
	int is_ok;
	//��ȡ���ھ��
	HWND hnd = GetHWnd();
	is_ok = MessageBox(hnd, "�Ƿ�Ҫѡ���˻���ս��", "������", MB_YESNO);
	if (is_ok == IDYES)
	{
		selectColor();
		return;
	}
	else if (is_ok == IDNO)
	{
		currentTurn = isPlayerTurn;
		currentColor = isBLACK;
		black = isPlayerTurn;
		white = isPlayerTurn;
		/*currentTurn = isAiTurn;
		currentColor = isBLACK;
		black = isAiTurn;
		white = isAiTurn;*/
		return;
	}
}
void chessBoard::putImage()
{
	putimage(0, 0, &img_Board);//��������ͼƬ�����
}
void chessBoard::startMatch()
{
	result = pause;
	isblack = true;
	//initgraph(windowWidth, windowHeight);//���һ�������̴�С��ͬ��ͼ�λ�����
	putimage(0, 0, &img_Board);//��������ͼƬ�����
	//currentTurn = isPlayerTurn;
	selectMethod();//ѡ����
	putimage(0, 0, &img_Board);//��������ͼƬ�����
	init();//���̳�ʼ��
	result = notEnd;
	steps = 0;
	lastPos = { -1,-1,isBLACK };
	ofstream outStream("res/game.txt");
	outStream << -1 << " " << -1 << endl;//������׵�һ��������ԭ���ļ�
}

chessPos chessBoard::transXYtoPos(short x, short y)
{
	int col = (round((x - gapX[style]) / gridSizeScaledF)) + 1;
	int row = (round((y - gapY[style]) / gridSizeScaledF)) + 1;
	//std::cout << col << " " << row;
	return { col,row,currentColor };//����֮�󷵻أ���������
}

void chessBoard::go(chessPos pos)
{
	int px = ((gapX[style] + round(scale * ((pos.col - 1) * gridSize))) - pieceSizeScaled / 2);//-1��ԭ������col��11��ʼ��00Ϊ���̱߽�
	int py = ((gapY[style] + round(scale * ((pos.row - 1) * gridSize))) - pieceSizeScaled / 2);
	int pxi = ((gapX[style] + round(scale * ((pos.col - 1) * gridSize))) - iconSizeScaled / 2);//��ʾ��λ�ã���������������Щ����
	int pyi = ((gapY[style] + round(scale * ((pos.row - 1) * gridSize))) - iconSizeScaled / 2);
	int pxl = ((gapX[style] + round(scale * ((lastPos.col - 1) * gridSize))) - pieceSizeScaled / 2); //��һ������λ��
	int pyl = ((gapY[style] + round(scale * ((lastPos.row - 1) * gridSize))) - pieceSizeScaled / 2);
	if (px<0 || py<0 || px>windowWidth || py>windowHeight) {//���Ϸ�������
		cout << "failed to go" << endl;
	}
	else {
		//x�������������У���y������ߣ��У�
		if (pos.color == isWHITE) {
			putimagePNG(px, py, &img_White);
			putimagePNG(pxi, pyi, &img_icon);
			boardInfo[pos.row][pos.col] = isWPiece;//��¼��������
			if(lastPos.col!=-1) putimagePNG(pxl, pyl, &img_Black);
		}
		else if (pos.color == isBLACK) {
			putimagePNG(px, py, &img_Black);
			putimagePNG(pxi, pyi, &img_icon);
			boardInfo[pos.row][pos.col] = isBPiece;//��¼��������
			if (lastPos.col != -1) putimagePNG(pxl, pyl, &img_White);
		}
		lastPos = pos;
		steps += 1;
		//cout << "chess down" << endl;
		//cout << endl;
		//coutBoard(boardInfo);
		//cout << "step:" << steps << endl;
		//��¼��������
		ofstream outStream("res/game.txt", ios::app);
		outStream << pos.col << " " << pos.row << endl;
		outStream.close();
		result = judge(pos, boardInfo);//�����û�л��ʤ��
		//if (result != notEnd) cout << "game end" << endl;
		//else cout << "game continue" << endl;
	}
	return;
}



void chessBoard::goWithoutJudge(chessPos pos)
{
	int px = ((gapX[style] + round(scale * ((pos.col - 1) * gridSize))) - pieceSizeScaled / 2);//-1��ԭ������col��11��ʼ��00Ϊ���̱߽�
	int py = ((gapY[style] + round(scale * ((pos.row - 1) * gridSize))) - pieceSizeScaled / 2);
	//x�������������У���y������ߣ��У�
	if (pos.color == isWHITE) {
		putimagePNG(px, py, &img_White);
		boardInfo[pos.row][pos.col] = isWPiece;//��¼��������
	}
	else if (pos.color == isBLACK) {
		putimagePNG(px, py, &img_Black);
		boardInfo[pos.row][pos.col] = isBPiece;//��¼��������
	}
	steps++;
}

bool chessBoard::isEmptyBoard(int x, int y)
{
	if (boardInfo[y][x] == isEmpty|| boardInfo[y][x] == isForbid) {
		return true;
	}
	return false;
}
void chessBoard::save()
{
	int is_ok;
	//��ȡ���ھ��
	HWND hnd = GetHWnd();
	if (result == notEnd) {
		ofstream outStream("res/save.txt");
		outStream << currentTurn << " " << white << " " <<
			black << " " << currentColor << endl;
		for (int i = 1; i <= boardSize; i++) {
			for (int j = 1; j <= boardSize; j++) {
				outStream << boardInfo[i][j] << " ";
			}
			outStream << endl;
		}
		//�������ڣ���ʾ�û�����
		is_ok = MessageBox(hnd, "��ǰ�����ѳɹ����棡", "������", MB_OK);
		//cout << "board save" << endl;
	}
	else {
		is_ok = MessageBox(hnd, "��ǰ������Ч��", "������", MB_OK);
	}
}
void chessBoard::readSave()
{
	lastPos = { -1,-1,isBLACK };
	int is_ok;
	//��ȡ���ھ��
	HWND hnd = GetHWnd();
	//initgraph(windowWidth, windowHeight);//���һ�������̴�С��ͬ��ͼ�λ�����
	if (result == notEnd||result==review) {
		//�������ڣ���ʾ�û�����
		is_ok = MessageBox(hnd, "ȷ��Ҫ���ǵ�ǰ������", "������", MB_OKCANCEL);
		if (is_ok == IDOK) {
			putimage(0, 0, &img_Board);//��������ͼƬ�����
			ifstream inStream("res/save.txt");
			inStream >> currentTurn >> white >> black >> currentColor;
			isblack = (currentColor == isBLACK ? true : false);
			ofstream outStream("res/game.txt");
			outStream << -3 << " " << -3 << " ";
			result = notEnd;
			for (int i = 1; i <= boardSize; i++) {
				for (int j = 1; j <= boardSize; j++) {
					inStream >> boardInfo[i][j];
					outStream << boardInfo[i][j] << " ";
				}
				outStream << endl;
			}
			if (inStream.fail()) {
				int is_ok;
				//��ȡ���ھ��
				HWND hnd = GetHWnd();
				//�������ڣ���ʾ�û�����
				is_ok = MessageBox(hnd, "�浵�������¿�ʼ�µ�һ��", "������", MB_OK);
				startMatch();
			}
			else {
				printBoard();
				//cout << "read success!" << endl;
			}
			inStream.close();
			outStream.close();
		}
		else return;
	}
	else {
		putimage(0, 0, &img_Board);//��������ͼƬ�����
		ifstream inStream("res/save.txt");
		inStream >> currentTurn >> white >> black >> currentColor;
		result = notEnd;
		for (int i = 1; i <= boardSize; i++) {
			for (int j = 1; j <= boardSize; j++) {
				inStream >> boardInfo[i][j];
			}
		}
		if (inStream.fail()) {
			int is_ok;
			//��ȡ���ھ��
			HWND hnd = GetHWnd();
			//�������ڣ���ʾ�û�����
			is_ok = MessageBox(hnd, "�浵�������¿�ʼ�µ�һ��", "������", MB_OK);
			startMatch();
		}
		else {
			printBoard();
			//cout << "read success!" << endl;
		}
		inStream.close();
	}
	
}

void chessBoard::leavegame()
{
	int is_ok;
	//��ȡ���ھ��
	HWND hnd = GetHWnd();
	//�������ڣ���ʾ�û�����
	is_ok = MessageBox(hnd, "ȷ��Ҫ�˳���Ϸ��", "������", MB_OKCANCEL);
	if (is_ok == IDOK)
	{
		if (result == notEnd) {
			is_ok = MessageBox(hnd, "�Ƿ�Ҫ���浱ǰ���棿", "������", MB_YESNO);
			if (is_ok == IDYES)
			{
				save();
			}
			exit(0);
		}
		else exit(0);
	}
	return;
}

void chessBoard::printBoard()
{
	for (int i = 1; i <= boardSize; i++) {
		for (int j = 1; j <= boardSize; j++) {
			switch (boardInfo[i][j])
			{
			case isWPiece:
				goWithoutJudge({ j,i,isWHITE });
				break;
			case isBPiece:
				goWithoutJudge({ j,i,isBLACK });
				break;
			case isEmpty:
				break;
			default:
				break;
			}
		}
	}
}

void chessBoard::coutBoard(vector<vector<crossPointState>> boardInfo)
{
	for (int i = 1; i <= boardSize; i++) {
		for (int j = 1; j <= boardSize; j++) {
			switch (boardInfo[i][j])
			{
			case isWPiece:
				cout<<1<<" ";
				break;
			case isBPiece:
				cout<<0<<" ";
				break;
			case isEmpty:
				cout << 2 << " ";
				break;
			case isForbid:
				cout << "- ";
			default:
				break;
			}
		}
		cout << endl;
	}
}

void chessBoard::coutBoard()
{
	for (int i = 1; i <= boardSize; i++) {
		for (int j = 1; j <= boardSize; j++) {
			switch (boardInfo[i][j])
			{
			case isWPiece:
				cout << 1 << " ";
				break;
			case isBPiece:
				cout << 0 << " ";
				break;
			case isEmpty:
				cout << 2 << " ";
				break;
			case isForbid:
				cout << "- ";
			default:
				break;
			}
		}
		cout << endl;
	}
}

vector<vector<crossPointState>> chessBoard::readBoard()
{
	return boardInfo;
}

void chessBoard::init()
{
	boardInfo.assign(boardSize + 2, vector<crossPointState>(boardSize + 2, isBorder));
	for (int i = 1; i <= boardSize; i++)
		for (int j = 1; j <= boardSize; j++)
			boardInfo[i][j] = isEmpty;
}

chessBoard::chessBoard()
{
	boardInfo.assign(boardSize + 2, vector<crossPointState>(boardSize + 2, isBorder));
	for (int i = 1; i <= boardSize; i++)
		for (int j = 1; j <= boardSize; j++)
			boardInfo[i][j] = isEmpty;
	loadimage(&img_Black, "res/black2.png", pieceSizeScaled, pieceSizeScaled);
	loadimage(&img_White, "res/white2.png", pieceSizeScaled, pieceSizeScaled);//���غڰ�����ͼƬ
	loadimage(&img_icon, "res/icon.png",iconSizeScaled,iconSizeScaled);
	switch (style) {
	case 0:
		loadimage(&img_Board, "res/board.png", windowWidth, windowHeight);//��������ͼƬ
		break;
	case 1:
		loadimage(&img_Board, "res/board2.png", windowWidth, windowHeight);//��������ͼƬ
		break;
	}
	//cout << "load image success" << endl;
}
