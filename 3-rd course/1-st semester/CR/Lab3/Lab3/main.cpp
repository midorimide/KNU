#include <string>
#include <iostream>
#include "rmd320.h"

byte *RMD(byte *message)
{
	dword         MDbuf[10];  
	static byte   hashcode[40]; 
	dword         X[16];    

	MDinit(MDbuf);
	dword length = (dword)strlen((char *)message);

	for (dword nbytes = length; nbytes > 63; nbytes -= 64) {
		for (int i = 0; i < 16; i++) {
			X[i] = BYTES_TO_DWORD(message);
			message += 4;
		}
		compress(MDbuf, X);
	}                                   

	MDfinish(MDbuf, message, length, 0);

	for (int i = 0; i < 40; i += 4) {
		hashcode[i] = MDbuf[i >> 2];       
		hashcode[i + 1] = (MDbuf[i >> 2] >> 8); 
		hashcode[i + 2] = (MDbuf[i >> 2] >> 16);
		hashcode[i + 3] = (MDbuf[i >> 2] >> 24);
	}

	return (byte *)hashcode;
}

int main()
{
	std::string msg;
	std::cin >> msg;
	byte* res = RMD((byte*)msg.c_str());
	for (int i = 0; i < 40; ++i) {
		printf("%02x", res[i]);
	}
	printf("\n");

	system("pause");
	return 0;
}