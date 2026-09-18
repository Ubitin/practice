//作用：告诉编译器函数名称以及如何调用函数。函数的实际主体可以单独定义
//函数的声明可以多次，但是函数的定义只能有一次

//比较函数：实现两个整型数字大小的比较并返回较大的一个

#include<iostream>
using namespace std;

//提前告诉编译器max函数的存在，可以利用函数的声明，使得可以把函数的定义挪到main函数后面

int max(int a, int b);//无函数体

int main() {

	int a = 10;
	int b = 20;
	cout << max(a, b) << endl;

	system("pause");
	return 0;
}

//定义
int max(int a, int b)
{
	return a >= b ? a : b;//三目运算符
}