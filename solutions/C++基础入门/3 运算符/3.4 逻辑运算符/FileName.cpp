/*作用：用于根据表达式的值返回真值或假值
有以下符号：
！ 非   && 与（且）   || 或*/
#include<iostream>
using namespace std;
int main() {

	//!
	
	int a = 10;
	cout << !a << endl;//0 在C++中，除了0，都为真，所以a为真，非a即为假

	cout << !!a << endl;//1
	//总结：真变假，假变真

	//&&  两个条件都为真，结果才为真

	int b = 10;
	int c = 10;
	cout << (c && b) << endl;//1    要加括号，使得逻辑运算优先

	b = 0;
	c = 10;
	cout << (c && b) << endl;//0

	b = 0;
	c = 0;
	cout << (b && c) << endl;//0
	//总结：同真为真，其余为假

	//||

	int d = 10;
	int e = 10;
	cout << (d || e) << endl;//1

	d = 0;
	e = 10;
	cout << (d || e) << endl;//1

	d = 0;
	e = 0;
	cout << (d || e) << endl;//0
	//总结：同假为假，其余为真

	system("pause");
	return 0;
}