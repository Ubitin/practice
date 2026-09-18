//作用：将结构体作为参数向函数中传递
//两种传递方式
//值传递，地址传递

#include<iostream>
using namespace std;
struct student
{
	string name;
	int age;
	int score;
};

void printStudent(student s)
{
	cout << "子函数中 姓名：" << s.name << "年龄：" << s.age << "成绩：" << s.score << endl;
}

void printStudent2(student* p)
{
	cout << "子函数中 姓名：" << p->name << "年龄：" << p->age << "成绩：" << p->score << endl;
}
int main() {
	student s;
	s.name = "张三";
	s.age = 18;
	s.score = 100;
	printStudent(s);
	printStudent2(&s);
	system("pause");
	return 0;
}