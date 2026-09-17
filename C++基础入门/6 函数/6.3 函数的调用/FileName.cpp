//功能：使用定义好的函数
//语法：函数名（参数）
#include<iostream>
using namespace std;
int add(int num1, int num2)//定义函数
{
	int sum = num1 + num2;//定义时num1，num2并没有真实数据，它只是一个形式上的参数，简称形参
	return sum;
}
int main()
{

	int a = 10;
	int b = 20;
	//调用add函数
	int sum = add(a, b);//调用时a,b称为实际参数，简称实参
	//调用函数时，实参的值会传递给形参
	cout << "sum = " << sum << endl;

	system("pause");
	return 0;
}