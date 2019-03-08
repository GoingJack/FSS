#include<iostream>
#include <fstream>
#include <windows.h>
using namespace std;
struct FileType
{
	char name[100];
	char Summary[100];
};

int file()
{
	char data[100];

	// 以写模式打开文件
	ofstream outfile;
	outfile.open("afile.dat");

	cout << "Writing to the file" << endl;
	cout << "Enter your name: ";
	cin.getline(data, 100);

	// 向文件写入用户输入的数据
	outfile << data << endl;

	cout << "Enter your age: ";
	cin >> data;
	cin.ignore();

	// 再次向文件写入用户输入的数据
	outfile << data << endl;

	// 关闭打开的文件
	outfile.close();

	// 以读模式打开文件
	ifstream infile;
	infile.open("afile.dat");

	cout << "Reading from the file" << endl;
	infile >> data;

	// 在屏幕上写入数据
	cout << data << endl;

	// 再次从文件读取数据，并显示它
	infile >> data;
	cout << data << endl;

	// 关闭打开的文件
	infile.close();

	return 0;
}

int main()
{
	//file();
	FileType s_File[3];
	sprintf(s_File[0].name, "C:/Desktop/hello.docx");
	sprintf(s_File[0].Summary, "这里有秘密呦吼");
	sprintf(s_File[1].name, "C:/Desktop/world.docx");
	sprintf(s_File[1].Summary, "秘密即使没有");
	sprintf(s_File[2].name, "C:/Desktop/你好.docx");
	sprintf(s_File[2].Summary, "没有秘密的文件");
	char data[100];
	sprintf(data, "abcdefg");
	SYSTEMTIME sys;
	GetLocalTime(&sys);
	char *LogFilename = (char*)malloc(sizeof(char) * 30);
	sprintf(LogFilename, "%4d-%02d-%02d-%02d-%2d-%2d.log", sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);

	ofstream outfile;
	outfile.open(LogFilename);

	//outfile << data << endl;
	for (int i = 0; i < 3; i++)
	{
		char tmp[1024] = { 0 };
		sprintf(tmp, "%s------%s", s_File[i].name, s_File[i].Summary);
		outfile << tmp << endl;
	}
	outfile.close();
	system("pause");
	return 0;
}

