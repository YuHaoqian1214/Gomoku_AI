#include "ai.h"
#include<random>
#include<vector>
#include<iostream>
#include<cmath>
#include<stdlib.h>
#include<string>
using namespace std;

void ai::transBoardToqz(vector<vector<crossPointState>> boardInfo)
{
	for (int i = 1; i <= boardSize; i++) {
		for (int j = 1; j <= boardSize; j++) {
			switch (boardInfo[i][j]) {
			case isEmpty:
				qz[i - 1][j - 1] = 0;
				break;
			case isWHITE:
				qz[i - 1][j - 1] = 2;
				break;
			case isBLACK:
				qz[i - 1][j - 1] = 1;
                break;
            case isForbid:
                qz[i - 1][j - 1] = 0;
				break;
            default:
                cout << "fail trans to qz" << endl;
                break;
			}
		}
	}
	return;
}

chessPos ai::lyxai(int qz[20][20], int p1, int p2, int n)
{
    //cout << "lyxai start" << endl;
    int i, j, k, l, pieces_count, curi, curj;
    long long int score[20][20];
    int py[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };
    //initailize score
    for (i = 0; i < 20; i++) {
        for (j = 0; j < 20; j++) {
            score[i][j] = 0;
        }
    }
    //detect consecutive pieces amount
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < 8; k++) {
                string situation = "";
                for (l = 0; l < 7; l++) {
                    if (legalind(i + l * py[k][0], j + l * py[k][1], n)) {
                        situation = situation + to_string(qz[i + l * py[k][0]][j + l * py[k][1]]);
                    }
                    else {
                        break;
                    }
                }
                if (situation.length() == 7) {
                    //"四缺一+*"
                    if (situation.substr(0, 5) == "02222") {
                        score[i][j] += 10e17;
                    }
                    else if (situation.substr(0, 5) == "20222") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e17;
                    }
                    else if (situation.substr(0, 5) == "22022") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e17;
                    }
                    //对面四个
                    else if (situation.substr(0, 6) == "011110") {
                        score[i][j] += 10e16;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e16;
                    }
                    else if (situation.substr(0, 6) == "011112") {
                        score[i][j] += 10e16;
                    }
                    else if (situation.substr(0, 5) == "10111") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e16;
                    }
                    else if (situation.substr(0, 5) == "11011") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e16;
                    }
                    //我活三
                    else if (situation == "0022200") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e15;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e15;
                    }
                    else if (situation == "1022200") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e15;
                    }
                    else if (situation == "1022201") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation.substr(0, 6) == "020220") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e15;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                        score[i + 0 * py[k][0]][j + 0 * py[k][1]] += 10e14;
                    }
                    //对手活三
                    else if (situation == "0011100") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation == "2011100") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation == "2011102") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation.substr(0, 6) == "010110") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e13;
                        score[i + 0 * py[k][0]][j + 0 * py[k][1]] += 10e13;
                    }
                    //我死三
                    else if (situation.substr(0, 6) == "122200") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation.substr(0, 6) == "122020") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation.substr(0, 6) == "120220") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation.substr(0, 5) == "22002") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e14;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e14;
                    }
                    else if (situation.substr(0, 5) == "20202") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e14;
                    }
                    //对手死三
                    else if (situation.substr(0, 6) == "211100") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation.substr(0, 6) == "211010") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation.substr(0, 6) == "210110") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation.substr(0, 5) == "11001") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e10;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    else if (situation.substr(0, 5) == "10101") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e10;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    //我活二
                    else if (situation.substr(0, 6) == "022000") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e12;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e11;
                    }
                    else if (situation.substr(0, 6) == "022001") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e12;
                    }
                    else if (situation.substr(0, 6) == "020200") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e12;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e11;
                    }
                    else if (situation.substr(0, 6) == "020201") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e12;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e9;
                    }
                    else if (situation.substr(0, 6) == "020020") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e11;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e11;
                    }
                    //对手活二
                    else if (situation.substr(0, 5) == "01100") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    else if (situation.substr(0, 6) == "201010") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e9;
                    }
                    else if (situation.substr(0, 6) == "001010") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e8;
                    }
                    //我死二
                    else if (situation.substr(0, 5) == "12200") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e9;
                    }
                    else if (situation.substr(0, 5) == "12020") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e9;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e9;
                    }
                    else if (situation.substr(0, 5) == "12002") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e9;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                    }
                    //对手死二
                    else if (situation.substr(0, 5) == "21100") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e7;
                    }
                    else if (situation.substr(0, 5) == "21010") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e7;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e7;
                    }
                    else if (situation.substr(0, 5) == "21001") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e7;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                    }
                    //我活一
                    else if (situation.substr(0, 5) == "00200") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                    }
                    else if (situation.substr(0, 5) == "02000") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e8;
                    }
                    else if (situation.substr(0, 6) == "020000") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e8;
                    }
                    //对手活一
                    else if (situation.substr(0, 5) == "00100") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e7;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                    }
                }
                else if (situation.length() == 6) {
                    //cout << situation<<endl;
                    //"四缺一+*"
                    if (situation.substr(0, 5) == "02222") {
                        score[i][j] += 10e17;
                    }
                    else if (situation.substr(0, 5) == "20222") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e17;
                    }
                    else if (situation.substr(0, 5) == "22022") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e17;
                    }
                    //对面四个
                    else if (situation == "011110") {
                        score[i][j] += 10e16;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e16;
                    }
                    else if (situation == "011112") {
                        score[i][j] += 10e16;
                    }
                    else if (situation.substr(0, 5) == "10111") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e16;
                    }
                    else if (situation.substr(0, 5) == "11011") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e16;
                    }
                    //我活三
                    else if (situation == "102220") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation == "002220") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e15;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation == "020220") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e15;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                        score[i + 0 * py[k][0]][j + 0 * py[k][1]] += 10e14;
                    }
                    //对手活三
                    else if (situation == "201110") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation == "001110") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation == "010110") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e13;
                        score[i + 0 * py[k][0]][j + 0 * py[k][1]] += 10e13;
                    }
                    //我死三
                    else if (situation == "122200") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation == "122020") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation == "120220") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation.substr(0, 5) == "22002") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e14;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e14;
                    }
                    else if (situation.substr(0, 5) == "20202") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e14;
                    }
                    //对手死三
                    else if (situation == "211100") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation == "211010") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation == "210110") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation.substr(0, 5) == "11001") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e10;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    else if (situation.substr(0, 5) == "10101") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e10;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    //我活二
                    else if (situation == "022000") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e12;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e11;
                    }
                    else if (situation == "022001") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e12;
                    }
                    else if (situation == "020200") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e12;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e11;
                    }
                    else if (situation == "020201") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e12;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e9;
                    }
                    else if (situation == "020020") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e11;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e11;
                    }
                    //对手活二
                    else if (situation.substr(0, 5) == "01100") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    else if (situation == "201010") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e9;
                    }
                    else if (situation == "001010") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e8;
                    }
                    //我死二
                    else if (situation.substr(0, 5) == "12200") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e9;
                    }
                    else if (situation.substr(0, 5) == "12020") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e9;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e9;
                    }
                    else if (situation.substr(0, 5) == "12002") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e9;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                    }
                    //对手死二
                    else if (situation.substr(0, 5) == "21100") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e7;
                    }
                    else if (situation.substr(0, 5) == "21010") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e7;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e7;
                    }
                    else if (situation.substr(0, 5) == "21001") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e7;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                    }
                    //我活一
                    else if (situation.substr(0, 5) == "00200") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                    }
                    else if (situation.substr(0, 5) == "02000") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e8;
                    }
                    else if (situation == "020000") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e8;
                    }
                    //对手活一
                    else if (situation.substr(0, 5) == "00100") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e7;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                    }
                }
                else if (situation.length() == 5) {
                    //我四个
                    if (situation == "02222") {
                        score[i][j] += 10e17;
                    }
                    else if (situation == "20222") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e17;
                    }
                    else if (situation == "22022") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e17;
                    }
                    //对面四个
                    else if (situation == "01111") {
                        score[i + 0 * py[k][0]][j + 0 * py[k][1]] += 10e16;
                    }
                    else if (situation == "10111") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e16;
                    }
                    else if (situation == "11011") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e16;
                    }
                    //我活三
                    else if (situation == "02220") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    //对面活三
                    else if (situation == "01110") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    //我活二
                    else if (situation == "02200") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e9;
                    }
                    //对手活二
                    else if (situation == "01100") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e8;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e8;
                    }
                }
            }
        }
    }

    long long int max_score = 0; int max_i = 0, max_j = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            //cout << score[i][j] << " ";
            if (score[i][j] > max_score) {
                max_score = score[i][j];
            }
        }
        //cout << endl;
    }
    //cout << max_score;
    int max_again = 0;
    int select_again[20][20];
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            select_again[i][j] = 0;
            //cout << score[i][j] << " ";
            if (score[i][j] == max_score && qz[i][j] == 0) {
                int neighbors_count = 0;
                for (k = 0; k < 8; k++) {
                    if (legalind(i + py[k][0], j + py[k][1], n)) {
                        if (qz[i + py[k][0]][j + py[k][1]] == 2) {
                            select_again[i][j] += 1000;
                        }
                    }
                }
                select_again[i][j] += 2000 * (0.5 - (pow((float)i / (float)n - 0.5, 2) + pow((float)j / (float)n - 0.5, 2)));
            }
        }
        //cout << endl;
    }
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (select_again[i][j] > max_again) {
                max_i = i; max_j = j;
                max_again = select_again[i][j];
            }
        }
    }
    //cout << "lyxai end" << endl;
    return { max_j + 1,max_i + 1,currentColor };
}

bool ai::legalind(int ind1, int ind2, int n)
{
    return ind1 >= 0 && ind1 < n && ind2 >= 0 && ind2 < n;
}
