/*作用：执行满足条件的语句
if语句的三种形式：
·单行格式if语句
·多行格式if语句
·多条件的if语句*/
#include<iostream>
using namespace std;
int main() {

	//1、单行格式if语句：if(条件){条件满足后执行的语句}
	//若条件为真，则执行语句；反之则结束程序

	//场景：用户输入分数，若分数大于600，视为考上一本大学，并在屏幕上输出

	//step1 用户输入分数
	int score = 0;
	cout << "请输入您的分数：" << endl;
	cin >> score;

	//step2 打印用户输入的分数
	cout << "您输入的分数为：" << score << endl;

	//step3 判断分数是否大于600
	if (score > 600)//后面一定不能加“；”，不然视为与下列代码无关，不管条件是否满足，下面的程序都会执行
	{
		cout << "恭喜您考上了一本大学！" << endl;
	}


	system("pause");
	return 0;
}