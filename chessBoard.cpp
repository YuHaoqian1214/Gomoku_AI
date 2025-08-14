#include "chessBoard.h"
#include<graphics.h>
#include<cmath>
#include<fstream>
#include<iostream>
#include<mmsystem.h>//包含多媒体接口
#pragma comment(lib,"winmm.lib")//处理接口
void putimagePNG(int x, int y, IMAGE* picture);
// 定义>>提取运算符
std::istream& operator>>(std::istream& in, crossPointState& state) {
	int value;
	if (in >> value) {  // 尝试从输入流中提取整数
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
			in.setstate(std::ios::failbit); // 设置流为失败状态
			break;
		}
	}
	return in;
}
std::istream& operator>>(std::istream& in, turn& state) {
	int value;
	if (in >> value) {  // 尝试从输入流中提取整数
		switch (value) {
		case 1:
			state = isAiTurn;
			break;
		case 0:
			state = isPlayerTurn;
			break;
		default:
			in.setstate(std::ios::failbit); // 设置流为失败状态
			break;
		}
	}
	return in;
}
std::istream& operator>>(std::istream& in, pieceType& state) {
	int value;
	if (in >> value) {  // 尝试从输入流中提取整数
		switch (value) {
		case 1:
			state = isWHITE;
			break;
		case 0:
			state = isBLACK;
			break;
		default:
			in.setstate(std::ios::failbit); // 设置流为失败状态
			break;
		}
	}
	return in;
}
void chessBoard::selectColor()
{
	//使用对话框选色
	//弹出窗口，提示用户操作
	int is_ok;
	//获取窗口句柄
	HWND hnd = GetHWnd();
	is_ok = MessageBox(hnd, "是否要选择先行(略微占优）？", "五子棋",MB_YESNO);
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
	//使用图形化窗口选色
	//go({ 6,8,isBLACK });
	//go({ 10,8,isWHITE });
	//while (1) {//选色完成前始终等待
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
	//使用对话框选色
//弹出窗口，提示用户操作
	int is_ok;
	//获取窗口句柄
	HWND hnd = GetHWnd();
	is_ok = MessageBox(hnd, "是否要选择人机对战？", "五子棋", MB_YESNO);
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
	putimage(0, 0, &img_Board);//加载棋盘图片并输出
}
void chessBoard::startMatch()
{
	result = pause;
	isblack = true;
	//initgraph(windowWidth, windowHeight);//输出一个和棋盘大小相同的图形化窗口
	putimage(0, 0, &img_Board);//加载棋盘图片并输出
	//currentTurn = isPlayerTurn;
	selectMethod();//选择颜
	putimage(0, 0, &img_Board);//加载棋盘图片并输出
	init();//棋盘初始化
	result = notEnd;
	steps = 0;
	lastPos = { -1,-1,isBLACK };
	ofstream outStream("res/game.txt");
	outStream << -1 << " " << -1 << endl;//输出棋谱第一步，覆盖原本文件
}

chessPos chessBoard::transXYtoPos(short x, short y)
{
	int col = (round((x - gapX[style]) / gridSizeScaledF)) + 1;
	int row = (round((y - gapY[style]) / gridSizeScaledF)) + 1;
	//std::cout << col << " " << row;
	return { col,row,currentColor };//走棋之后返回，结束函数
}

void chessBoard::go(chessPos pos)
{
	int px = ((gapX[style] + round(scale * ((pos.col - 1) * gridSize))) - pieceSizeScaled / 2);//-1的原因在于col从11开始，00为棋盘边界
	int py = ((gapY[style] + round(scale * ((pos.row - 1) * gridSize))) - pieceSizeScaled / 2);
	int pxi = ((gapX[style] + round(scale * ((pos.col - 1) * gridSize))) - iconSizeScaled / 2);//提示点位置（由于四舍五入有些许误差）
	int pyi = ((gapY[style] + round(scale * ((pos.row - 1) * gridSize))) - iconSizeScaled / 2);
	int pxl = ((gapX[style] + round(scale * ((lastPos.col - 1) * gridSize))) - pieceSizeScaled / 2); //上一步落子位置
	int pyl = ((gapY[style] + round(scale * ((lastPos.row - 1) * gridSize))) - pieceSizeScaled / 2);
	if (px<0 || py<0 || px>windowWidth || py>windowHeight) {//不合法的输入
		cout << "failed to go" << endl;
	}
	else {
		//x代表横向坐标宽（列），y是纵向高（行）
		if (pos.color == isWHITE) {
			putimagePNG(px, py, &img_White);
			putimagePNG(pxi, pyi, &img_icon);
			boardInfo[pos.row][pos.col] = isWPiece;//记录落子数据
			if(lastPos.col!=-1) putimagePNG(pxl, pyl, &img_Black);
		}
		else if (pos.color == isBLACK) {
			putimagePNG(px, py, &img_Black);
			putimagePNG(pxi, pyi, &img_icon);
			boardInfo[pos.row][pos.col] = isBPiece;//记录落子数据
			if (lastPos.col != -1) putimagePNG(pxl, pyl, &img_White);
		}
		lastPos = pos;
		steps += 1;
		//cout << "chess down" << endl;
		//cout << endl;
		//coutBoard(boardInfo);
		//cout << "step:" << steps << endl;
		//记录棋谱数据
		ofstream outStream("res/game.txt", ios::app);
		outStream << pos.col << " " << pos.row << endl;
		outStream.close();
		result = judge(pos, boardInfo);//检测有没有获得胜利
		//if (result != notEnd) cout << "game end" << endl;
		//else cout << "game continue" << endl;
	}
	return;
}



void chessBoard::goWithoutJudge(chessPos pos)
{
	int px = ((gapX[style] + round(scale * ((pos.col - 1) * gridSize))) - pieceSizeScaled / 2);//-1的原因在于col从11开始，00为棋盘边界
	int py = ((gapY[style] + round(scale * ((pos.row - 1) * gridSize))) - pieceSizeScaled / 2);
	//x代表横向坐标宽（列），y是纵向高（行）
	if (pos.color == isWHITE) {
		putimagePNG(px, py, &img_White);
		boardInfo[pos.row][pos.col] = isWPiece;//记录落子数据
	}
	else if (pos.color == isBLACK) {
		putimagePNG(px, py, &img_Black);
		boardInfo[pos.row][pos.col] = isBPiece;//记录落子数据
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
	//获取窗口句柄
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
		//弹出窗口，提示用户操作
		is_ok = MessageBox(hnd, "当前局面已成功保存！", "五子棋", MB_OK);
		//cout << "board save" << endl;
	}
	else {
		is_ok = MessageBox(hnd, "当前局面无效！", "五子棋", MB_OK);
	}
}
void chessBoard::readSave()
{
	lastPos = { -1,-1,isBLACK };
	int is_ok;
	//获取窗口句柄
	HWND hnd = GetHWnd();
	//initgraph(windowWidth, windowHeight);//输出一个和棋盘大小相同的图形化窗口
	if (result == notEnd||result==review) {
		//弹出窗口，提示用户操作
		is_ok = MessageBox(hnd, "确认要覆盖当前局面吗？", "五子棋", MB_OKCANCEL);
		if (is_ok == IDOK) {
			putimage(0, 0, &img_Board);//加载棋盘图片并输出
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
				//获取窗口句柄
				HWND hnd = GetHWnd();
				//弹出窗口，提示用户操作
				is_ok = MessageBox(hnd, "存档错误！重新开始新的一局", "五子棋", MB_OK);
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
		putimage(0, 0, &img_Board);//加载棋盘图片并输出
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
			//获取窗口句柄
			HWND hnd = GetHWnd();
			//弹出窗口，提示用户操作
			is_ok = MessageBox(hnd, "存档错误！重新开始新的一局", "五子棋", MB_OK);
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
	//获取窗口句柄
	HWND hnd = GetHWnd();
	//弹出窗口，提示用户操作
	is_ok = MessageBox(hnd, "确认要退出游戏吗？", "五子棋", MB_OKCANCEL);
	if (is_ok == IDOK)
	{
		if (result == notEnd) {
			is_ok = MessageBox(hnd, "是否要保存当前局面？", "五子棋", MB_YESNO);
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
	loadimage(&img_White, "res/white2.png", pieceSizeScaled, pieceSizeScaled);//加载黑白棋子图片
	loadimage(&img_icon, "res/icon.png",iconSizeScaled,iconSizeScaled);
	switch (style) {
	case 0:
		loadimage(&img_Board, "res/board.png", windowWidth, windowHeight);//加载棋盘图片
		break;
	case 1:
		loadimage(&img_Board, "res/board2.png", windowWidth, windowHeight);//加载棋盘图片
		break;
	}
	//cout << "load image success" << endl;
}
