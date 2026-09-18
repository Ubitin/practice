//案例：三只小猪称体重
//有三只小猪ABC，请分别输入三只小猪的体重，并判断哪只小猪最重
#include<iostream>
using namespace std;
int main() {

	int A = 0;
	int B = 0;
	int C = 0;

	cout << "请输入小猪A的体重：" << endl;
	cin >> A;
	cout << "请输入小猪B的体重：" << endl;
	cin >> B;
	cout << "请输入小猪C的体重：" << endl;
	cin >> C;

	int MAX = max(A, B, C);

	int x = bool(A == MAX);
	int y = bool(B == MAX);
	int z = bool(C == MAX);

	if (x * y * z == 1)
	{
		cout << "三只小猪一样重" << endl;
	}

	else if ((x * y == 1)&&(z==0))
	{
		cout << "小猪A和B都最重" << endl;
	}

	else if ((x * z == 1) && (y == 0))
	{
		cout << "小猪A和C都最重" << endl;
	}
	else if ((y * z == 1) && (x == 0))
	{
		cout << "小猪B和C都最重" << endl;
	}
	else if (x == 1 && y == 0 && z == 0)
	{
		cout << "小猪A最重" << endl;
	}
	else if (y == 1 && x == 0 && z == 0)
	{
		cout << "小猪B最重" << endl;
	}
	else if (z == 1 && y == 0 && x == 0)
	{
		cout << "小猪C最重" << endl;
	}

	system("pause");
	return 0;
}