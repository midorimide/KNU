#include <string>
#include <iostream>
#include "rmd320.h"

int main()
{
	std::string msg;
	std::cin >> msg;
	byte* res = RIPEMD320::hash((byte*)msg.c_str());
	for (int i = 0; i < 40; ++i) {
		printf("%02x", res[i]);
	}
	printf("\n");

	system("pause");
	return 0;
}