//作用：通过指针访问结构体中的成员
//利用操作符 -> 可以通过结构体指针访问结构体属性

#include<iostream>
using namespace std;
struct student
{
	string name;
	int age;
	int score;
};

int main() {
	student s = { "张三",18,100 };
	//通过指针指向结构体变量
	student* p = &s;
	//通过指针访问结构体变量中的数据
	//需要用->访问
	cout << "姓名：" << p->name << endl;
	system("pause");
	return 0;
}