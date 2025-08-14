#include"ai.h"
#include <iostream>
#include<cmath>
#include<stdlib.h>
#include<string>
using namespace std;



//С���ߣ��ж�ĳ��(i,j)�Ƿ������̷�Χ��
bool ai::legalindv2(int ind1, int ind2, int n) {
    return ind1 >= 0 && ind1 < n && ind2 >= 0 && ind2 < n;
}

//aiִ����
int ai::lyxaiv2(int qz[20][20], int n, bool IsAIWhite, int count) {
    //cout << "from qz:" << endl;
    /*for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            cout << qz[i][j] << " ";
        }
        cout << endl;
    }*/
    if (IsAIWhite == false && count == 0) return (n / 2) * n + n / 2;
    int i, j, k, l, pieces_count, curi, curj;
    long long int score[20][20];
    int py[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };
    //��ʼ���÷�
    int qz_copy1[20][20]; int qz_copy2[20][20];
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            score[i][j] = 0;
        }
    }
    //AIִ����
    if (count > 8) {
        if (IsAIWhite) {
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    if (qz[i][j] == 0) {
                        bool rll = check_long_lian(qz, n, i, j);
                        int r34 = check34(qz, n, i, j);
                        //if (i == 10 && j == 7) cout << rll << "yeah" << r34 << endl;
                        qz_copy1[i][j] = qz[i][j];
                        if (rll || (r34 > 1)) {
                            qz_copy1[i][j] = 2;//�ڷ���ʱ����Ϊ���ֵ�Ҳ�����ѱ�����ռ��ĵ�
                            printf("banned:%d %d,rll:%d,r34:%d\n", i, j, rll, r34);
                        }
                        else if (r34 == 1) {
                            score[i][j] += 10e13 + 5 * 10e12;
                        }
                    }
                    else {
                        qz_copy1[i][j] = qz[i][j];
                    }
                }
            }
        }
        //AIִ����
        else {
            //for (i = 0; i < n; i++) {
            //    for (j = 0; j < n; j++) {
            //        if (qz[i][j] == 0) {
            //            if (check_banned_hand(qz, n, i, j)) qz_copy1[i][j] = 2;//�ڷ���ʱ����Ϊ���ֵ�Ҳ�����ѱ�����ռ��ĵ�
            //            else {
            //                qz_copy1[i][j] = qz[i][j];
            //            }
            //        }
            //        else {
            //            qz_copy1[i][j] = qz[i][j];
            //        }
            //    }
            //}
            //��ת��ɫ�������Ͳ��ð�AI�����ٳ�һ���ˣ���
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    if (qz[i][j] == 1) qz_copy2[i][j] = 2;
                    else if (qz[i][j] == 2) qz_copy2[i][j] = 1;
                    else qz_copy2[i][j] = 0;
                    if (qz[i][j] == 1) qz_copy1[i][j] = 2;
                    else if (qz[i][j] == 2) qz_copy1[i][j] = 1;
                    else qz_copy1[i][j] = 0;
                }
            }
        }
    }
    else {
        if (IsAIWhite) {
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    qz_copy1[i][j] = qz[i][j];
                }
            }
        }
        //AIִ����
        else {
            //for (i = 0; i < n; i++) {
            //    for (j = 0; j < n; j++) {
            //        if (qz[i][j] == 0) {
            //            if (check_banned_hand(qz, n, i, j)) qz_copy1[i][j] = 2;//�ڷ���ʱ����Ϊ���ֵ�Ҳ�����ѱ�����ռ��ĵ�
            //            else {
            //                qz_copy1[i][j] = qz[i][j];
            //            }
            //        }
            //        else {
            //            qz_copy1[i][j] = qz[i][j];
            //        }
            //    }
            //}
            //��ת��ɫ�������Ͳ��ð�AI�����ٳ�һ���ˣ���
            for (i = 0; i < n; i++) {
                for (j = 0; j < n; j++) {
                    if (qz[i][j] == 1) qz_copy2[i][j] = 2;
                    else if (qz[i][j] == 2) qz_copy2[i][j] = 1;
                    else qz_copy2[i][j] = 0;
                    if (qz[i][j] == 1) qz_copy1[i][j] = 2;
                    else if (qz[i][j] == 2) qz_copy1[i][j] = 1;
                    else qz_copy1[i][j] = 0;
                }
            }
        }
    }

    //�������ۿ��ܵ����
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < 8; k++) {
                string situation = ""; string situation2 = "";
                for (l = 0; l < 7; l++) {
                    if (legalindv2(i + l * py[k][0], j + l * py[k][1], n)) {
                        if (IsAIWhite) situation = situation + to_string(qz[i + l * py[k][0]][j + l * py[k][1]]);
                        else situation = situation + to_string(qz_copy2[i + l * py[k][0]][j + l * py[k][1]]);
                        situation2 = situation2 + to_string(qz_copy1[i + l * py[k][0]][j + l * py[k][1]]);
                    }
                    else {
                        break;
                    }
                }
                if (situation.length() == 7) {
                    //�����ĸ�
                    if (situation.substr(0, 5) == "02222") {
                        score[i][j] += 10e17;
                    }
                    else if (situation.substr(0, 5) == "20222") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e17;
                    }
                    else if (situation.substr(0, 5) == "22022") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e17;
                    }
                    //�����ĸ�
                    else if (situation2.substr(0, 6) == "011110") {
                        score[i][j] += 10e16;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e16;
                    }
                    else if (situation2.substr(0, 6) == "011112") {
                        score[i][j] += 10e16;
                    }
                    else if (situation2.substr(0, 5) == "10111") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e16;
                    }
                    else if (situation2.substr(0, 5) == "11011") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e16;
                    }
                    //�һ���
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
                    //���ֻ���
                    else if (situation2 == "0011100") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation2 == "2011100") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14 + 5 * 10e13;
                    }
                    else if (situation2 == "2011102") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation2.substr(0, 6) == "010110") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e13;
                        score[i + 0 * py[k][0]][j + 0 * py[k][1]] += 10e13;
                    }
                    //������
                    else if (situation.substr(0, 6) == "122200") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e13;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e13;
                    }
                    else if (situation.substr(0, 6) == "122020") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e13;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e13;
                    }
                    else if (situation.substr(0, 6) == "120220") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e13;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e13;
                    }
                    else if (situation.substr(0, 5) == "22002") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e13;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e13;
                    }
                    else if (situation.substr(0, 5) == "20202") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e13;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e13;
                    }
                    //��������
                    else if (situation2.substr(0, 6) == "211100") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation2.substr(0, 6) == "211010") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation2.substr(0, 6) == "210110") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation2.substr(0, 5) == "11001") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e10;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    else if (situation2.substr(0, 5) == "10101") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e10;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    //�һ��
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
                    //���ֻ��
                    else if (situation2.substr(0, 5) == "01100") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    else if (situation2.substr(0, 6) == "201010") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e9;
                    }
                    else if (situation2.substr(0, 6) == "001010") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e8;
                    }
                    //������
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
                    //��������
                    else if (situation2.substr(0, 5) == "21100") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e7;
                    }
                    else if (situation2.substr(0, 5) == "21010") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e7;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e7;
                    }
                    else if (situation2.substr(0, 5) == "21001") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e7;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                    }
                    //�һ�һ
                    else if (situation.substr(0, 5) == "00200") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                    }
                    else if (situation.substr(0, 5) == "02000") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e8;
                    }
                    else if (situation.substr(0, 6) == "020000") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e8;
                    }
                    //���ֻ�һ
                    else if (situation2.substr(0, 5) == "00100") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e7;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                    }
                }
                else if (situation.length() == 6) {
                    //cout << situation<<endl;
                    //�����ĸ�
                    if (situation.substr(0, 5) == "02222") {
                        score[i][j] += 10e17;
                    }
                    else if (situation.substr(0, 5) == "20222") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e17;
                    }
                    else if (situation.substr(0, 5) == "22022") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e17;
                    }
                    //�����ĸ�
                    else if (situation2 == "011110") {
                        score[i][j] += 10e16;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e16;
                    }
                    else if (situation2 == "011112") {
                        score[i][j] += 10e16;
                    }
                    else if (situation2.substr(0, 5) == "10111") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e16;
                    }
                    else if (situation2.substr(0, 5) == "11011") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e16;
                    }
                    //�һ���
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
                    //���ֻ���
                    else if (situation2 == "201110") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14 + 5 * (10e13);
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation2 == "001110") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    else if (situation2 == "010110") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e13;
                        score[i + 0 * py[k][0]][j + 0 * py[k][1]] += 10e13;
                    }
                    //������
                    else if (situation == "122200") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e13;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e13;
                    }
                    else if (situation == "122020") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e13;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e13;
                    }
                    else if (situation == "120220") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e13;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e13;
                    }
                    else if (situation.substr(0, 5) == "22002") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e13;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e13;
                    }
                    else if (situation.substr(0, 5) == "20202") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e13;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e13;
                    }
                    //��������
                    else if (situation2 == "211100") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation2 == "211010") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation2 == "210110") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    else if (situation2.substr(0, 5) == "11001") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e10;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    else if (situation2.substr(0, 5) == "10101") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e10;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    //�һ��
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
                    //���ֻ��
                    else if (situation2.substr(0, 5) == "01100") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e10;
                    }
                    else if (situation2 == "201010") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e9;
                    }
                    else if (situation2 == "001010") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e8;
                    }
                    //������
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
                    //��������
                    else if (situation2.substr(0, 5) == "21100") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e7;
                    }
                    else if (situation2.substr(0, 5) == "21010") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e7;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e7;
                    }
                    else if (situation2.substr(0, 5) == "21001") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e7;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                    }
                    //�һ�һ
                    else if (situation.substr(0, 5) == "00200") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e9;
                    }
                    else if (situation.substr(0, 5) == "02000") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e8;
                    }
                    else if (situation == "020000") {
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e8;
                    }
                    //���ֻ�һ
                    else if (situation2.substr(0, 5) == "00100") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e7;
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e7;
                    }
                }
                else if (situation.length() == 5) {
                    //���ĸ�
                    if (situation == "02222") {
                        score[i][j] += 10e17;
                    }
                    else if (situation == "20222") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e17;
                    }
                    else if (situation == "22022") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e17;
                    }
                    //�����ĸ�
                    else if (situation2 == "01111") {
                        score[i + 0 * py[k][0]][j + 0 * py[k][1]] += 10e16;
                    }
                    else if (situation2 == "10111") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e16;
                    }
                    else if (situation2 == "11011") {
                        score[i + 2 * py[k][0]][j + 2 * py[k][1]] += 10e16;
                    }
                    //�һ���
                    else if (situation == "02220") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e14;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e14;
                    }
                    //�������
                    else if (situation2 == "01110") {
                        score[i + 1 * py[k][0]][j + 1 * py[k][1]] += 10e10;
                        score[i + 5 * py[k][0]][j + 5 * py[k][1]] += 10e10;
                    }
                    //�һ��
                    else if (situation == "02200") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e9;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e9;
                    }
                    //���ֻ��
                    else if (situation2 == "01100") {
                        score[i + 3 * py[k][0]][j + 3 * py[k][1]] += 10e8;
                        score[i + 4 * py[k][0]][j + 4 * py[k][1]] += 10e8;
                    }
                }
            }
        }
    }

    long long int max_score = 0; int max_i, max_j;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            //cout << score[i][j] << " ";
            if (score[i][j] > max_score) {
                if (IsAIWhite) max_score = score[i][j];
                else if (!check_banned_hand(qz, n, i, j)) {
                    max_score = score[i][j];
                }
                else {
                    score[i][j] = -1;
                }
            }
        }
        //cout << endl;
    }
    //cout << max_score << endl;
    long long int max_again = -1000000;
    long long int select_again[20][20];
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            select_again[i][j] = 0;
            //cout << score[i][j] << " ";
            if (score[i][j] == max_score && qz[i][j] == 0) {
                if (!IsAIWhite) {
                    if (check_banned_hand(qz, n, i, j)) continue;
                }
                int neighbors_count = 0;
                for (k = 0; k < 8; k++) {
                    if (legalindv2(i + py[k][0], j + py[k][1], n)) {
                        if (qz[i + py[k][0]][j + py[k][1]] != 0) {
                            select_again[i][j] += 1000;
                        }
                    }
                }
                //if(!IsAIWhite) select_again[i][j] += 2000 * (0.5 - (pow((float)i / (float)n - 0.5, 2) + pow((float)j / (float)n - 0.5, 2)));
                select_again[i][j] += 10e8;
                if (i < 3) select_again[i][j] -= (3 - i) * 2000;
                else if (i > n - 4) select_again[i][j] -= (i - n + 4) * 2000;
                if (j < 3) select_again[i][j] -= (3 - j) * 2000;
                else if (j > n - 4) select_again[i][j] -= (j - n + 4) * 2000;
            }
            else {
                select_again[i][j] = -1;
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
    return max_i * n + max_j;
}

int ai::have_anyone_won(int qz[20][20], int n) {//����Ƿ����˻�ʤ
    int py[8][2] = { {0,1},{1,1},{1,0},{1,-1},{0,-1},{-1,-1},{-1,0},{-1,1} };
    int i, j, k, l;
    int boardisfull = true;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            if (qz[i][j] == 0) boardisfull = false;
            for (k = 0; k < 4; k++) {
                string situation = "";
                for (l = 0; l < 7; l++) {
                    if (legalindv2(i + l * py[k][0], j + l * py[k][1], n)) {
                        situation = situation + to_string(qz[i + l * py[k][0]][j + l * py[k][1]]);
                    }
                    else {
                        break;
                    }
                }
                if (situation.substr(0, 5) == "22222") return 2;//�����ʤ
                //�������������Ϊ�����ʤ
                if (situation == "0111110" || situation == "2111110" || situation == "0111112" || situation == "2111112") return 1;
                if (situation == "111110" || situation == "111112" || situation == "011111" || situation == "211111")return 1;
                if (situation == "11111") return 1;
            }
        }
    }
    if (boardisfull) return 3;//�����ѱ�����������
    return 0;
}


bool ai::check_long_lian(int qz_copy[20][20], int n, int pi, int pj) {//��ⳤ������
    int i, j, k, l;
    bool flag1 = false; bool flag2 = false;
    int qz[20][20];
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            qz[i][j] = qz_copy[i][j];
        }
    }
    qz[pi][pj] = 1;
    if (have_anyone_won(qz, n) == 1) return false;
    int py[4][2] = { {0,1},{1,1},{1,0},{1,-1} };
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < 4; k++) {
                string situation = "";
                for (l = 0; l < 6; l++) {
                    if (legalindv2(i + l * py[k][0], j + l * py[k][1], n)) {
                        situation = situation + to_string(qz[i + l * py[k][0]][j + l * py[k][1]]);
                    }
                    else {
                        break;
                    }
                }
                //cout << situation << endl;
                if (situation == "111111") return true;
            }
        }
    }
    return false;
}



int ai::check_four(int qz_copy[20][20], int n, int pi, int pj) {   //������Ľ���
    int i, j, k, l;
    int qz[20][20];
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            qz[i][j] = qz_copy[i][j];
        }
    }
    qz[pi][pj] = 1;
    int four = 0; int three = 0;
    int py[4][2] = { {0,1},{1,1},{1,0},{1,-1} };
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            for (k = 0; k < 4; k++) {
                bool flag = true;
                for (l = 0; l < 5; l++) {
                    if (i + l * py[k][0] == pi && j + l * py[k][1] == pj) flag = false;
                }
                if (flag) continue;
                string situation = ""; string situation5 = "";
                for (l = 0; l < 5; l++) {
                    if (legalindv2(i + l * py[k][0], j + l * py[k][1], n)) {
                        situation5 = situation5 + to_string(qz[i + l * py[k][0]][j + l * py[k][1]]);
                    }
                    else {
                        break;
                    }
                }
                situation = situation5.substr(0, 4);
                if (situation5 == "11101") {
                    //cout << "jxk1" << endl;
                    if (!check_long_lian(qz, n, i + 3 * py[k][0], j + 3 * py[k][1])) {
                        four++;
                        if (four >= 2) return 2;
                    }
                }
                else if (situation5 == "11011") {
                    //cout << "jxk2" << endl;
                    if (!check_long_lian(qz, n, i + 2 * py[k][0], j + 2 * py[k][1])) {
                        four++;
                        if (four >= 2) return 2;
                    }
                }
                else if (situation5 == "10111") {
                    //cout << "jxk3" << endl;
                    if (!check_long_lian(qz, n, i + 3 * py[k][0], j + 3 * py[k][1])) {
                        four++;
                        if (four >= 2) return 2;
                    }
                }
                else if (situation == "1111") {
                    //cout << i << " " << j << endl;
                    if (legalindv2(i - py[k][0], j - py[k][1], n)) {
                        if (qz[i - py[k][0]][j - py[k][1]] == 0) {
                            if (!check_long_lian(qz, n, i - py[k][0], j - py[k][1])) {
                                printf("1111:%d %d\n", i, j);
                                four++;
                                if (four >= 2) return 2;
                                continue;
                            }
                        }
                    }
                    if (legalindv2(i + 4 * py[k][0], j + 4 * py[k][1], n)) {
                        if (qz[i + 4 * py[k][0]][j + 4 * py[k][1]] == 0) {
                            if (!check_long_lian(qz, n, i + 4 * py[k][0], j + 4 * py[k][1])) {
                                printf("1111:%d %d\n", i, j);
                                four++;
                                if (four >= 2) return 2;
                            }
                        }
                    }

                }
            }
        }
    }
    return four;
}

int ai::check_live_three(int qz_copy[20][20], int n, int pi, int pj) {//�����������
    int i, j, k, l;
    int qz[20][20];
    int py[4][2] = { {0,1},{1,1},{1,0},{1,-1} };
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            qz[i][j] = qz_copy[i][j];
        }
    }
    qz[pi][pj] = 1;
    int four = 0; int three = 0;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            //bool avk[4] = { true,true,true,true };
            for (k = 0; k < 4; k++) {
                //if (!avk[k]) continue;
                bool flag = true;
                for (l = 0; l < 4; l++) {
                    if (i + l * py[k][0] == pi && j + l * py[k][1] == pj) flag = false;
                }
                if (flag) continue;

                string situation5 = "";
                for (l = 0; l < 5; l++) {
                    if (legalindv2(i + l * py[k][0], j + l * py[k][1], n)) {
                        situation5 = situation5 + to_string(qz[i + l * py[k][0]][j + l * py[k][1]]);
                    }
                    else {
                        break;
                    }
                }
                string situation = situation5.substr(0, 4);
                if (situation == "1101") {
                    if (i == 10) printf("1101appear:%d %d\n", i, j);
                    //cout << "1101" << endl;
                    if (legalindv2(i - py[k][0], j - py[k][1], n) && legalindv2(i + 4 * py[k][0], j + 4 * py[k][1], n)) {
                        if (qz[i - py[k][0]][j - py[k][1]] == 0 && qz[i + 4 * py[k][0]][j + 4 * py[k][1]] == 0) {
                            if (!check_banned_hand(qz, n, i + 2 * py[k][0], j + 2 * py[k][1])) {
                                printf("1101:%d %d\n", i, j);
                                three++;
                                if (three >= 2) return 2;
                                //avk[k] = false;
                                continue;
                            }
                            else {
                                printf("1101butblocked:%d %d %d\n", i, j, check_banned_hand(qz, n, i + 2 * py[k][0], j + 2 * py[k][1]));
                            }
                        }
                    }
                }
                else if (situation == "1011") {
                    if (legalindv2(i - py[k][0], j - py[k][1], n) && legalindv2(i + 4 * py[k][0], j + 4 * py[k][1], n)) {
                        if (qz[i - py[k][0]][j - py[k][1]] == 0 && qz[i + 4 * py[k][0]][j + 4 * py[k][1]] == 0) {
                            if (!check_banned_hand(qz, n, i + 1 * py[k][0], j + 1 * py[k][1])) {
                                printf("1011:%d %d\n", i, j);
                                three++;
                                if (three >= 2) return 2;
                                //avk[k] = false;
                                continue;
                            }
                        }
                    }
                }
                else if (situation5 == "01110") {
                    //if (pi == 10 && pj == 7) printf("1110,");
                    if (legalindv2(i + 5 * py[k][0], j + 5 * py[k][1], n)) {
                        if (qz[i + 5 * py[k][0]][j + 5 * py[k][1]] == 0) {
                            if (!(check_banned_hand(qz, n, i + 0 * py[k][0], j + 0 * py[k][1]) ||
                                check_banned_hand(qz, n, i + 5 * py[k][0], j + 5 * py[k][1]) ||
                                check_banned_hand(qz, n, i + 4 * py[k][0], j + 4 * py[k][1]))) {
                                printf("01110:%d %d\n", i, j);
                                three++;
                                if (three >= 2) return 2;
                                //avk[k] = false;
                                continue;
                            }
                        }
                    }
                    if (legalindv2(i - 1 * py[k][0], j - 1 * py[k][1], n)) {
                        if (qz[i - 1 * py[k][0]][j - 1 * py[k][1]] == 0) {
                            if (!(check_banned_hand(qz, n, i + 0 * py[k][0], j + 0 * py[k][1]) ||
                                check_banned_hand(qz, n, i - 1 * py[k][0], j - 1 * py[k][1]) ||
                                check_banned_hand(qz, n, i + 4 * py[k][0], j + 4 * py[k][1]))) {
                                printf("01110:%d %d\n", i, j);
                                three++;
                                if (three >= 2) return 2;
                                //avk[k] = false;
                                continue;
                            }
                        }
                    }
                }
                //else if (situation == "0111") {
                //    //if (pi == 10 && pj == 7) cout << "wwww" << endl;
                //    if (legalindv2(i - py[k][0], j - py[k][1], n) && legalindv2(i + 4 * py[k][0], j + 4 * py[k][1], n)) {
                //        if (qz[i - py[k][0]][j - py[k][1]] == 0 && qz[i + 4 * py[k][0]][j + 4 * py[k][1]] == 0) {
                //            if (!check_banned_hand(qz, n, i + 0 * py[k][0], j + 0 * py[k][1])) {
                //                printf("0111:%d %d\n", i, j);
                //                three++;
                //                if (three >= 2) return 2;
                //                //avk[k] = false;
                //                continue;
                //            }
                //        }
                //    }
                //}
            }
        }
    }
    return three;
}


int ai::check_banned_hand(int qz_copy[20][20], int n, int pi, int pj) {
    bool r1 = check_long_lian(qz_copy, n, pi, pj);
    if (r1) return 1;//����
    int r2 = check_four(qz_copy, n, pi, pj);
    if (r2 == 2) return 2;//�������Ľ���
    int r3 = check_live_three(qz_copy, n, pi, pj);
    if (r3 == 2) return 3;//������������

    return 0;//δ��������
}

int ai::check34(int qz_copy[20][20], int n, int pi, int pj) {
    int r4 = check_four(qz_copy, n, pi, pj);
    if (r4 == 2) return 2;//˫��
    int r3 = check_live_three(qz_copy, n, pi, pj);
    if (r3 == 2) return 3;//˫����
    if (r4 == 1 && r3 == 1) return 1;//��+����
    return 0;
}