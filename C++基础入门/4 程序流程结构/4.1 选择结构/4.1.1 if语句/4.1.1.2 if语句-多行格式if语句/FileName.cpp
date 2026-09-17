//2、多行格式if语句：if（条件）{条件满足时执行的语句}else{条件不满足时执行的语句}
#include<iostream>
using namespace std;
int main() {

	//场景：用户输入考试分数，若分数大于600，则视为考上一本大学并在屏幕上输出；否则打印未考上一本大学

	//step1 输入考试分数
	int score = 0;
	cout << "请输入您的分数：" << endl;
	cin >> score;
	//step2 提示用户输入的分数
	cout << "您的分数为：" << score << endl;

	//step3 判断并打印
	if(score>=600)
	{
		cout << "恭喜您考上一本大学！" << endl;
	}
	else
	{
		cout << "很可惜您未能考上一本大学，请再接再厉！" << endl;
	}
	system("pause");
	return 0;
}