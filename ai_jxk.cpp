#include "ai_jxk.h"
#include<string>
#include<vector>
#include<random>
#include<utility>
#include<climits>
#include<iostream>
#include <sstream>
#include "jsoncpp/json.h" // C++����ʱĬ�ϰ����˿�
using namespace std;
void ai_jxk::inputFromGame(vector<vector<crossPointState>> boardInfo)
{
	//��������ñ����̵Ĵ洢��ʽ��crosspointstate����������������һ�����棬���õ�������߿�
	init();
	step = 0;//�ж��Ѿ����˼���
	for (int i = 0; i <= boardSize + 1; ++i) {
		for (int j = 0; j <= boardSize + 1; ++j) {
			switch (boardInfo[i][j])
			{
			case isBPiece:
				//���̼�¼���ӵĹ������£�������1��ʾ��������2��ʾ���հ���0��ʾ���߿���-1��ʾ���������������
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

	//�жϵ�ǰ�������ߺ��廹�ǰ���
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
	// ����JSON
	string str;
	getline(cin, str);
	Json::Reader reader;
	Json::Value input;
	reader.parse(str, input);
	// �����Լ��յ���������Լ���������������ָ�״̬
	int turnID = input["responses"].size();
	for (int i = 0; i < turnID; i++) {
		placeAt(input["requests"][i]["x"].asInt(), input["requests"][i]["y"].asInt());
		placeAt(input["responses"][i]["x"].asInt(), input["responses"][i]["y"].asInt());
	}
	placeAt(input["requests"][turnID]["x"].asInt(), input["requests"][turnID]["y"].asInt());
}

void ai_jxk::outputToBotzone()
{
	// �������JSON
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
	//ai�ľ����������������ÿһ���㣬���������Ƿ��ܳ�Ϊ�ڷ���׷��Ĺ��㣬
	// ���������ܷ��Ϊ�ڷ���׷���Ǳ���㣬�����������Ĳ��Ϸ֣���������ͬʱ���У�
	//���������һ��������ӵ㣬�����߼����£��ȼ����ܷ�ֱ�ӽ���ȡʤ���ټ����Ƿ����һЩ������ذ׷���ɱ�㣬
	// ����������ܲ����γ����Ĳ��ϣ���������γ��˽��ֻ��п۷�
	//�����ķ���ֵ��һ��pair�飬��һ�����У��ڶ������У����Ͻ���Ϊ��1��1
	
	//color������ɫ���������£�1�����������[row][col],2�������������
	int color = (isblack ? 1 : 2);
	int nextcolor = (isblack ? 2 : 1);
	int isempty[17][17] = { {0} };//����һ�����飬��¼�����Ͽյĵط�����Ϊ0��������Ϊ1

	string strs[4];//�����ṩɨ��������ַ���

	//����һ��ai�Ľ����ж�������ȷ
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

	//���������ϳ��˱߿��ÿһ���㲢����ַ�����ɨ�裨����Ҫ��
	//����ise	mpty
	string allstr[17][17][4];
	for (int row = 1; row <= 15; ++row) {
		for (int col = 1; col <= 15; ++col) {
			//��ÿһ����board[row][col]����ɨ������������Ӧ���ַ���strs��Ȼ��strs�浽���������������
			//��������ԭ���ǣ�����ϣ������⹥�㡢Ǳ����Ͳ��������ֿ�����Ϊ������Ч�ʣ���ϣ������ɨ��ÿ���㣬���Դ洢����
			if (board[row][col] == 0) {
				isempty[row][col] = 1;
				//scan(row, col);//ֻɨ��հף����Ч��
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

	//����ú�������
	if (isblack) {
		//���������ͷ��ص�Ȩ��ֵ
		atweight = 1, deweight = 1;
		
		//�����������û�е�����Ӯ��,����о�ֱ���������ӻ�ʤ
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

		//�����������û�а���Ӯ��,����о�ֱ���������ӷ���
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

		//���������ҵ����е�Ǳ����ͷ��ص㣬Ȼ��ʹ��alpha-beta��֦��������
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
		//�������̼�����к���ǿ����͹���
		vector<pair<int, int>> strongAttack, attack;// ��ʶǿ�����������
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
		for (auto pa : strongAttack) {//�Ƚ�һ�¼���ǿ����ĺû�
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

		//�������̼�����а���Ĺ����ǿ����
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
		for (auto pa : strongDefense) {//�Ƚ�һ�¼������ص�ĺû�
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
		for (auto pa : develop) {//�Ƚ�һ�¼�����չ��ĺû�
			if (forbidBoard[pa.first][pa.second] != 9) {
				int output = calculateDevelop(pa.first, pa.second, color, 0, INT_MIN, INT_MAX, develop, undevelop);
				if (output > bestDevelop) {
					bestStrongAttack = output;
					bestatpa = pa;
				}
			}
		}
		for (auto pa : undevelop) {//�Ƚ�һ�¼�����չ��ĺû�
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


	//����
	else {//����ð�������
		//���������ͷ��ص�Ȩ��ֵ
		atweight = 1, deweight = 1.75;

		//�����������û�а���Ӯ��,����о�ֱ���������ӻ�ʤ
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

		//�����������û�к���Ӯ��,����о�ֱ���������ӷ���
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

		//���������ҵ����е�Ǳ����ͷ��ص㣬Ȼ��ʹ��alpha-beta��֦��������
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
		//�������̼�����а����ǿ����͹���
		vector<pair<int, int>> strongAttack, attack;// ��ʶǿ�����������
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
		for (auto pa : strongAttack) {//�Ƚ�һ�¼���ǿ����ĺû�
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

		//�������̼�����к���Ĺ����ǿ����
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
		for (auto pa : strongDefense) {//�Ƚ�һ�¼������ص�ĺû�
			cout << pa.first << " " << pa.second << " " << endl;
			if (forbidBoard[pa.first][pa.second] != 9) {//���ص��ǽ��־Ϳ��Բ�����
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
	//��������㷨
    random_device rd;
    uniform_int_distribution<>	num(1, boardSize);
    int px, py;
    px = num(rd);
    py = num(rd);
    //std::cout << px << " " << py;
	return { px,py };//����֮�󷵻أ���������
}

int ai_jxk::calculateAttack(int row, int col, int color, int depth, vector<pair<int, int>> strongAttack,
	vector<pair<int, int>> attack, vector<pair<int, int>> strongDefense, vector<pair<int, int>> defense)
{
	return 0;
}

int ai_jxk::calculateDevelop(int row, int col, int color, int depth, int min, int max,
	vector<pair<int, int>> develop, vector<pair<int, int>> undevelop)
{
	//Ҫ���жϵ�λ�ñ���Ϊ��
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

	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//�ĸ�����ˮƽ����ֱ����б����б
	int k = 0;
	int x, y;
	for (int l = 0; l < 4; l++) {
		//������
		k = 0;//��0�����Լ�
		while (k <= 4) {
			x = row + dir[l][0] * k, y = col + dir[l][1] * k;
			if (board[x][y] < 0) break;//�����߽��ֹͣɨ��
			if (isDevelopPoint(x, y, color) >= 4) develop.push_back({ x,y });
			if (isDevelopPoint(x, y, nextcolor >= 4)) develop.push_back({ x,y });
			k += 1;
		}
		//������
		k = 1;
		while (k <= 4) {
			x = row - dir[l][0] * k, y = col - dir[l][1] * k;
			if (board[x][y] < 0) break;//�����߽��ֹͣɨ��
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
			if (maxScore >= max) break; // ��֦
		}
		else {
			if (output < minScore) minScore = output;
			if (minScore <= min) break; // ��֦
		}
	}
	board[row][col] = 0;
	return (color == 1) ? maxScore : minScore;
}


int ai_jxk::calculateScore(int row, int col, int color, string(&strs)[4])
{
	//�������������һ�����ǲ��ǽ���
//ע������ֻ�к����н��֣�����Ĭ���Ǻ��壬color��������

	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//�ĸ�����ˮƽ����ֱ����б����б
	//���Ӳ�ɨ��
	int blackScore = 0, whiteScore = 0;
	scanForbid(row, col, strs);
	//ͬʱ�����Щ�����Ѿ��й�4�ˣ���4�ķ����ټ��3
	int isfour[4] = { 0 };
	for (int i = 0; i < 4; i++) {
		bool checkline = true;
		for (string str : blackfournr) {
			if (strs[i].find(str) != string::npos) {
				checkline = false;//�������ֻ������һ������
				isfour[i] = 1;
				blackScore += 500;
				break;
			}
		}
		for (string str : blackfoura) {//�л��ģ������ֱ��ȡʤ
			if (checkline && strs[i].find(str) != string::npos) {
				isfour[i] = 1;
				blackScore += 10000;
				break;
			}
		}
		for (string str : blackfourd) {//�г���
			if (checkline && strs[i].find(str) != string::npos) {
				isfour[i] = 1;
				blackScore += 500;
				break;
			}
		}
	}

	//���ʼ�ж�������4�ķ����ټ��3
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			//������������Ƿ���ڣ�������
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
					isfour[i] = 1;//�Ѿ����������Ͳ�������������
					blackScore += 300;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : blackthreec) {//������
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
			for (string str : blackthreed) {//����
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
			for (string str : blackthrees) {//������
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
			for (string str : blacktwoa) {//���
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
			for (string str : blacktwoa) {//�߶�
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;
					blackScore += 50;
					break;
				}
			}
		}
	}
	//ͬʱ�����Щ�����Ѿ��й�4�ˣ���4�ķ����ټ��3
	isfour[0] = 0, isfour[1] = 0, isfour[2] = 0, isfour[3] = 0;
	for (int i = 0; i < 4; i++) {
		bool checkline = true;
		for (string str : whitefoura) {//�л��ģ������ֱ��ȡʤ
			if (checkline && strs[i].find(str) != string::npos) {
				isfour[i] = 1;
				whiteScore += 10000;
				break;
			}
		}
		for (string str : whitefourd) {//�г���
			if (checkline && strs[i].find(str) != string::npos) {
				isfour[i] = 1;
				whiteScore += 500;
				break;
			}
		}
	}

	//���ʼ�ж�������4�ķ����ټ��3
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : whitethreej) {
				if (strs[i].find(str) != string::npos) {
					isfour[i] = 1;//�Ѿ����������Ͳ�������������
					whiteScore += 300;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			for (string str : whitethreec) {//������
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
			for (string str : whitethreed) {//����
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
			for (string str : whitethrees) {//������
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
			for (string str : whitetwoa) {//���
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
			for (string str : whitetwod) {//�߶�
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
	//�������������������ϵ�һ������㣬ɨ���Ը���Ϊ���ģ����벻�����ĸ����ӵ��ĸ����򣬷�����������Ϊ9���ַ���
	//ע���������ɨ��ʱ����ı����̵�״̬������Ҫ���л���
	//ע���������Ϊ�����Ч�ʣ���ͨ������ֵ���ݣ�����ֱ�Ӹ����Ѿ��������ַ�������Ҫ���ַ���Ϊ�գ�ֻҪ��str[4]����

	for (int i = 0; i < 4; ++i) strs[i] = "";//��ÿ���ַ������
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//�ĸ�����ˮƽ����ֱ����б����б
	int k = 0;
	int x, y;
	for (int l = 0; l < 4; l++) {
		//������
		k = 0;//��0�����Լ�
		while (k <= 4) {
			x = row + dir[l][0] * k, y = col + dir[l][1] * k;
			strs[l] = strs[l] + intToChar(board[x][y]);
			if (board[x][y] < 0) break;//�����߽��ֹͣɨ��
			k += 1;
		}
		while (k <= 4) {
			strs[l] = strs[l] + '-';//ǿ�в�����9��������֮������жϺͲ���
			k += 1;
		}
		//������
		k = 1;
		while (k <= 4) {
			x = row - dir[l][0] * k, y = col - dir[l][1] * k;
			strs[l] = intToChar(board[x][y]) + strs[l];
			if (board[x][y] < 0) break;//�����߽��ֹͣɨ��
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
	for (int i = 0; i < 4; ++i) strs[i] = "";//��ÿ���ַ������
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//�ĸ�����ˮƽ����ֱ����б����б
	int k = 0;
	int x, y;
	for (int l = 0; l < 4; l++) {
		//������
		k = 0;//��0�����Լ�
		while (k <= 4) {
			x = row + dir[l][0] * k, y = col + dir[l][1] * k;
			strs[l] = strs[l] + intToChar(forbidBoard[x][y]);
			if (forbidBoard[x][y] < 0) break;//�����߽��ֹͣɨ��
			k += 1;
		}
		while (k <= 4) {
			strs[l] = strs[l] + '-';//ǿ�в�����9��������֮������жϺͲ���
			k += 1;
		}
		//������
		k = 1;
		while (k <= 4) {
			x = row - dir[l][0] * k, y = col - dir[l][1] * k;
			strs[l] = intToChar(forbidBoard[x][y]) + strs[l];
			if (forbidBoard[x][y] < 0) break;//�����߽��ֹͣɨ��
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
	//�����������ڰ�����ɨ����ַ�����ʱ�򣬰�ÿһ��int���͵����ӱ��char���͵�
	//���̼�¼���ӵĹ������£�������1��ʾ��������2��ʾ���հ���0��ʾ���߿���-1��ʾ���������������

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
		return 'X';//����
	}
	return 0;
}

bool ai_jxk::isForbid(int row, int col, int color)
{
	//�������������һ�����ǲ��ǽ���
	//ע������ֻ�к����н��֣�����Ĭ���Ǻ��壬color��������

	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//�ĸ�����ˮƽ����ֱ����б����б
	//���Ӳ�ɨ��
	board[row][col] = 1;//����������ط�����壬������
	string strs[4];
	scan(row, col, strs);

	//��ÿһ���������ж��ǲ��ǳ��������ж��ǲ�������
	for (int i = 0; i < 4; i++) {
		for (string str : blacksix) {
			if (strs[i].find(str) != string::npos) {
				board[row][col] = 0;//����
				return true;
			}
		}
		for (string str : blackfive) {
			if (strs[i].find(str) != string::npos) {
				board[row][col] = 0;//����
				return false;
			}
		}
	}
	
	//���ʼ�ж����Ľ��֣�ͬʱ�����Щ�����Ѿ��й�4�ˣ���4�ķ����ټ��3
	int isfour[4] = { 0 };
	int fourcount = 0;
	for (int i = 0; i < 4; i++) {
		//�ȼ��һ��������û�п�������������
		bool checkline = true;
		for (string str : blackfournr) {
			if (strs[i].find(str) != string::npos) {
				checkline = false;//�������ֻ������һ������
				isfour[i] = 1;
				++fourcount;
				break;
			}
		}
		for (string str : blackfourr) {
			if (checkline&&strs[i].find(str) != string::npos) {
				board[row][col] = 0;//����
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
	if (fourcount >= 2) {//��⵽���������ģ��ǽ���
		board[row][col] = 0;//����
		return true;
	}

	//���ʼ�ж�����������4�ķ����ټ��3
	int threecount = 0;
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			//������������Ƿ񲻴��ڻ���
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
			//���һ��������û�����������������Ҫ�ݹ��ж����������м��
			for (string str : blackthreej) {
				//if (strs[i].find(str) != string::npos) {
				//	//�ҵ���Ҫ�ݹ��λ��
				//	int start = str.find("011010");
				//	if (str.find("011010") != string::npos) {
				//		//����ݹ������ĵ����Ծ���,�Ƶ����ݹ�����
				//		int idx = start + 3;
				//		int dis = idx - 4;
				//		int r = row + dir[i][0] * dis, c = col + dir[i][1] * dis;
				//		if (!isForbid(r, c, color)); {
				//			isfour[i] = 1;//�Ѿ����������Ͳ�������������
				//			++threecount;
				//			break;
				//		}
				//	}
				//}
				if (strs[i].find(str) != string::npos) {
					////�ҵ���Ҫ�ݹ��λ��
					//int start = str.find("010110");
					//if (str.find("010110") != string::npos) {
					//	//����ݹ������ĵ����Ծ���,�Ƶ����ݹ�����
					//	int idx = start + 2;
					//	int dis = idx - 4;
					//	int r = row + dir[i][0] * dis, c = col + dir[i][1] * dis;
					//	if (!isForbid(r, c, color)); {
					//		isfour[i] = 1;//�Ѿ����������Ͳ�������������
					//		++threecount;
					//		break;
					//	}
					//}
					isfour[i] = 1;//�Ѿ����������Ͳ�������������
					++threecount;
					break;
				}
			}
		}
	}
	for (int i = 0; i < 4; i++) {
		if (!isfour[i]) {
			//�ټ��һ��������û�����������������Ҫ�ݹ��ж�����������ĵ�
			for (string str : blackthreec) {
				//if (strs[i].find(str) != string::npos) {
				//	//�ҵ���Ҫ�ݹ��λ��
				//	int start = strs[i].find("011100");
				//	if (strs[i].find("011100") != string::npos) {
				//		//����ݹ������ĵ����Ծ���,�Ƶ����ݹ�����
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
					////�ҵ���Ҫ�ݹ��λ��
					//int start = strs[i].find("001110");
					//if (strs[i].find("001110") != string::npos) {
					//	//����ݹ������ĵ����Ծ���,�Ƶ����ݹ�����
					//	int idx = start + 2;
					//	int dis = idx - 4;
					//	int r = row + dir[i][0] * dis, c = col + dir[i][1] * dis;
					//	if (!isForbid(r, c, color)); {
					//		++threecount;
					//		break;
					//	}
					//}
					//��ʱ�����ݹ飬���ڵݹ��ΪӰ��Ч��
					threecount++;
					break;
				}
			}
		}
	}
	if (threecount >= 2) {//��⵽�������������ǽ���
		board[row][col] = 0;//����
		return true;
	}
	board[row][col] = 0;//����
	return false;
}

//bool ai_jxk::checkWinPoint(int row, int col, int color, int(&board)[17][17])
//{
//	//�����������������һ�����ǲ���Ӯ�㣬��Ҫ������
//	//������Ҫ�����֣�������Ҫ������������
//
//
//	return false;
//}

int ai_jxk::isAttackPoint(int row, int col, int color, string(&strs)[4])
{
	//�����������������һ�����ǲ��ǹ��㣬��ʱ�������֣�ֻҪ�����ǰ�����γ��˻��������ġ�����Ϳ���
	//color������ɫ���������£�1�����������[row][col],2�������������
	//����intֵ�������0�Ͳ��ǹ��㣬1�ǹ��㣬2��ǿ������(��Ϊ����û���жϽ��֣�����˵�Ժ������˫��������ǿ������
	//��Ϊ������һ��������

	//�ȿ��ǲ����γ��˻��ģ�����ǻ��ģ�Ҳ��Ϊ��ǿ����
	if (isFourA(row, col, color, strs)) return 2;
	
	//����ǰ��壬ץ����Ҳ������Ϊ��һ��ǿ����
	if (color == 2) {
		string forbidstr[4];
		int a = forbidBoard[row][col];
		forbidBoard[row][col] = 2;
		scanForbid(row, col, forbidstr);
		forbidBoard[row][col] = a;
		if (isCatch(row, col, color, forbidstr)) return 2;
	}
	//���ܲ����γ�����������
	int three = countThreeandFour(row, col, color, strs);
	bool five = isFive(row, col, color, strs);
	if (three + five >= 2) return 2;
	else if (three + five == 1) return 1;
	else return 0;
}

int ai_jxk::isDevelopPoint(int row, int col, int color)
{
	//�����������������һ�����ǲ��ǹ��㣬��ʱ�������֣�ֻҪ�����ǰ�����γ�����������
	//color������ɫ���������£�1�����������[row][col],2�������������
	//����ֵ�����������һ�����γ��˶��ٹ���
	//����Ǻ��壬������ӽ����жϣ���������˽��ּ����
	int count = 0,before = 0;
	int dir[4][2] = { {0,1},{1,0},{1,1},{1,-1} };//�ĸ�����ˮƽ����ֱ����б����б
	int k = 0;
	int x = 0, y = 0;
	//��������,ɨ�����з���
	for (int l = 0; l < 4; l++) {
		//������
		k = 1;//�������Լ�
		while (k <= 4) {
			x = row + dir[l][0] * k, y = col + dir[l][1] * k;
			if (board[x][y] < 0) break;//�����߽��ֹͣɨ��
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
		//������
		k = 1;
		while (k <= 4) {
			x = row - dir[l][0] * k, y = col - dir[l][1] * k;
			if (board[x][y] < 0) break;//�����߽��ֹͣɨ��
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
	if (color == 2) {//���ڰ��壬������ں�������ϣ�Ҫ�ʵ�����
		if (isForbid(row, col, color)) count -= 3;
	}
	return count;
}

bool ai_jxk::isThreeA(int row, int col, int color, string (&strs)[4])
{
	//�����������������һ������û���γɻ������������)
	//color������ɫ���������£�1�����������[row][col],2�������������

	if (color == 1) {
		//����Ǻ���
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
		//����ǰ���
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
	//�����������������һ������û���γɻ������������)
	//color������ɫ���������£�1�����������[row][col],2�������������
	int count = 0;
	if (color == 1) {
		//����Ǻ���
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
		//����ǰ���
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
	//�����������������һ������û�г��ģ��������)
	//color������ɫ���������£�1�����������[row][col],2�������������

	if (color == 1) {
		//����Ǻ���
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
		//����ǰ���
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
	//�����������������һ�����м������ģ��������)
	//color������ɫ���������£�1�����������[row][col],2�������������

	int count = 0;
	if (color == 1) {
		//����Ǻ���
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
		//����ǰ���
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
	//�����������������һ������û�г��ģ��������)
//color������ɫ���������£�1�����������[row][col],2�������������

	if (color == 1) {
		//����Ǻ���
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
		//����ǰ���
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
	//�����������������һ���ܷ���ģ��������)
	//color������ɫ���������£�1�����������[row][col],2�������������

	if (color == 1) {
		//����Ǻ���
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
		//����ǰ���
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
	//�����������������һ������û�г��壨�������)
	//color������ɫ���������£�1�����������[row][col],2�������������

	if (color == 1) {
		//����Ǻ���
		for (int i = 0; i < 4; i++) {
			for (string str : blackfive) {
				if (strs[i].find(str) != string::npos) return true;
			}
		}
		return false;
	}
	else {
		//����ǰ���
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
