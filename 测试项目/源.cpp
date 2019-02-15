#define  _CRT_SECURE_NO_WARNINGS 
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int GetKeyPosition(FILE *fp,const char *Buffer,const char *KeyWord)
{
	int i = 0;
	char Str[1024] = { 0 };
	char Key[256] = { 0 };
	char *pTmp = Str;
	char *pStrstr = NULL;
	int KeyPostion = 0;
	strcpy(Str, Buffer);
	strcpy(Key, KeyWord);
	if (fp == NULL)
	{
		return -1;
	}
	if (Buffer == NULL || KeyWord == NULL)//传进来的字符串为空
	{
		return -1;
	}
	pStrstr = strstr(Str, Key);
	if (pStrstr == NULL)//如果没有找到返回-1
	{
		return -1;
	}
	while (pTmp != pStrstr)
	{
		KeyPostion++;
		pTmp++;
	}
	return KeyPostion;
}
void main()
{
	FILE *fp = NULL;
	fp = fopen("d:/1.txt", "r");
	if (fp == NULL)
	{
		printf("打开文件失败\n");
	}
	int position = 0;
	char *name = NULL;
	name = (char*)malloc(sizeof(char)*1024);
	sprintf(name, "123秘密123");
	position = GetKeyPosition(name, "秘密");


	printf("hello...\n");
	system("pause");
	return ;
}