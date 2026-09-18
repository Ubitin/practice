//作用：关键字是C++中预先保留的单词（标识符）
//在定义变量或常量时，不要用关键字
#include<iostream>
using namespace std;

int main() {
	//创建变量：数据类型 变量名称 = 初始值
	//不要用关键字给变量或者常量起名
	//int int = 10;//错误，第二个int是关键字，不可以作为变量名称
	//正确的如下：
	int a = 7;
	cout << "a = " << a << endl;
	system("pause");

	return 0;
}