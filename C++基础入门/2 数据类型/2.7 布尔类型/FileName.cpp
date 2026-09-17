/*作用：布尔数据类型代表真或假的值
bool类型只有两个值：
true   真（本质是1）
false  假（本质是0）
bool类型占1个字节大小*/
#include<iostream>
using namespace std;
int main() {

	//1、创建bool数据类型

	bool flag = true;//true代表真（即1）
	cout << flag << endl;

	flag = false;//false代表假（即0）
	cout << flag << endl;

	//2、bool数据类型所占的内存空间

	cout << "bool类型所占的内存空间：" << sizeof(flag) << endl;

	system("pause");
	return 0;
}