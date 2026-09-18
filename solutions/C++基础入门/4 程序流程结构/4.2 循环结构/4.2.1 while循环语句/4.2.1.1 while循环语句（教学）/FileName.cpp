/*作用：满足循环条件，执行循环语句
语法：while（循环条件）{循环语句}
解释：只要循环条件的结果为真，就执行循环语句*/
#include<iostream>
using namespace std;
int main() {

	//在屏幕中打印0~9这十个数
	int num = 0;

	//while()中填入循环条件时一定要避免死循环的出现
	while (num <= 9)
	{
		cout << num << endl;
		num++;
	}
	system("pause");
	return 0;
}