//作用：利用sizeof关键字可以统计数据类型所占内存的大小
//语法：sizeof(数据类型/变量)
#include<iostream>
using namespace std;
int main() {
	cout << "short所占的内存空间为" << sizeof(short) << endl;
	int num1 = 10;
	cout << "int所占的内存空间为" << sizeof(num1) << endl;
	system("pause");
	return 0;
}