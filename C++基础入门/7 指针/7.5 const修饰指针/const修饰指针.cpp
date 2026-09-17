/*const修饰指针有三种情况：
1.const修饰指针——常量指针 const int * p = &a;特点：指针的指向可以修改，但指针指向的值不可以改
2.const修饰常量——指针常量 int * const p = &a;特点：指针的指向不可修改，但指针指向的值可以修改
3.const既修饰指针，又修饰常量 const int * const p = &a;特点：指针的指向和指向的值均不可以修改*/

#include<iostream>
using namespace std;
int main() {

	//1.
	int a = 10;
	int b = 10;
	const int* p = &a;//常量指针
	int* p = &b;
	//*p=20;错误，常量指针指向的值不可以修改

	//2.
	int* const p2 = &a;//指针常量
	* p2 = 20;
	//p=&b;错误，指针常量的指向不可以修改

	//3.
	const int* const p3 = &a;//既修饰指针，又修饰常量
	//p3=&b;
	//*p3=20;

	system("pause");
	return 0;
}