//作用：让代码结构更加清晰
/*函数分文件编写一般有四步
1.创建后缀名为.h的头文件
2.创建后缀名为.cpp的源文件
3.在头文件中写函数的声明
4.在源文件中写函数的定义*/

//函数：实现两个整型数据的交换

#include<iostream>
using namespace std;
#include"swap.h"//用双引号，因为是自定义的头文件

//void swap(int a, int b);//函数的声明

//函数的定义
//void swap(int a, int b)
//{
//	int temp = a;
//	a = b;
//	b = temp;

//	cout << "a = " << a << endl;
//	cout << "b = " << b << endl;

//}

int main() {
	int a = 10;
	int b = 20;
	swap(a, b);

	system("pause");
	return 0;
}