/*作用：用于表示一串字符
两种风格
1.C风格字符串：char 变量名[] = "字符串值"
2.C++风格字符串：string  变量名 = "字符串值"*/

#include<iostream>
#include<string>//在用C++风格字符串时，要包含这个头文件
using namespace std;
int main() {
	
	//C风格
	//注意事项  1 char 字符串名 []
	//2 等号后面要用双引号括起字符串（区别于字符型用单引号）

	char str1[] = "hello world";
	cout << str1 << endl;

	//C++风格
	//注意事项  包含一个头文件  #include<string>

	string str2 = "hello world";
	cout << str2 << endl;

	system("pause");
	return 0;
}