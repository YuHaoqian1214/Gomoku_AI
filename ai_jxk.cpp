#include "ai_jxk.h"
#include<string>
#include<vector>
#include<random>
#include<utility>
#include<climits>
#include<iostream>
#include <sstream>
#include "jsoncpp/json.h" // C++编译时默认包含此库
using namespace std;
void ai_jxk::inputFromGame(vector<vector<crossPointState>> boardInfo)
{
	//这个函数用本棋盘的存储方式（crosspointstate）数据类型来导入一个局面，不用单独处理边框
	init();
	step = 0;//判断已经走了几步
	for (int i = 0; i <= boardSize + 1; ++i) {
		for (int j = 0; j <= boardSize + 1; ++j) {
			switch (boardInfo[i][j])
			{
			case isBPiece:
				//棋盘记录棋子的规则如下：黑棋用1表示，白棋用2表示，空白用0表示，边框用-1表示，不存在其他情况
				board[i][j] = 1;
				++step;
				break;
			case isWPiece:
				++step;
				board[i][j] = 2;
				break;
			case isEmpty:
				board[i][j] = 0;
				break;
			case isBorder:
				board[i][j] = -1;
				break;
			default:
				break;
			}
		}
	}

	//判断当前步数该走黑棋还是白棋
	if (step % 2 == 0) isblack = true;
	else isblack = false;

	return;
}

void ai_jxk::init()
{
	for (int i = 0; i < 17; i++) {
		for (int j = 0; j < 17; j++) {
			if (i == 0 || j == 0 || i == 16 || j == 16) board[i][j] = -1;
			else board[i][j] = 0;
		}
	}
	step = 0;
}

void ai_jxk::placeAt(int x, int y)
{
	if (x >= 0 && y >= 0) {
		step += 1;
		if (step % 2 == 0) board[y + 1][x + 1] = 2;
		else board[y + 1][x + 1] = 1;
	}
}

void ai_jxk::inputFromBotzone()
{
	init();
	// 读入JSON
	string str;
	getline(cin, str);
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);
	// 分析自己收到的输入和自己过往的输出，并恢复状态
	int turnID = input["responses"].size();
	for (int i = 0; i < turnID; i++) {
		placeAt(input["requests"][i]["x"].asInt(), input["requests"][i]["y"].asInt());
		placeAt(input["responses"][i]["x"].asInt(), input["responses"][i]["y"].asInt());
	}
	placeAt(input["requests"][turnID]["x"].asInt(), input["requests"][turnID]["y"].asInt());
}

void ai_jxk::outputToBotzone()
{
	// 输出决策JSON
	Json::Value ret;
	int i = 0;
	Json::Value action;
	pair <int, int> pa = evaluate();
	action["x"] = pa.second - 1;
	action["y"] = pa.first - 1;
	ret["response"] = action;
	Json::FastWriter writer;
	cout << writer.write(ret) << endl;
	return ;
}

void ai_jxk::simpleInput()
{
	int round = 0;
	cin >> round;
	int x = 0, y = 0;
	cin >> x >> y;
	if (x == -1) {
		isblack = true;
		for (int i = 1; i < round; ++i) {
			cin >> x >> y;
			board[x + 1][y + 1] = 1;
			cin >> x >> y;
			board[x + 1][y + 1] = 2;
		}
	}
	else {
		isblack = false;
		board[x + 1][y + 1] = 1;
		for (int i = 1; i < round; ++i) {
			cin >> x >> y;
			board[x + 1][y + 1] = 2;
			cin >> x >> y;
			board[x + 1][y + 1] = 1;
		}
	}
	string data, globaldata;
	cin >> data;
	cin >> globaldata;
}

void ai_jxk::simpleOutput(pair<int, int> pa)
{
	cout << pa.first - 1 << " " << pa.second - 1 << endl;
	cout<< pa.first - 1 << " " << pa.second - 1 << endl;
	cout << "data" << endl;
	cout << "global data" << endl;
}

pair<int, int> ai_jxk::evaluate()
{
	//ai的局面分析函数，对于每一个点，先评估他是否能成为黑方或白方的攻点，
	// 再评估他能否成为黑方或白方的潜力点，再评估这个点的材料分（三步可能同时进行）
	//最终运算出一个最佳落子点，运算逻辑如下：先计算能否直接进攻取胜，再计算是否存在一些必须防守白方的杀点，
	// 最后再评估能不能形成最多的材料，如果黑棋形成了禁手会有扣分
	//函数的返回值：一个pair组，第一项是行，第二项是列，左上角认为是1，1
	
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这
	int color = (isblack ? 1 : 2);
	int nextcolor = (isblack ? 2 : 1);
	int isempty[17][17] = { {0} };//创建一个数组，记录棋盘上空的地方，空为0，有棋子为1

	string strs[4];//用来提供扫描的所有字符串

	//测试一下ai的禁手判断正不正确
	for (int row = 0; row < 17; ++row) {
		for (int col = 0; col < 17; ++col) {
			if (board[row][col] == 0) {
				forbidBoard[row][col] = (isForbid(row, col, color) ? 9 : 0);
			}
			else forbidBoard[row][col] = board[row][col];
		}
	}
	for (int row = 0; row < 17; ++row) {
		for (int col = 0; col < 17; ++col) {
			cout << forbidBoard[row][col] << " ";
		}
		cout << endl;
	}
	cout << endl;

	//遍历棋盘上除了边框的每一个点并完成字符串的扫描（不需要）
	//更新ise	mpty
	string allstr[17][17][4];
	for (int row = 1; row <= 15; ++row) {
		for (int col = 1; col <= 15; ++col) {
			//对每一个点board[row][col]，先扫描出来这条点对应的字符串strs，然后将strs存到更大的棋盘数组中
			//这样做的原因是，我们希望将检测攻点、潜力点和材料三步分开，但为了提升效率，不希望重新扫描每个点，所以存储起来
			if (board[row][col] == 0) {
				isempty[row][col] = 1;
				//scan(row, col);//只扫描空白，提高效率
			}
		}
	}
	for (int row = 0; row < 17; ++row) {
		for (int col = 0; col < 17; ++col) {
			cout << isempty[row][col] << " ";
		}
		cout << endl;
	}
	cout << endl;

	//如果该黑棋走棋
	if (isblack) {
		//调整进攻和防守的权重值
		atweight = 1, deweight = 1;
		
		//检测棋盘上有没有当黑棋赢点,如果有就直接在这落子获胜
		for (int row = 1; row <= 15; ++row) {
			for (int col = 1; col <= 15; ++col) {
				if (isempty[row][col]) {
					board[row][col] = 1;
					scan(row, col, strs);
					board[row][col] = 0;
					if (forbidBoard[row][col] != 9 && isFive(row, col, color, strs)) {
						return { row,col };
					}
				}
			}
		}

		//检测棋盘上有没有白棋赢点,如果有就直接在这落子防守
		for (int row = 1; row <= 15; ++row) {
			for (int col = 1; col <= 15; ++col) {
				if (isempty[row][col]) {
					board[row][col] = 2;
					scan(row, col, strs);
					board[row][col] = 0;
					if (isFive(row, col, nextcolor, strs)) {
						return { row,col };
					}
				}
			}
		}

		//遍历棋盘找到所有的潜力点和防守点，然后使用alpha-beta剪枝进行搜索
		int mapde[17][17] = { {0} }, mapun[17][17] = { {0} }, mapal[17][17] = { {0} };
		int sum1 = 0 , sum2 = 0;
		vector<pair<int, int>> develop, undevelop;
		pair<int, int> bestpa = { 8,8 };
		float best = 0;
		for (int row = 1; row <= 15; ++row) {
			for (int col = 1; col <= 15; ++col) {
				if (isempty[row][col]) {
					board[row][col] = 1;
					int output = isDevelopPoint(row, col, color);
					mapde[row][col] = output;
					if (output >= 6) develop.push_back({ row,col });
					board[row][col] = 2;
					int output2 = isDevelopPoint(row, col, nextcolor);
					mapun[row][col] = output2;
					mapal[row][col] = output + output2;
					sum1 += output, sum2 += output2;
					if (output2 >= 6) undevelop.push_back({ row,col });
					board[row][col] = 0;
					if ((atweight * output + deweight * output2) > best && forbidBoard[row][col] != 9) {
						best = atweight * output + deweight * output2;
						bestpa = { row,col };
					}
				}
			}
		}
		if (sum2 != 0) deweight = sum2 * 1.0 / sum1;
		cout << "dew:  " << deweight << endl;
		for (int row = 0; row < 17; ++row) {
			for (int col = 0; col < 17; ++col) {
				cout << mapde[row][col] << " ";
			}
			cout << endl;
		}
		cout << endl;
		for (int row = 0; row < 17; ++row) {
			for (int col = 0; col < 17; ++col) {
				cout << mapun[row][col] << " ";
			}
			cout << endl;
		}
		cout << endl;
		for (int row = 0; row < 17; ++row) {
			for (int col = 0; col < 17; ++col) {
				cout << mapal[row][col] << " ";
			}
			cout << endl;
		}
		cout << endl;
		//	cout << "develope:" << endl;
		//	for (auto pa : develop) {
		//		cout << pa.first << " " << pa.second << " " << endl;
		//	}
		//	cout << endl;
		//	for (auto pa : undevelop) {
		//		cout << pa.first << " " << pa.second << " " << endl;
		//	}
		//	cout << endl;
		// 
		// 
		//遍历棋盘检测所有黑棋强攻点和攻点
		vector<pair<int, int>> strongAttack, attack;// 标识强力攻点的坐标
		for (int row = 1; row <= 15; ++row) {
			for (int col = 1; col <= 15; ++col) {
				if (isempty[row][col]) {
					board[row][col] = 1;
					scan(row, col, strs);
					int output = isAttackPoint(row, col, color, strs);
					board[row][col] = 0;
					if (output == 2) strongAttack.push_back({ row,col });
					else if (output == 1) attack.push_back({ row,col });
				}
			}
		}
		cout << "attack and strong:" << endl;
		float bestStrongAttack = 0;
		pair<int, int> bestatpa = { -1,-1 };
		for (auto pa : strongAttack) {//比较一下几个强攻点的好坏
			cout << pa.first << " " << pa.second << " " << endl;
			if (forbidBoard[pa.first][pa.second] != 9) {
				int output = isDevelopPoint(pa.first, pa.second, color);
				int output2 = isDevelopPoint(pa.first, pa.second, nextcolor);
				if ((atweight * output + deweight * output2) > bestStrongAttack) {
					bestStrongAttack = atweight * output + deweight * output2;
					bestatpa = pa;
				}
			}
		}
		if (bestatpa.first != -1) return bestatpa;
		cout << endl;
		for (auto pa : attack) {
			cout << pa.first << " " << pa.second << " " << endl;
		}
		cout << endl;

		//遍历棋盘检测所有白棋的攻点和强攻点
		vector<pair<int, int>> strongDefense, defense;
		for (int row = 1; row <= 15; ++row) {
			for (int col = 1; col <= 15; ++col) {
				if (isempty[row][col]) {
					board[row][col] = 2;
					scan(row, col, strs);
					//if (isFourA(row, col, nextcolor, strs)) strongDefense.push_back({ row,col });
					int output = isAttackPoint(row, col, nextcolor, strs);
					board[row][col] = 0;
					if (output == 2) strongDefense.push_back({ row,col });
					else if (output == 1) defense.push_back({ row,col });
				}
			}
		}
		cout << "defense and strong:" << endl;
		float bestStrongDefense = 0;
		pair<int, int> bestdepa = { -1,-1 };
		for (auto pa : strongDefense) {//比较一下几个防守点的好坏
			cout << pa.first << " " << pa.second << " " << endl;
			int output = isDevelopPoint(pa.first, pa.second, color);
			int output2 = isDevelopPoint(pa.first, pa.second, nextcolor);
			if ((atweight*output + deweight*output2) > bestStrongDefense) {
				bestStrongDefense = atweight * output + deweight * output2;
				bestdepa = pa;
			}
		}
		cout << endl;
		for (auto pa : defense) {
			cout << pa.first << " " << pa.second << " " << endl;
		}


		cout << endl;
		if (bestdepa.first != -1) return bestdepa;
		cout << "develop:" << endl;
		float bestDevelop= -10;
		for (auto pa : develop) {//比较一下几个发展点的好坏
			if (forbidBoard[pa.first][pa.second] != 9) {
				int output = calculateDevelop(pa.first, pa.second, color, 0, INT_MIN, INT_MAX, develop, undevelop);
				if (output > bestDevelop) {
					bestStrongAttack = output;
					bestatpa = pa;
				}
			}
		}
		for (auto pa : undevelop) {//比较一下几个发展点的好坏
			if (forbidBoard[pa.first][pa.second] != 9) {
				int output = calculateDevelop(pa.first, pa.second, color, 0, INT_MIN, INT_MAX, develop, undevelop);
				if (output > bestDevelop) {
					bestStrongAttack = output;
					bestatpa = pa;
				}
			}
		}
		return bestpa;
	}


	//白棋
	else {//如果该白棋走棋
		//调整进攻和防守的权重值
		atweight = 1, deweight = 1.75;

		//检测棋盘上有没有白棋赢点,如果有就直接在这落子获胜
		for (int row = 1; row <= 15; ++row) {
			for (int col = 1; col <= 15; ++col) {
				if (isempty[row][col]) {
					board[row][col] = 2;
					scan(row, col, strs);
					board[row][col] = 0;
					if (isFive(row, col, color, strs)) {
						return { row,col };
					}
				}
			}
		}

		//检测棋盘上有没有黑棋赢点,如果有就直接在这落子防守
		for (int row = 1; row <= 15; ++row) {
			for (int col = 1; col <= 15; ++col) {
				if (isempty[row][col]) {
					board[row][col] = 1;
					scan(row, col, strs);
					board[row][col] = 0;
					if (forbidBoard[row][col] != 9 && isFive(row, col, nextcolor, strs)) {
						return { row,col };
					}
				}
			}
		}

		//遍历棋盘找到所有的潜力点和防守点，然后使用alpha-beta剪枝进行搜索
		int sum1 = 0, sum2 = 0;
		int mapde[17][17] = { {0} }, mapun[17][17] = { {0} }, mapal[17][17] = { {0} };
		vector<pair<int, int>> develop, undevelop;
		pair<int, int> bestpa = { 8,8 };
		int dir[8][2] = { {1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1},{0,1} };
		if (step <= 2 && board[8][8] != 0) {
			random_device rd2;
			uniform_int_distribution<>	num(0, 7);
			int ran = 0;
			ran = num(rd2);
			while (board[8 + dir[ran][0]][8 + dir[ran][1]] != 0) {
				ran = num(rd2);
			}
			bestpa = { 8 + dir[ran][0],8 + dir[ran][1] };
		}
		float best = 0;
		for (int row = 1; row <= 15; ++row) {
			for (int col = 1; col <= 15; ++col) {
				if (isempty[row][col]) {
					board[row][col] = 2;
					scan(row, col, strs);
					int output = isDevelopPoint(row, col, color);
					if (output >= 8) develop.push_back({ row,col });
					board[row][col] = 1;
					scan(row, col, strs);
					int output2 = isDevelopPoint(row, col, nextcolor);
					if (output2 >= 8) undevelop.push_back({ row,col });
					board[row][col] = 0;
					mapde[row][col] = output, mapun[row][col] = output2, mapal[row][col] = output + output2;
					sum1 += output, sum2 += output2;
					if ((atweight * output + deweight * output2) > best) {
						best = atweight * output + deweight * output2;
						bestpa = { row,col };
					}
				}
			}
		}
		deweight = sum2 * 1.0 / sum1;
		cout << "dew:  " << deweight << endl;
		for (int row = 0; row < 17; ++row) {
			for (int col = 0; col < 17; ++col) {
				cout << mapde[row][col] << " ";
			}
			cout << endl;
		}
		cout << endl;
		for (int row = 0; row < 17; ++row) {
			for (int col = 0; col < 17; ++col) {
				cout << mapun[row][col] << " ";
			}
			cout << endl;
		}
		cout << endl;
		for (int row = 0; row < 17; ++row) {
			for (int col = 0; col < 17; ++col) {
				cout << mapal[row][col] << " ";
			}
			cout << endl;
		}
		//cout << "develope:" << endl;
		//for (auto pa : develop) {
		//	cout << pa.first << " " << pa.second << " " << endl;
		//}
		//cout << endl;
		//for (auto pa : undevelop) {
		//	cout << pa.first << " " << pa.second << " " << endl;
		//}
		//cout << endl;
		// 
		// 
		//遍历棋盘检测所有白棋的强攻点和攻点
		vector<pair<int, int>> strongAttack, attack;// 标识强力攻点的坐标
		for (int row = 1; row <= 15; ++row) {
			for (int col = 1; col <= 15; ++col) {
				if (isempty[row][col]) {
					board[row][col] = 2;
					scan(row, col, strs);
					int output = isAttackPoint(row, col, color, strs);
					board[row][col] = 0;
					if (output == 2) strongAttack.push_back({ row,col });
					else if (output == 1) attack.push_back({ row,col });
				}
			}
		}
		cout << "attack and strong:" << endl;
		float bestStrongAttack = 0;
		pair<int, int> bestatpa = { -1,-1 };
		for (auto pa : strongAttack) {//比较一下几个强攻点的好坏
			cout << pa.first << " " << pa.second << " " << endl;
			int output = isDevelopPoint(pa.first, pa.second, color);
			int output2 = isDevelopPoint(pa.first, pa.second, nextcolor);
			if ((atweight*output + deweight*output2) > bestStrongAttack) {
				bestStrongAttack = atweight * output + deweight * output2;
				bestatpa = pa;
			}
		}
		if (bestatpa.first != -1) return bestatpa;
		cout << endl;
		for (auto pa : attack) {
			cout << pa.first << " " << pa.second << " " << endl;
		}
		cout << endl;

		//遍历棋盘检测所有黑棋的攻点和强攻点
		vector<pair<int, int>> strongDefense, defense;
		for (int row = 1; row <= 15; ++row) {
			for (int col = 1; col <= 15; ++col) {
				if (isempty[row][col]) {
					board[row][col] = 1;
					scan(row, col, strs);
					//if (isFourA(row, col, nextcolor, strs)) strongDefense.push_back({ row,col });
					int output = isAttackPoint(row, col, nextcolor, strs);
					board[row][col] = 0;
					if (output == 2) strongDefense.push_back({ row,col });
					else if (output == 1) defense.push_back({ row,col });
				}
			}
		}
		cout << "defense and strong:" << endl;
		float bestStrongDefense = 0;
		pair<int, int> bestdepa = {  -1,-1 };
		for (auto pa : strongDefense) {//比较一下几个防守点的好坏
			cout << pa.first << " " << pa.second << " " << endl;
			if (forbidBoard[pa.first][pa.second] != 9) {//防守点是禁手就可以不防守
				int output = isDevelopPoint(pa.first, pa.second, color);
				int output2 = isDevelopPoint(pa.first, pa.second, nextcolor);
				if ((atweight * output + deweight * output2) > bestStrongDefense) {
					bestStrongDefense = atweight * output + deweight * output2;
					bestdepa = pa;
				}
			}
		}
		cout << endl;
		for (auto pa : defense) {
			cout << pa.first << " " << pa.second << " " << endl;
		}
		cout << endl;
		if (bestdepa.first != -1) return bestdepa;

		return bestpa;
		}
	//随机落子算法
    random_device rd;
    uniform_int_distribution<>	num(1, boardSize);
    int px, py;
    px = num(rd);
    py = num(rd);
    //std::cout << px << " " << py;
	return { px,py };//走棋之后返回，结束函数
}

int ai_jxk::calculateAttack(int row, int col, int color, int depth, vector<pair<int, int>> strongAttack,
	vector<pair<int, int>> attack, vector<pair<int, int>> strongDefense, vector<pair<int, int>> defense)
{
	return 0;
}

int ai_jxk::calculateDevelop(int row, int col, int color, int depth, int min, int max,
	vector<pair<int, int>> develop, vector<pair<int, int>> undevelop)
{
	//要求判断的位置必须为空
	int nextcolor = (color == 1 ? 2 : 1);
	int score = 0;
	board[row][col] = color;
	string strs[4];
	if (depth >= developLimit) {
		for (int x = 1; x <= 15; x++) {
			for (int y = 1; y <= 15; y++) {
				scanForbid(x, y, strs);
				score += isAttackPoint(x, y, color, strs);
				score -= isAttackPoint(x, y, nextcolor, strs);
			}
		}
		board[row][col] = 0;
		return score;
	}

	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//四个方向，水平，竖直，正斜，反斜
	int k = 0;
	int x, y;
	for (int l = 0; l < 4; l++) {
		//正方向
		k = 0;//用0包括自己
		while (k <= 4) {
			x = row + dir[l][0] * k, y = col + dir[l][1] * k;
			if (board[x][y] < 0) break;//遇到边界就停止扫描
			if (isDevelopPoint(x, y, color) >= 4) develop.push_back({ x,y });
			if (isDevelopPoint(x, y, nextcolor >= 4)) develop.push_back({ x,y });
			k += 1;
		}
		//反方向
		k = 1;
		while (k <= 4) {
			x = row - dir[l][0] * k, y = col - dir[l][1] * k;
			if (board[x][y] < 0) break;//遇到边界就停止扫描
			if (isDevelopPoint(x, y, color) >= 4) develop.push_back({ x,y });
			if (isDevelopPoint(x, y, nextcolor >= 4)) develop.push_back({ x,y });
			k += 1;
		}
	}


	int maxScore = min, minScore = max, output = 0;
	for (auto pa : develop) {
		if (board[pa.first][pa.second] == 0) {
			board[pa.first][pa.second] = color;
			output = calculateDevelop(pa.first, pa.second, nextcolor, depth + 1, minScore, maxScore, undevelop, develop);
			board[pa.first][pa.second] = 0;
		}
		if (color == 1) {
			if (output > maxScore) maxScore = output;
			if (maxScore >= max) break; // 剪枝
		}
		else {
			if (output < minScore) minScore = output;
			if (minScore <= min) break; // 剪枝
		}
	}
	board[row][col] = 0;
	return (color == 1) ? maxScore : minScore;
}


int ai_jxk::calculateScore(int row, int col, int color, string(&strs)[4])
{
	//辅助函数，检测一个点是不是禁手
//注意由于只有黑棋有禁手，所以默认是黑棋，color不起作用

	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//四个方向，水平，竖直，正斜，反斜
	//落子并扫描
	int blackScore = 0, whiteScore = 0;
	scanForbid(row, col, strs);
	//同时标记哪些方向已经有过4了，有4的方向不再检测3
	int isfour[4] = { 0 };
	for (int i = 0; i < 4; i++) {
		bool checkline = true;
		for (string str : blackfournr) {
			if (strs[i].find(str) != string::npos) {
				checkline = false;//这种情况只能算是一个冲四
				isfour[i] = 1;
				blackScore += 500;
				break;
			}
		}
		for (string str : blackfoura) {//有活四，大概率直接取胜
			if (checkline && strs[i].find(str) != string::npos) {
				isfour[i] = 1;
				blackScore += 10000;
				break;
			}
		}
		for (string str : blackfourd) {//有冲四
			if (checkline && strs[i].find(str) != string::npos) {
				isfour[i] = 1;
				blackScore += 500;
				break;
			}
		}
	}

	//随后开始判断三，有4的方向不再检测3
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			//检测四条线上是否存在（眠三）
			for (string str : blackthreenr) {
				if (strs[i].find(str) != string::npos) {
					blackScore += 200;
					break;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : blackthreej) {
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;//已经有跳活三就不可能有连活三
					blackScore += 300;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : blackthreec) {//连活三
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					blackScore += 500;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : blackthreed) {//眠三
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					blackScore += 200;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : blackthrees) {//特型三
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					blackScore += 250;
					break;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : blacktwoa) {//活二
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					blackScore += 150;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : blacktwoa) {//眠二
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					blackScore += 50;
					break;
				}
			}
		}
	}
	//同时标记哪些方向已经有过4了，有4的方向不再检测3
	isfour[0] = 0, isfour[1] = 0, isfour[2] = 0, isfour[3] = 0;
	for (int i = 0; i < 4; i++) {
		bool checkline = true;
		for (string str : whitefoura) {//有活四，大概率直接取胜
			if (checkline && strs[i].find(str) != string::npos) {
				isfour[i] = 1;
				whiteScore += 10000;
				break;
			}
		}
		for (string str : whitefourd) {//有冲四
			if (checkline && strs[i].find(str) != string::npos) {
				isfour[i] = 1;
				whiteScore += 500;
				break;
			}
		}
	}

	//随后开始判断三，有4的方向不再检测3
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : whitethreej) {
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;//已经有跳活三就不可能有连活三
					whiteScore += 300;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : whitethreec) {//连活三
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					whiteScore += 500;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : whitethreed) {//眠三
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					whiteScore += 200;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : whitethrees) {//特型三
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					whiteScore += 250;
					break;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : whitetwoa) {//活二
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					whiteScore += 150;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : whitetwod) {//眠二
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					blackScore += 50;
					break;
				}
			}
		}
	}
	return blackScore - whiteScore;
}

void ai_jxk::scan(int row, int col,string (&strs)[4])
{
	//辅助函数，对于棋盘上的一个坐标点，扫描以该子为中心，距离不超过四个棋子的四个方向，返回四条长度为9的字符串
	//注意这个函数扫描时不会改变棋盘的状态，所以要进行回溯
	//注意这个函数为了提高效率，不通过返回值传递，而是直接更改已经创建的字符串，不要求字符串为空，只要是str[4]即可

	for (int i = 0; i < 4; ++i) strs[i] = "";//把每个字符串清空
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//四个方向，水平，竖直，正斜，反斜
	int k = 0;
	int x, y;
	for (int l = 0; l < 4; l++) {
		//正方向
		k = 0;//用0包括自己
		while (k <= 4) {
			x = row + dir[l][0] * k, y = col + dir[l][1] * k;
			strs[l] = strs[l] + intToChar(board[x][y]);
			if (board[x][y] < 0) break;//遇到边界就停止扫描
			k += 1;
		}
		while (k <= 4) {
			strs[l] = strs[l] + '-';//强行补齐至9个，便于之后禁手判断和操作
			k += 1;
		}
		//反方向
		k = 1;
		while (k <= 4) {
			x = row - dir[l][0] * k, y = col - dir[l][1] * k;
			strs[l] = intToChar(board[x][y]) + strs[l];
			if (board[x][y] < 0) break;//遇到边界就停止扫描
			k += 1;
		}
		while (k <= 4) {
			strs[l] = '-' + strs[l];
			k += 1;
		}
	}
	return;
}

void ai_jxk::scanForbid(int row, int col, string(&strs)[4])
{
	for (int i = 0; i < 4; ++i) strs[i] = "";//把每个字符串清空
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//四个方向，水平，竖直，正斜，反斜
	int k = 0;
	int x, y;
	for (int l = 0; l < 4; l++) {
		//正方向
		k = 0;//用0包括自己
		while (k <= 4) {
			x = row + dir[l][0] * k, y = col + dir[l][1] * k;
			strs[l] = strs[l] + intToChar(forbidBoard[x][y]);
			if (forbidBoard[x][y] < 0) break;//遇到边界就停止扫描
			k += 1;
		}
		while (k <= 4) {
			strs[l] = strs[l] + '-';//强行补齐至9个，便于之后禁手判断和操作
			k += 1;
		}
		//反方向
		k = 1;
		while (k <= 4) {
			x = row - dir[l][0] * k, y = col - dir[l][1] * k;
			strs[l] = intToChar(forbidBoard[x][y]) + strs[l];
			if (forbidBoard[x][y] < 0) break;//遇到边界就停止扫描
			k += 1;
		}
		while (k <= 4) {
			strs[l] = '-' + strs[l];
			k += 1;
		}
	}
	return;
}

char ai_jxk::intToChar(int piece)
{
	//辅助函数，在把棋盘扫描成字符串的时候，把每一个int类型的棋子变成char类型的
	//棋盘记录棋子的规则如下：黑棋用1表示，白棋用2表示，空白用0表示，边框用-1表示，不存在其他情况

	switch (piece)
	{
	case -1:
		return '-';
	case 0:
		return '0';
	case 1:
		return '1';
	case 2:
		return '2';
	case 9:
		return '9';
	default:
		return 'X';//报错
	}
	return 0;
}

bool ai_jxk::isForbid(int row, int col, int color)
{
	//辅助函数，检测一个点是不是禁手
	//注意由于只有黑棋有禁手，所以默认是黑棋，color不起作用

	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//四个方向，水平，竖直，正斜，反斜
	//落子并扫描
	board[row][col] = 1;//假设在这个地方落黑棋，最后回溯
	string strs[4];
	scan(row, col, strs);

	//对每一个方向，先判断是不是长连，再判断是不是连五
	for (int i = 0; i < 4; i++) {
		for (string str : blacksix) {
			if (strs[i].find(str) != string::npos) {
				board[row][col] = 0;//回溯
				return true;
			}
		}
		for (string str : blackfive) {
			if (strs[i].find(str) != string::npos) {
				board[row][col] = 0;//回溯
				return false;
			}
		}
	}
	
	//随后开始判断四四禁手，同时标记哪些方向已经有过4了，有4的方向不再检测3
	int isfour[4] = { 0 };
	int fourcount = 0;
	for (int i = 0; i < 4; i++) {
		//先检测一条线上有没有可能有两个活四
		bool checkline = true;
		for (string str : blackfournr) {
			if (strs[i].find(str) != string::npos) {
				checkline = false;//这种情况只能算是一个活四
				isfour[i] = 1;
				++fourcount;
				break;
			}
		}
		for (string str : blackfourr) {
			if (checkline&&strs[i].find(str) != string::npos) {
				board[row][col] = 0;//回溯
				return true;
			}
		}
		for (string str : blackfour) {
			if (checkline && strs[i].find(str) != string::npos) {
				++fourcount;
				isfour[i] = 1;
				break;
			}
		}
	}
	if (fourcount >= 2) {//检测到超过两个四，是禁手
		board[row][col] = 0;//回溯
		return true;
	}

	//随后开始判断三三禁手有4的方向不再检测3
	int threecount = 0;
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			//检测四条线上是否不存在活三
			for (string str : blackthreenr) {
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					break;
				}
			}
		}
	}

	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			//检测一条线上有没有跳活三，如果有需要递归判断跳活三的中间点
			for (string str : blackthreej) {
				//if (strs[i].find(str) != string::npos) {
				//	//找到需要递归的位置
				//	int start = str.find("011010");
				//	if (str.find("011010") != string::npos) {
				//		//计算递归点和中心点的相对距离,推导出递归坐标
				//		int idx = start + 3;
				//		int dis = idx - 4;
				//		int r = row + dir[i][0] * dis, c = col + dir[i][1] * dis;
				//		if (!isForbid(r, c, color)); {
				//			isfour[i] = 1;//已经有跳活三就不可能有连活三
				//			++threecount;
				//			break;
				//		}
				//	}
				//}
				if (strs[i].find(str) != string::npos) {
					////找到需要递归的位置
					//int start = str.find("010110");
					//if (str.find("010110") != string::npos) {
					//	//计算递归点和中心点的相对距离,推导出递归坐标
					//	int idx = start + 2;
					//	int dis = idx - 4;
					//	int r = row + dir[i][0] * dis, c = col + dir[i][1] * dis;
					//	if (!isForbid(r, c, color)); {
					//		isfour[i] = 1;//已经有跳活三就不可能有连活三
					//		++threecount;
					//		break;
					//	}
					//}
					isfour[i] = 1;//已经有跳活三就不可能有连活三
					++threecount;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			//再检测一条线上有没有连活三，如果有需要递归判断连活三两侧的点
			for (string str : blackthreec) {
				//if (strs[i].find(str) != string::npos) {
				//	//找到需要递归的位置
				//	int start = strs[i].find("011100");
				//	if (strs[i].find("011100") != string::npos) {
				//		//计算递归点和中心点的相对距离,推导出递归坐标
				//		int idx = start + 4;
				//		int dis = idx - 4;
				//		int r = row + dir[i][0] * dis, c = col + dir[i][1] * dis;
				//		if (!isForbid(r, c, color)); {
				//			++threecount;
				//			break;
				//		}
				//	}
				//}
				if (strs[i].find(str) != string::npos) {
					////找到需要递归的位置
					//int start = strs[i].find("001110");
					//if (strs[i].find("001110") != string::npos) {
					//	//计算递归点和中心点的相对距离,推导出递归坐标
					//	int idx = start + 2;
					//	int dis = idx - 4;
					//	int r = row + dir[i][0] * dis, c = col + dir[i][1] * dis;
					//	if (!isForbid(r, c, color)); {
					//		++threecount;
					//		break;
					//	}
					//}
					//暂时放弃递归，由于递归较为影响效率
					threecount++;
					break;
				}
			}
		}
	}
	if (threecount >= 2) {//检测到超过两个三，是禁手
		board[row][col] = 0;//回溯
		return true;
	}
	board[row][col] = 0;//回溯
	return false;
}

//bool ai_jxk::checkWinPoint(int row, int col, int color, int(&board)[17][17])
//{
//	//辅助函数，用来检测一个点是不是赢点，需要检测禁手
//	//由于需要检测禁手，所以需要传递整个棋盘
//
//
//	return false;
//}

int ai_jxk::isAttackPoint(int row, int col, int color, string(&strs)[4])
{
	//辅助函数，用来检测一个点是不是攻点，此时不检测禁手，只要在这个前提下形成了活三、活四、连五就可以
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这
	//返回int值，如果是0就不是攻点，1是攻点，2是强力攻点(因为我们没有判断禁手，所以说对黑棋白棋双三都会是强力攻点
	//因为可能是一个假三三

	//先看是不是形成了活四，如果是活四，也认为是强攻点
	if (isFourA(row, col, color, strs)) return 2;
	
	//如果是白棋，抓禁手也可以认为是一个强攻点
	if (color == 2) {
		string forbidstr[4];
		int a = forbidBoard[row][col];
		forbidBoard[row][col] = 2;
		scanForbid(row, col, forbidstr);
		forbidBoard[row][col] = a;
		if (isCatch(row, col, color, forbidstr)) return 2;
	}
	//看能不能形成两个三或四
	int three = countThreeandFour(row, col, color, strs);
	bool five = isFive(row, col, color, strs);
	if (three + five >= 2) return 2;
	else if (three + five == 1) return 1;
	else return 0;
}

int ai_jxk::isDevelopPoint(int row, int col, int color)
{
	//辅助函数，用来检测一个点是不是攻点，此时不检测禁手，只要在这个前提下形成了两个攻点
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这
	//返回值代表这个点这一步新形成了多少攻点
	//如果是黑棋，额外添加禁手判断，如果创造了禁手减五分
	int count = 0,before = 0;
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//四个方向，水平，竖直，正斜，反斜
	int k = 0;
	int x = 0, y = 0;
	//遍历棋盘,扫描所有方向
	for (int l = 0; l < 4; l++) {
		//正方向
		k = 1;//不包括自己
		while (k <= 4) {
			x = row + dir[l][0] * k, y = col + dir[l][1] * k;
			if (board[x][y] < 0) break;//遇到边界就停止扫描
			if (board[x][y] != 0) {
				k += 1;
				continue;
			}
			board[x][y] = color;
			string strs[4];
			scan(x, y, strs);
			if (isAttackPoint(x, y, color, strs) == 1) ++count;
			else if (isAttackPoint(x, y, color, strs) == 2) count += 2;
			if (color == 1) {
				if (isForbid(x, y, color)) count -= 5;
			}
			board[x][y] = 0;
			k += 1;
		}
		//反方向
		k = 1;
		while (k <= 4) {
			x = row - dir[l][0] * k, y = col - dir[l][1] * k;
			if (board[x][y] < 0) break;//遇到边界就停止扫描
			if (board[x][y] != 0) {
				k += 1;
				continue;
			}
			board[x][y] = color;
			string strs[4];
			scan(x, y, strs);
			if (isAttackPoint(x, y, color, strs) == 1) ++count;
			else if (isAttackPoint(x, y, color, strs) == 2) count += 2;
			if (color == 1) {
				if (isForbid(x, y, color)) count -= 5;
			}
			board[x][y] = 0;
			k += 1;
		}
	}
	if (color == 2) {//对于白棋，如果落在黑棋禁手上，要适当减分
		if (isForbid(row, col, color)) count -= 3;
	}
	return count;
}

bool ai_jxk::isThreeA(int row, int col, int color, string (&strs)[4])
{
	//辅助函数，用来检测一个点有没有形成活三（不算禁手)
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这

	if (color == 1) {
		//如果是黑棋
		for (int i = 0; i < 4; i++) {
			for (string str : blackthreea) {
				if (strs[i].find(str) != string::npos) {
					return true;
				}
			}
		}
		return false;
	}
	else {
		//如果是白棋
		for (int i = 0; i < 4; i++) {
			for (string str : whitethreea) {
				if (strs[i].find(str) != string::npos) {
					return true;
				}
			}
		}
		return false;
	}
}

int ai_jxk::countThreeA(int row, int col, int color, string(&strs)[4])
{
	//辅助函数，用来检测一个点有没有形成活三（不算禁手)
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这
	int count = 0;
	if (color == 1) {
		//如果是黑棋
		for (int i = 0; i < 4; i++) {
			for (string str : blackthreea) {
				if (strs[i].find(str) != string::npos) {
					++count;
					break;
				}
			}
		}
		return count;
	}
	else {
		//如果是白棋
		for (int i = 0; i < 4; i++) {
			for (string str : whitethreea) {
				if (strs[i].find(str) != string::npos) {
					++count;
					break;
				}
			}
		}
		return count;
	}
}

bool ai_jxk::isFour(int row, int col, int color, string(&strs)[4])
{
	//辅助函数，用来检测一个点有没有成四（不算禁手)
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这

	if (color == 1) {
		//如果是黑棋
		for (int i = 0; i < 4; i++) {
			for (string str : blackfour) {
				if (strs[i].find(str) != string::npos) {
					return true;
					for (int i = 0; i < 4; i++) strs[i][4] = color;
				}
			}
		}
		return false;
	}
	else {
		//如果是白棋
		for (int i = 0; i < 4; i++) {
			for (string str : whitefour) {
				if (strs[i].find(str) != string::npos) {
					return true;
				}
			}
		}
		return false;
	}
}

int ai_jxk::countFour(int row, int col, int color, string(&strs)[4])
{
	//辅助函数，用来检测一个点有几个成四（不算禁手)
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这

	int count = 0;
	if (color == 1) {
		//如果是黑棋
		for (int i = 0; i < 4; i++) {
			for (string str : blackfour) {
				if (strs[i].find(str) != string::npos) {
					++count;
					break;
				}
			}
		}
		return count;
	}
	else {
		//如果是白棋
		for (int i = 0; i < 4; i++) {
			for (string str : whitefour) {
				if (strs[i].find(str) != string::npos) {
					++count;
					break;
				}
			}
		}
		return count;
	}
}

int ai_jxk::countThreeandFour(int row, int col, int color, string(&strs)[4]) {
	//辅助函数，用来检测一个点有没有成四（不算禁手)
//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这

	if (color == 1) {
		//如果是黑棋
		int count = 0;
		int four[4] = { 0 };
		for (int i = 0; i < 4; i++) {
			for (string str : blackfour) {
				if (strs[i].find(str) != string::npos) {
					count++;
					four[i] = 1;
					break;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			for (string str : blackthreea) {
				if (!four[i]&&strs[i].find(str) != string::npos) {
					++count;
					break;
				}
			}
		}
		return count;
	}
	else {
		int count = 0;
		int four[4] = { 0 };
		//如果是白棋
		for (int i = 0; i < 4; i++) {
			int four[4] = { 0 };
			for (string str : whitefour) {
				if (strs[i].find(str) != string::npos) {
					count++;
					four[i] = 1;
					break;
				}
			}
		}
		for (int i = 0; i < 4; i++) {
			for (string str : whitethreea) {
				if (!four[i]&&strs[i].find(str) != string::npos) {
					++count;
					break;
				}
			}
		}
		return count;
	}
}

bool ai_jxk::isFourA(int row, int col, int color, string(&strs)[4])
{
	//辅助函数，用来检测一个能否活四（不算禁手)
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这

	if (color == 1) {
		//如果是黑棋
		for (int i = 0; i < 4; i++) {
			for (string str : blackfoura) {
				if (strs[i].find(str) != string::npos) {
					return true;
				}
			}
		}
		return false;
	}
	else {
		//如果是白棋
		for (int i = 0; i < 4; i++) {
			for (string str : whitefoura) {
				if (strs[i].find(str) != string::npos) {
					return true;
				}
			}
		}
		return false;
	}
}

bool ai_jxk::isFive(int row, int col, int color, string(&strs)[4])
{
	//辅助函数，用来检测一个点有没有成五（不算禁手)
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这

	if (color == 1) {
		//如果是黑棋
		for (int i = 0; i < 4; i++) {
			for (string str : blackfive) {
				if (strs[i].find(str) != string::npos) return true;
			}
		}
		return false;
	}
	else {
		//如果是白棋
		for (int i = 0; i < 4; i++) {
			for (string str : whitefive) {
				if (strs[i].find(str) != string::npos) return true;
			}
		}
		return false;
	}
}

bool ai_jxk::isCatch(int row, int col, int color, string(&strs)[4])
{
	for (int i = 0; i < 4; i++) {
		for (string str : whitecatch) {
			if (strs[i].find(str) != string::npos) return true;
		}
	}
	return false;
}
