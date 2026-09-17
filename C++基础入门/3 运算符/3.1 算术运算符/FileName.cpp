/*运算符的作用：用于执行代码的运算
算术运算符的作用：用于处理四则运算
运算符    +（正号/加号）   -（负号/减号）   *（乘）   /（除）
%（取模） eg.10%3的结果为1
++（前置递增/后置递增）   --（前置递减/后置递减）*/
#include<iostream>
using namespace std;
int main() {

	//1、加减乘除
	int a1 = 10;
	int b1 = 3;

	cout << a1 + b1 << endl;
	cout << a1 - b1 << endl;
	cout << a1 * b1 << endl;
	cout << a1 / b1 << endl;//输出为“3”，在C++中，认为整型变量与整型变量相除后的结果依然为整数，将小数部分去除

	int a2 = 10;
	int b2 = 20;
	cout << a2 / b2 << endl;

	int a3 = 10;
	int b3 = 0;
	//cout << a3 / b3 << endl;//错误，除数不能为0

	//两个小数可以相除
	double d1 = 0.5;
	double d2 = 0.25;
	double d3 = 0.22;
	cout << d1 / d2 << endl;
	cout << d1 / d3 << endl;//运算的结果也可以是小数

	//2、取模运算
	int c1 = 10;
	int c2 = 3;
	int c3 = 0;
	cout << c1 % c2 << endl;
	//cout << c1 % c3 << endl;//错误，除数不能为0

	//两个小数是不可以做取模运算的

	//3、递增，递减运算符
	
	//前置递增
	int a = 10;
	++a;//让变量+1
	cout <<"a = "<< a << endl;

	//后置递增
	int b = 10;
	b++;//让变量+1
	cout << "b = " << b << endl;

	/*前置与后置的区别
	前置递增 先让变量+1 然后进行表达式的运算
	后置递增 先进行表达式的运算 然后让变量+1*/
	int f = 10;
	int e = ++f * 10;//e=(f+1)*10=f'*10  f'=f+1
	cout << "f = " << f << endl;
	cout << "e = " << e << endl;
	
	int g = 10;
	int h = g++ * 10;//h=g*10 g'=g+1
	cout << "g = " << g << endl;
	cout << "h = " << h << endl;

	system("pause");
	return 0;
}