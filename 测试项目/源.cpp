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
	if (Buffer == NULL || KeyWord == NULL)//���������ַ���Ϊ��
	{
		return -1;
	}
	pStrstr = strstr(Str, Key);
	if (pStrstr == NULL)//���û���ҵ�����-1
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
		printf("���ļ�ʧ��\n");
	}
	int position = 0;
	char *name = NULL;
	name = (char*)malloc(sizeof(char)*1024);
	sprintf(name, "123����123");
	position = GetKeyPosition(name, "����");


	printf("hello...\n");
	system("pause");
	return ;
}