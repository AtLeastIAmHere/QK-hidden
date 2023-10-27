#define _CRT_SECURE_NO_WARNINGS
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "math.h"
#include "time.h"
#include <iostream>
#include<cstring>
#include<fstream>
#include<sstream>
#include <algorithm>
using namespace std;
#define N 600000
#define M 200000
#define RN 500
#define RM 20

char NDb[M][8192] = {};
char s[M];
int m;
int cn;
double r;
int len = 0;
double p[4];
double q[8];
typedef struct {
	int p[3];				//三个位 
	char c[4];
}Ent;
Ent NDB[N];
double posbility[8][2] = { 0 };


double xq[30000][1024];
int N0[8192];//第i个负数据库中第j个位为0的个数
int N1[8192];
double pow_b[9] = { 1,2,4,8,16,32,64,128,256 };
const int L = 8;
double Q[1024][256];
double q0[10], q1[10];

double  diff(int i) {                  //计算每一位与原串不同的概率
	double Ndiff = 0;
	double Nsame = 0;
	for (int j = 1; j <= 3; j++) {
		Ndiff += j * p[j] * q[i];
	}
	for (int j = 1; j <= 3; j++) {
		Nsame += ((3 - j) * p[j]) / 8;
	}

	double Pdiff = 0;
	Pdiff = Ndiff / (Ndiff + Nsame);
	return Pdiff;

}
void init(double cr)
{
	m = 8192;
	r = cr;
	p[0] = 0;
	p[1] = 0.70;
	p[2] = 0.24;
	p[3] = 1 - p[1] - p[2];
	cn = int(m * r + 0.5);


	q[0] = 0.95;
	q[1] = 0.0;
	q[2] = 0.0;
	q[3] = 0.0;
	q[4] = 0.0;
	q[5] = 0.0;
	q[6] = 0.0;
	q[7] = 0.05;


	for (int i = 0; i < 8; i++) {
		posbility[i][0] = diff(i);
		posbility[i][1] = 1 - posbility[i][0];
	}
}

int rand1(int n)
{
	//	return rand()%n;
	return 0 + (int)n * rand() / (RAND_MAX + 1);
}

double rand1()
{
	return (double)rand() / (RAND_MAX + 1.0);
}

int  generateRandomNumbers(double l)
{

	if (l < q[0]) return 0;
	else if (l < q[0] + q[1]) return 1;
	else if (l < q[0] + q[1] + q[2])return 2;
	else if (l < q[0] + q[1] + q[2] + q[3])return 3;
	else if (l < q[0] + q[1] + q[2] + q[3] + q[4])return 4;
	else if (l < q[0] + q[1] + q[2] + q[3] + q[4] + q[5])return 5;
	else if (l < q[0] + q[1] + q[2] + q[3] + q[4] + q[5] + q[6])return 6;
	else return 7;

}


void addToNDB(Ent x)		//负数据库确定位赋值 
{
	int i;
	for (i = 0; i < 3; i++)
	{
		NDB[cn].p[i] = x.p[i];
		NDB[cn].c[i] = x.c[i];
	}
	cn++;
}

void f(char s[])		//生成s[]的负数据库	
{
	Ent v;
	int n;
	double t;
	double u;
	m = 8192;

	len = m / L;

	n = int(m * r + 0.5);


	int bit1 = 0;
	int bit2 = 0;
	int bit3 = 0;
	int attr1 = 0;
	int attr2 = 0;
	int attr3 = 0;

	cn = 0;

	int counter = 0;
	do
	{
		//取3个随机位 
		t = rand1();				//生成0~1之间的小数 
		if (t < p[1])                 //生成类型一
		{

			u = rand1();
			v.p[0] = generateRandomNumbers(u) + rand1(len) * L;
			v.c[0] = '1' + '0' - s[v.p[0]];


			bit2 = rand1(L);
			attr2 = rand1(len);

			while ((bit2 + attr2 * L) == v.p[0]) {
				bit2 = rand1(L);
				attr2 = rand1(len);
			}
			v.p[1] = bit2 + attr2 * L;
			v.c[1] = s[v.p[1]];


			bit3 = rand1(L);
			attr3 = rand1(len);
			while ((bit3 + attr3 * L) == v.p[0] || (bit3 + attr3 * L) == v.p[1]) {
				bit3 = rand1(L);
				attr3 = rand1(len);
			}
			v.p[2] = bit3 + attr3 * L;
			v.c[2] = s[v.p[2]];


		}
		else if (t < p[1] + p[2])
		{



			v.p[0] = generateRandomNumbers(rand1()) + rand1(len) * L;
			v.c[0] = '1' + '0' - s[v.p[0]];

			bit2 = generateRandomNumbers(rand1());
			attr2 = rand1(len);
			while ((bit2 + attr2 * L) == v.p[0]) {
				bit2 = generateRandomNumbers(rand1());
				attr2 = rand1(m / L);
			}
			v.p[1] = bit2 + attr2 * L;
			v.c[1] = '1' + '0' - s[v.p[1]];

			attr3 = rand1(len);
			bit3 = rand1(L);
			while ((bit3 + attr3 * L) == v.p[1] || (bit3 + attr3 * L) == v.p[0]) {
				bit3 = rand1(L);
				attr3 = rand1(len);
			}
			v.p[2] = bit3 + attr3 * L;
			v.c[2] = s[v.p[2]];

		}
		else
		{
			v.p[0] = generateRandomNumbers(rand1()) + rand1(len) * L;
			v.c[0] = '1' + '0' - s[v.p[0]];

			bit2 = generateRandomNumbers(rand1());
			attr2 = rand1(len);
			while ((bit2 + attr2 * L) == v.p[0]) {
				bit2 = generateRandomNumbers(rand1());
				attr2 = rand1(m / L);     //当q设置中有0时  极端情况
			}
			v.p[1] = bit2 + attr2 * L;      //生成的反转位
			v.c[1] = '1' + '0' - s[v.p[1]];

			bit3 = generateRandomNumbers(rand1());
			attr3 = rand1(len);
			while ((bit3 + attr3 * L) == v.p[1] || (bit3 + attr3 * L) == v.p[0]) {
				bit3 = generateRandomNumbers(rand1());  //正常参数设置情况
				attr3 = rand1(m / L);  //当q设置中有0时 极端情况
			}
			v.p[2] = bit3 + attr3 * L;
			v.c[2] = '1' + '0' - s[v.p[2]];

		}
		addToNDB(v);
	} while (cn < n);


}



void printNDB()
{

	int i, j;
	for (i = 0; i < cn; i++)
	{
		for (j = 0; j < 3; j++)
		{
			cout << NDB[i].p[j] << " " << NDB[i].c[j] << "  ";

		}
		cout << endl;
	}




}



int main() {

	srand((unsigned)(time(0)));

	double change_r;

	string filemnist[7] = { "\\train_1.txt","\\train_2.txt", "\\train_3.txt", "\\train_4.txt",
						  "\\train_5.txt", "\\test.txt", "\\mnist_test.txt", };

	for (int cr = 2; cr <= 2; cr += 2) {
		change_r = 6.5;
		init(change_r);
		for (int i = 0; i < L; i++) {
			cout << posbility[i][0] << " " << posbility[i][1] << " ";
		}
		string filein[7] = { "..\\128\\二进制串\\train_1.txt",
							 "..\\128\\二进制串\\train_2.txt",
							 "..\\128\\二进制串\\train_3.txt",
							 "..\\128\\二进制串\\train_4.txt",
							 "..\\128\\二进制串\\train_5.txt",
							 "..\\128\\二进制串\\test.txt",

		};
		string fileout[7] = { "..\\128\\q\\12" + filemnist[0],
							  "..\\128\\q\\12" + filemnist[1],
							  "..\\128\\q\\12" + filemnist[2],
							  "..\\128\\q\\12" + filemnist[3],
							  "..\\128\\q\\12" + filemnist[4],
							  "..\\128\\q\\12" + filemnist[5],
							  "..\\MNIST\\数据集\\ChangeP\\080\\" + filemnist[6],

		};
		for (int each = 0; each < 6; each++) {

			ifstream myfile;
			string tmp;
			myfile.open(filein[each].c_str());
			int count = 0;
			while (!myfile.eof())   //按行读取,遇到换行符结束 
			{
				myfile >> tmp;
				if (myfile.peek() == EOF)  break;
				(strcpy(NDb[count++], tmp.c_str()));
				//if (count == 30) break;
			}
			myfile.close();
			cout << count << "张图片的01串已经生成" << endl;
			time_t  start = time(NULL);
			for (int i = 0; i < count; i++) {
				f(NDb[i]); //生成每张图片所对应的负数据库记录。
				if (i % 2500 == 0) { cout << "第" << i << "张图片已经生成" << endl; }

			}
			time_t  end = time(NULL);
			cout << "负数据库生成时间：" << difftime(end, start) << "秒" << endl;
			
		}
	}


}
