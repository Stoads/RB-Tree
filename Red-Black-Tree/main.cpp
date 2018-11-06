#include "test.h"
#include <cstdio>
#ifdef _WIN32
#include <Windows.h>
#endif
int main() {
	test1();
	printf("test1 Successed\n");
	test2();
	printf("test2 Successed\n");
#ifdef _WIN32
	system("pause");
#else
	scanf(" %c");
#endif
}