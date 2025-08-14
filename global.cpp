#include"global.h"
int boardSize = 15;
const float scale = 0.5;//����ϵͳ��ʾԭ����Ҫһ�����ų����������ڴ�С������Ϊ1����
const int windowWidth = round(1997 * scale), windowHeight = round(scale * 1548);//���ڴ�С
const int BoardWidth = round(1500 * scale), BoardHeight = round(scale * 1483);//������Ч��С
const int gapX[2] = { round(123 * scale),round(123 * scale) }, 
gapY[2] = { round(63 * scale),round(63 * scale) };//��¼��һ�������������̶����������ľ��룬��λ������
const int gridSize = 97, pieceSize = 86;
int style = 1;
const int gridSizeScaled = round(97 * scale), pieceSizeScaled = round(86 * scale);//һ�����̸�Ĵ�С��Ҳ��Լһ�����ӵĴ�С
const int iconSizeScaled = round(11 * scale);
const float gridSizeScaledF = round(97 * scale), pieceSizeScaledF = round(86 * scale);
const int startBtnL = round(1566 * scale), startBtnR = round(1914 * scale);
const int startBtnT = round(399 * scale), startBtnB = round(506 * scale);
const int saveBtnL = round(1568 * scale), saveBtnR = round(1914 * scale);
const int saveBtnT = round(596 * scale), saveBtnB = round(699 * scale);
const int readBtnL = round(1568 * scale), readBtnR = round(1914 * scale);
const int readBtnT = round(789 * scale), readBtnB = round(886 * scale);
const int exitBtnL = round(1566 * scale), exitBtnR = round(1914 * scale);
const int exitBtnT = round(981 * scale), exitBtnB = round(1083 * scale);
turn currentTurn=isPlayerTurn;
turn white=isAiTurn, black=isPlayerTurn;
pieceType currentColor = isBLACK;
chessPos clickPos = { 1,1,isBLACK };
const string blackLong[1] = { "000000" };//���峤��
const string blackFive[1] = { "00000" };//��������
const string blackFour[5] = { "00002","20000","00020","02000","00200" };//�������
const string blackSpecial[3] = { "000202000","00200200","202000202" };//�������Ľ���
const string blackFourA[1] = { "200002" };//�������
const string blackFourD[9] = { "100002","200001","300002","200003","00020","02000","00200" };//��������
const string blackThreeA[2] = { "220002","200022" };//����������
const string blackThreeJ[2] = { "202002","200202" };//����������
const string blackThreeS[1] = { "20200202" };//����ǻ���
const string blackThreeD[11] = { "202001","100202","202003","300202","220001",
"100022","220003","300022","02020","22002","20022" };//��������
const string blackTwoA[6] = { "200222","222002","220202","202022","202202" };//������
const string blackTwoD[12] = { "222001","100222","222003","300222","220201","102022",
"220203","302022","102202","202201","302202","202203" };//�����߶�
const string whiteFive[1] = { "11111" };//��������
const string whiteFourA[1] = { "211112" };//�������
const string whiteFourD[7] = { "011112","211110","311112","211113","11121","12111","11211" };//��������
const string whiteThreeA[2] = { "221112","211122" };//����������
const string whiteThreeJ[2] = { "212112","211212" };//����������
const string whiteThreeD[9] = { "212110","011212","212113","311212","221110",
"011122","221113","311122","12121" };//��������
const string whiteTwoA[6] = { "211222","222112","221212","212122","212212" };//������
const string whiteTwoD[12] = { "222110","011222","222113","311222","221210","012122",
"221213","312122","012212","212210","312212","212213" };//�����߶�
matchResult result = notEnd;//�����û�л��ʤ��
int steps = 0;
const int limit = 5;
int timeH = 0, timeM = 10, timeS = 0;
//int bkHScaled = round(1000 * scale), bkWScaled = round(1000 * scale);//���ӱ����Ĵ�С
const int timerbkSizeScaledH = round(188 * scale);
const int timerbkSizeScaledW = round(387 * scale);
const int blackbkxScaled = round(1549 * scale), blackbkyScaled = round(114 * scale);
const int whitebkxScaled = round(1554 * scale), whitebkyScaled = round(1186 * scale);
const int lastBtnL = round(1549 * scale), lastBtnT = round(114 * scale);
const int lastBtnR = round(1935 * scale), lastBtnB = round(301 * scale);
const int nextBtnL = round(1554 * scale), nextBtnT = round(1186 * scale);
const int nextBtnR = round(1940 * scale), nextBtnB = round(1373 * scale);
bool isblack = true;