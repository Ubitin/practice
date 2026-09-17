//提问：指针也是一种数据类型，那么该数据类型占用多少内存空间呢？

#include<iostream>
using namespace std;
int main() {

	int a = 10;
	int* p = &a;
	
	//在32（64）位系统下，不管是什么数据类型，指针都占4（8）个字节
	cout << "sizeof int * = " << sizeof(int*) << endl;
	cout << "sizeof int * = " << sizeof(float*) << endl;
	cout << "sizeof int * = " << sizeof(double*) << endl;
	cout << "sizeof int * = " << sizeof(char*) << endl;

	system("pause");
	return 0;
}