/*作用：字符型变量用于显示单个字符
语法：char ch = 'a';
#注意1：在显示字符型变量时，用单引号将字符括起来，不要用双引号
#注意2：单引号内只能有一个字符，不可以是字符串
C和C++语言中字符型变量只占用1个字节
字符型变量并不是把字符本身放到内存中储存，而是将对应的ACSⅡ编码放入储存单元*/
#include<iostream>
using namespace std;
int main() {
	
	//1、字符型变量创建方式
	char ch = 'a';
	cout << ch << endl;
	
	//2、字符型变量所占内存的大小
	cout << "char字符型变量所占的内存：" << sizeof(ch) << endl;

	//3、字符型变量常见错误
	//char ch2 = "b";//创建字符型变量时要用单引号
	//char ch2='abcdef'//创建字符型变量时，单引号内只能有一个字符

	//4、字符型变量对应ASCⅡ编码
	//a - 97
	//A - 65
	cout << (int)ch << endl;

	system("pause");
	return 0;
}