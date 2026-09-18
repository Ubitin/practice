//作用：用const来防止误操作

#include<iostream>
using namespace std;
const struct Student 
{
	string name;
	int age;
	int score;
};

void PrintStudent(const Student *s)
{
	//s->age=150;
	cout << "姓名" << s->name << "年龄" << s->age << "成绩" << s->score << endl;
}

int main() {

	Student s = { "张三",15,98 };

	PrintStudent(&s);
	cout << "年龄" << s.age << endl;

	system("pause");
	return 0;
}