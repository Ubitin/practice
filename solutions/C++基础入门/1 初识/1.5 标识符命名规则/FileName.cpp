//作用：C++规定给标识符（变量、常量）命名时，有一套自己的规则
//1 标识符不能是关键字
//2 由字母、数字、下划线组成
//3 第一个字符必须为字母或下划线
//4 标识符中字母区分大小写
//tip:给标识命名时，争取做到见名知意的效果，方便自己和他人阅读
#include<iostream>
using namespace std;
int main() {
	//1 标识符不能是关键字，由字母、数字、下划线组成
	//int int = 10;
	//2 由字母、数字、下划线组成
	int abc = 10;
	int _abc = 20;
	int _123abc = 30;
	//3 第一个字符必须为字母或下划线
	//int 123abc = 40;
	//4 标识符中字母区分大小写
	int aaa = 100;
	cout << aaa << endl;
	//cout << AAA << endl;//AAA和aaa不是同一个名称
	//tip:给标识命名时，争取做到见名知意的效果，方便自己和他人阅读
	int num1 = 10;
	int num2 = 20;
	int sum = num1 + num2;
	cout << sum << endl;
	system("pause");
	return 0;
}