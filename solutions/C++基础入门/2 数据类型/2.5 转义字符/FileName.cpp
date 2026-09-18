/*作用：用于表示一些不能显示出来的ASCⅡ字符
现阶段我们常用的转义字符有：\n    \\      \t
转义字符       含义                                        ASCⅡ码值（十进制）
\n             换行（LF），将当前位置移到下一行开头        010
\\             表示一个反斜线字符"\"                       092
\t             水平制表（HT)(跳到下一个TAB位置）           009*/

#include<iostream>
using namespace std;
int main() {
	//换行符  \n

	cout << "hello world\n";

	//反斜杠  \\

	//cout<<"\"<<endl;//错误，要输入两个反斜杠才可以输出一个反斜杠
	cout<<"\\"<<endl;

	//水平制表符  \t        作用：对齐，可以整齐地输出数据

	cout << "aaa\thello world" << endl;//"\t"前的字符长度加上"\t"之后的空格长度为8个
	cout << "aaaaa\thello world" << endl;
	cout << "aa\thello world" << endl;
	//对比如下
	cout << "aaa hello world" << endl;
	cout << "aaaaa hello world" << endl;
	cout << "aa hello world" << endl;
	
	system("pause");
	return 0;

}