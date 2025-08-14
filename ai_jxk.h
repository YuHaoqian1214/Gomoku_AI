#pragma once
#include<vector>
#include<utility>
#include<algorithm>
#include<string>
#include"global.h"
using namespace std;
class ai_jxk//这个ai暂时只能支持15*15的棋盘
{
public:
	//这个函数用本棋盘的存储方式（crosspointstate）数据类型来导入一个局面，不用单独处理边框
	void inputFromGame(vector<vector<crossPointState>> boardInfo);

	//专门为botzone开发的输入端，应当能完成json的写入，具有良好的封装性
	void init();
	void placeAt(int x, int y);
	void inputFromBotzone();
	void outputToBotzone();
	void simpleInput();
	void simpleOutput(pair<int, int> pa);

	//ai的局面分析函数，对于每一个点，先评估他是否能成为黑方或白方的攻点，
	// 再评估他能否成为黑方或白方的潜力点，再评估这个点的材料分（三步可能同时进行）
	//最终运算出一个最佳落子点，运算逻辑如下：先计算能否直接进攻取胜，再计算是否存在一些必须防守白方的杀点，
	// 最后再评估能不能形成最多的材料，如果黑棋形成了禁手会有扣分
	//函数的返回值：一个pair组，第一项是行，第二项是列，左上角认为是1，1
	pair<int, int> evaluate();

	//递归深层计算函数，有多种输入方法，可以计算攻点，也可以计算潜力点和材料值
	//关于函数的返回值，如果是算杀，就只可能有两种返回值，0意味着没有杀法，-1意味黑棋胜利，-2意味白棋胜利，
	//对于算分，就只可能是正值
	//算杀的流程：优先进行对杀，尝试所有可能的攻点（后期由于时间限制，可能变为随机尝试以尝试尽可能广的分支，做一些均衡）
	//如果发现所有的攻点都不是杀点，那就计算白方下一步的杀点，如果有就堵住，有多个选择能堵住最多白棋攻点的位置
	//如果双方都没有杀点，那么返回0
	//如果返回了零，就进入计算潜力点的流程，潜力点就是指可以形成两个及以上攻点的点位，如果时间不足，可以直接选择形成攻点最多的潜力点落子，
	//如果时间充足，可以计算形成的攻点包括几个杀点。
	//如果不存在潜力点，那就只能简单的计算形成的材料，进行函数估值
	int calculateAttack(int row, int col, int color, int depth, vector<pair<int, int>> strongAttack, vector<pair<int, int>> attack,
		vector<pair<int, int>> strongDefense, vector<pair<int, int>> defense);
	int calculateDevelop(int row, int col, int color, int depth, int min,int max,
		vector<pair<int, int>> develop, vector<pair<int, int>> undevelop);
	int calculateScore(int row, int col, int color, string(&strs)[4]);

private://这里是所有的辅助函数，注意这些函数扫描后时不会改变棋盘的状态，但是递归时不需要回溯

	//辅助函数，对于棋盘上的一个坐标点，扫描以该子为中心，距离不超过四个棋子的四个方向，返回四条长度为9的字符串
	//注意这个函数为了提高效率，不通过返回值传递，而是直接更改已经创建的字符串，不要求字符串为空，只要是str[4]即可
	void scan(int row, int col, string(&strs)[4]);
	void scanForbid(int row, int col, string(&strs)[4]);

	//辅助函数，在把棋盘扫描成字符串的时候，把每一个int类型的棋子变成char类型的
	char intToChar(int piece);

	//辅助函数，检测一个点是不是禁手
	//注意由于只有黑棋有禁手，所以默认是黑棋，color不起作用
	bool isForbid(int row, int col, int color);

	////辅助函数，用来检测一个点是不是赢点，需要检测禁手
	////由于需要检测禁手，所以需要传递整个棋盘
	//bool checkWinPoint(int row, int col, int color, int(&board)[17][17]);

	//辅助函数，用来检测一个点是不是攻点，此时不检测禁手，只要在这个前提下形成了活三、成四就可以
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这
	//返回int值，如果是0就不是攻点，1是攻点，2是强力攻点
	int isAttackPoint(int row, int col, int color, string(&strs)[4]);

	//辅助函数，用来检测一个点是不是攻点，此时不检测禁手，只要在这个前提下形成了两个攻点
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这
	//返回值代表这个点这一步新形成了多少攻点
	//如果是黑棋，额外添加禁手判断，如果创造了禁手减五分
	int isDevelopPoint(int row, int col, int color);

	//辅助函数，用来检测一个点有没有形成活三（不算禁手)
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这
	bool isThreeA(int row, int col, int color, string(&strs)[4]);
	int countThreeA(int row, int col, int color, string(&strs)[4]);

	//辅助函数，用来检测一个点有没有形成四（不算禁手)
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这
	bool isFour(int row, int col, int color, string(&strs)[4]);
	int countFour(int row, int col, int color, string(&strs)[4]);
	int countThreeandFour(int row, int col, int color, string(&strs)[4]);
	bool isFourA(int row, int col, int color, string(&strs)[4]);

	//辅助函数，用来检测一个点有没有形成五（不算禁手)
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这
	bool isFive(int row, int col, int color, string(&strs)[4]);
	//成五直接获胜，不需要检测数量

	//辅助函数，用来检测一个点有没有形成抓禁手
	//color代表颜色，规则如下：1代表黑棋下载[row][col],2代表白棋下在这
	//color没有意义，默认是白色，因为只有白棋能抓禁手
	bool isCatch(int row, int col, int color, string(&strs)[4]);


public:
	int step = 0;
	int board[17][17] = { {0} };//一个大小为15*15的棋盘，包括边框
	int forbidBoard[17][17] = { {0} };//一个包含禁手信息的棋盘
	//棋盘记录棋子的规则如下：黑棋用1表示，白棋用2表示，空白用0表示，边框用-1表示，不存在其他情况
	bool isblack = true;//记录输入的一个局面该轮到黑棋还是白棋行棋，在每次输入的时候重新计算
	float atweight = 1, deweight = 1;//进攻和防守的权重
	int developLimit = 3;

private:
	//一些字符串组常量，记录黑棋和白棋的特定棋型（从眠二到长连）
	//命名规则如下，英文全拼代表数字，而a代表活，j代表跳，c代表连，d代表眠，
	// s代表特型，而r代表用于规则判断的禁手，nr表示用于规则判断的非禁手
	//命名都是小写字母

	//黑棋特殊棋型
	//五及以上
	string blacksix[1] = { "111111" };//连六（包括所有长连）
	string blackfive[1] = { "11111" };//连五
	//四
	string blackfour[5] = { "01111","10111","11011","11101","11110" };//成四
	string blackfoura[1] = { "011110" };//活四
	string blackfourd[5] = { "011112","10111","11011","11101","211110" };//眠四（冲四）
	string blackfourr[3] = { "111010111","11011011","1011101" };
	string blackfournr[6] = { "1111010111","111011011","11011101","1110101111","110110111","10111011" };
	//三
	string blackthreea[4] = { "011100","001110","011010","010110" };//活三
	string blackthreec[2] = { "011100","001110" };//连活三
	string blackthreej[2] = { "011010","010110" };//跳活三
	string blackthreed[7] = { "001112","211100","2011102","010112","211010","011012","210110" };//眠三
	string blackthrees[4] = { "010101","101010","011101","101110" };//特型三
	string blackthreenr[2] = { "101101","1101011" };
	//二
	string blacktwoa[6] = { "001100","010100","011000","000110","001010","010010" };//活二
	string blacktwod[10] = { "2110002","2101002","2100102","2100012","2011002","2010102",
		"2010012","2001102","2001012","2000112" };//眠二

	//白棋特殊棋型
	//白棋抓禁手
	string whitecatch[10] = { "29222","22922","22292","092220","022290","022920","029220","29922","22992","29292" };
	//五及以上
	string whitefive[1] = { "22222" };//连五（包括所有五以上）
	//四
	string whitefour[5] = { "02222","20222","22022","22202","22220" };//成四
	string whitefoura[1] = { "022220" };//活四
	string whitefourd[5] = { "022221","20222","22022","22202","122220" };//眠四（冲四）
	//三
	string whitethreea[4] = { "022200","002220","022020","020220" };//活三
	string whitethreec[2] = { "022200","002220" };//连活三
	string whitethreej[2] = { "022020","020220" };//跳活三
	string whitethreed[7] = { "002221","122200","1022201","020221","122020","022021","120220" };//眠三
	string whitethrees[4] = { "020202","202020","022202","202220" };//特型三
	//二
	string whitetwoa[6] = { "002200","020200","022000","000220","002020","020020" };//活二
	string whitetwod[10] = { "1220001","1202001","1200201","1200021","1022001","1020201",
		"1020021","1002201","1002021","1000221" };//眠二
};

