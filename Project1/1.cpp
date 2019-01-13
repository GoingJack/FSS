#include "stdio.h"
#include "stdlib.h"
int main()
{
	system("dir /b /a-d /s c:\\*.* >d:\\allfiles.txt");
}