/*作用：用于表达式的比较，并返回一个真值或假值(0或1)
有以下符号：
== 相等于   != 不等于   < 小于   > 大于   <= 小于等于   >= 大于等于*/
#include<iostream>
using namespace std;
int main() {

	//==

	int a = 10;
	int b = 20;
	cout << (a == b) << endl;//要加括号，使得其运算优先级靠前

	//!=
	
	cout << (a != b) << endl;

	//<
	
	cout << (a < b) << endl;

	//>

	cout << (a > b) << endl;

	//<=

	cout << (a <= b) << endl;

	//>=

	cout << (a >= b) << endl;

	system("pause");
	return 0;
}