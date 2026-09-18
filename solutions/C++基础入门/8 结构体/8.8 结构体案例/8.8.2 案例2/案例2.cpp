#include<iostream>
using namespace std;
struct hero
{
	string name;
	int age;
	string sex;
};

void bubble(hero* hArry, int len)
{
	for (int i = 0;i < len-1;i++)
	{
		for (int j = i + 1;j < len;j++)
		{
			if (hArry[i].age > hArry[j].age)
			{
				hero temp = hArry[i];
				hArry[i] = hArry[j];
				hArry[j] = temp;
			}
		}
	}
}

void printinfo(hero hArry[], int len)
{
	for (int i = 0;i < len;i++)
	{
		cout << "姓名：" << hArry[i].name << " 年龄：" << hArry[i].age << " 性别:" << hArry[i].sex << endl;
	}
}

int main() {
	string nameseed[5] = { "刘备","关羽","张飞","赵云","貂蝉" };
	string sex[5] = { "男","男", "男", "男", "女" };
	int age[5] = { 23,22,20,21,19 };
	hero hArry[5];
	int len = sizeof(hArry) / sizeof(hArry[0]);
	for (int k=0;k < len;k++)
	{
		hArry[k].name = nameseed[k];
		hArry[k].age = age[k];
		hArry[k].sex = sex[k];
	}
	bubble(hArry, len);
	printinfo(hArry, len);
	system("pause");
	return 0;
}