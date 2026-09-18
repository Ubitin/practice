#include<iostream>
using namespace std;
struct student {
	string sname;
	int score = 0; // 在声明处初始化
};
struct teacher {
	string tname;
	student stu[5];
};

void allocateSpace(teacher*t,int len)
{
	string nameseed = "ABCDE";
	for (int i = 0;i < len;i++)
	{
		t[i].tname = "Teacher_";
		t[i].tname += nameseed[i];
		for(int j=0;j<5;j++)
		{
			t[i].stu[j].sname = "Student_";
			t[i].stu[j].sname += nameseed[j];
			int random = rand() % 61 + 40;//生成[0+40，60+40]的随机数
			t[i].stu[j].score = random;
		}
	}
}

void printInfo(teacher t[], int len)
{
	for (int i = 0;i < len;i++)
	{
		cout << "老师名：" << t[i].tname << endl;
		for (int j = 0;j < 5;j++)
		{
			cout << "\t学生名：" << t[i].stu[j].sname << " 考试分数：" << t[i].stu[j].score << endl;
		}
	}
}

void foo(const teacher& t)
{
	
}

int main() {
	teacher t[3];
	int len = sizeof(t) / sizeof(t[0]);
	allocateSpace(t,len);
	printInfo(t, len);
	system("pause");
	return 0;
}