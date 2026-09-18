/*常量
作用：用于记录程序中不可更改的数据
C++定义常量的两种方式
1.#define 宏常量：#define 常量名 常量值
通常在文件上方定义，表示一个常量
2.const修饰的变量：const 数据类型 常量名 = 常量值*/

#include<iostream>
#define day 7
using namespace std;
int main() {
	//day = 14;//错误，day是常量，一旦修改就会报错
	cout << "一周的天数为：" << day << "天" << endl;
	
	const int month = 12;
	//month = 24;//错误，const修饰的变量也称为常量
	cout << "一个年的天数为：" << month << "月" << endl;
	system("pause");

	return 0;

}