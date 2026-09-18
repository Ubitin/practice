//语法：struct 结构体名{结构体成员列表}
/*通过结构体创建变量的方式有三种：
struct结构体名 变量名
struct结构体名 变量名={成员1值，成员2值...}
定义结构体时顺便创建变量*/

#include<iostream>
#include<string>
using namespace std;
//1、创建“学生”数据类型：学生包括（姓名，年龄，分数）
//自定义数据类型，一些类型集合组成的一个类型
//定义数据类型时，struct不可省略
struct Student
{
	string name;
	int age;
	int score;
}s3;

//2、通过学生类型创建具体学生

int main() {
	//2.1 struct Student s1
	//创建变量时，struct关键字可以省略
	Student s1;
	//给s1属性赋值,通过.来访问结构体变量中的属性
	s1.name = "张三";
	s1.age = 18;
	s1.score = 100;
	cout << "姓名：" << s1.name << endl << "年龄：" << s1.age << endl << "分数：" << s1.score << endl;
	//2.2 struct Student s2={}
	struct Student s2 = { "李四",19,80 };
	cout << "姓名：" << s2.name << endl << "年龄：" << s2.age << endl << "分数：" << s2.score << endl;
	//2.3 定义结构体时顺便创建变量
	s3.name = "王五";
	s3.age = 20;
	s3.score = 70;
	cout << "姓名：" << s3.name << endl << "年龄：" << s3.age << endl << "分数：" << s3.score << endl;
	system("pause");
	return 0;
}