//作用：整型变量表示的是整数类型的数据
//C++中能够表示整型的类型有以下方式，区别在于所占内存空间不同：
//数据类型               占用空间                                             取值范围
//short(短整型)          2字节                                                -2^15~2^15-1(-32768~32767)
//(最常用)int(整型)              4字节                                                -2^31~2^31-1
//long(长整型)           Windows为4字节，Linux为4字节(32位）,8字节(64位)      -2^31~2^31-1
//long long(长长整型)    8字节                                                -2^63~2^63-1
#include<iostream>
using namespace std;
int main() {
	//short(短整型) 
	short num1 = 10;
	//int(整型) 
	int num2 = 10;
	//long(长整型)
	long num3 = 10;
	//long long(长长整型)
	long long num4 = 10;
	cout << "num1 = " << num1 << endl;
	cout << "num2 = " << num2 << endl;
	cout << "num3 = " << num3 << endl;
	cout << "num4 = " << num4 << endl;
	system("pause");
	return 0;
}