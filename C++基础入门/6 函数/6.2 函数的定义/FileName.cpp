/*函数的定义一般有5个步骤：
1、返回值类型(返回成什么样的数据类型）
2、函数名
3、参数列表（给这个函数传入什么数据）
4、函数体语句（函数内部具体的代码）
5、return表达式（反馈什么结果）
语法：
返回值类型 函数名 （参数列表）
{

函数体语句

return表达式

}*/
//实现一个加法函数，功能是，传入两个整型数据，计算数据相加的结果并返回
/*1、返回值类型 int
2、函数名 add
3、参数列表 （int num1,int num2)
4、函数体语句 int sum=num1+num2;
5、return表达式 return sum;*/
#include<iostream>
using namespace std;
int add(int num1, int num2)
{
	int sum = num1 + num2;
	return sum;
}

int main() {
	system("pause");
	return 0;
}
