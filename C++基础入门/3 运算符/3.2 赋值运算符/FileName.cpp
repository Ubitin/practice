/*作用：用于将表达式的值赋给变量
包括以下几个符号：
= 赋值   += 加等于   -= 减等于   *= 乘等于   /= 除等于   %= 模等于*/

#include<iostream>
using namespace std;
int main() {

	//=
	int a = 10;
	cout << "a= " << a << endl;

	//+=
	a = 10;
	a += 2;// a = a + 2;
	cout << "a = " << a << endl;

	//-=
	a = 10;
	a -= 2;// a = a - 2;
	cout << "a = " << a << endl;

	//*=
	a = 10;
	a *= 2;// a = a * 2;
	cout << "a = " << a << endl;

	///=
	a = 10;
	a /= 2;// a = a / 2;
	cout << "a = " << a << endl;

	//%=
	a = 10;
	a %= 3;
	cout << "a = " <<a<< endl;

	system("pause");
	return 0;
}