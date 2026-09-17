/*作用：通过三目运算符实现简单的判断
语法：表达式1？表达式2：表达式3
如果表达式1的结果为真，执行表达式2并返回表达式2的结果
如果表达式1的结果为假，执行表达式3并返回表达式3的结果*/
#include<iostream>
using namespace std;
int main() {

	//创建三个变量a b c
	//将a和b做比较，将变量大的值赋值给变量c

	int a = 20;
	int b = 30;
	int c = 0;
	c = (a > b ? a : b);
	cout << "a和b中更大的值为：" << c << endl;

	//在C++中三目运算符返回的是变量，可以继续赋值
	(a > b ? a : b) = 100;//返回b的变量
	cout << "a = " << a << endl;
	cout << "b = " << b << endl;

	system("pause");
	return 0;
}
//三目运算符返回的可以为左值也可以为右值