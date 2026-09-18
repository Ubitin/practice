/*作用：将自定义的结构体放入到数组中方便维护
语法：struct 结构体名 数组名[元素个数]={{}，{}，{}，{}...}*/

#include<iostream>
using namespace std;
struct Student
{
	string name;
	int age;
	int score;
};

int main() {

	Student arr[3]
	{
		{"张三",19,80},
		{"李四",18,70},
		{"王五",17,60}
	};
	for (int i = 0;i < 3;i++)
	{
		cout << "姓名：" << arr[i].name << endl << "年龄：" << arr[i].age << endl << "成绩：" << arr[i].score << endl;
	}

	system("pause");
	return 0;
}