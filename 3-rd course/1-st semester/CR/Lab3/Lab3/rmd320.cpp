#include <string.h>
#include "rmd320.h"      

dword RIPEMD320::f1(dword x, dword y, dword z) {
	return x ^ y ^ z;
}

dword RIPEMD320::f2(dword x, dword y, dword z) {
	return (x & y) | (~x & z);
}

dword RIPEMD320::f3(dword x, dword y, dword z) {
	return (x | ~y) ^ z;
}

dword RIPEMD320::f4(dword x, dword y, dword z) {
	return (x & z) | (y & ~z);
}

dword RIPEMD320::f5(dword x, dword y, dword z) {
	return x ^ (y | ~z);
}

dword RIPEMD320::rotate(dword x, dword n) {
	return (x << n) | (x >> (32 - n));
}

dword RIPEMD320::byte_to_word(byte * strptr) {
	return (dword) *((strptr)+3) << 24 | (dword) *((strptr)+2) << 16 | (dword) *((strptr)+1) << 8 | (dword) *(strptr);
}

void RIPEMD320::MDinit(dword *MDbuf) {
	MDbuf[0] = 0x67452301UL;
	MDbuf[1] = 0xefcdab89UL;
	MDbuf[2] = 0x98badcfeUL;
	MDbuf[3] = 0x10325476UL;
	MDbuf[4] = 0xc3d2e1f0UL;
	MDbuf[5] = 0x76543210UL;
	MDbuf[6] = 0xFEDCBA98UL;
	MDbuf[7] = 0x89ABCDEFUL;
	MDbuf[8] = 0x01234567UL;
	MDbuf[9] = 0x3C2D1E0FUL;
}

void RIPEMD320::compress(dword *MDbuf, dword *X) {
	dword words[10] = { MDbuf[0], MDbuf[1], MDbuf[2], MDbuf[3], MDbuf[4], MDbuf[5], MDbuf[6], MDbuf[7], MDbuf[8], MDbuf[9] };
	dword(*f[])(dword, dword, dword) = { f1, f2, f3, f4, f5 };
	dword temp;
	for (int i = 0; i < 80; ++i) {
		temp = rotate(words[0] + f[i / 16](words[1], words[2], words[3]) + X[r[i]] + k[i / 16], s[i]) + words[4];
		words[0] = words[4]; words[4] = words[3]; words[3] = rotate(words[2], 10); words[2] = words[1]; words[1] = temp;
		temp = rotate(words[5] + f[4 - i / 16](words[6], words[7], words[8]) + X[r[i + 80]] + k[i / 16 + 5], s[i + 80]) + words[9];
		words[5] = words[9]; words[9] = words[8]; words[8] = rotate(words[7], 10); words[7] = words[6]; words[6] = temp;
		if (i == 15) {
			temp = words[1]; words[1] = words[6]; words[6] = temp;
		} else if (i == 31) {
			temp = words[3]; words[3] = words[8]; words[8] = temp;
		} else if (i == 47) {
			temp = words[0]; words[0] = words[5]; words[5] = temp;
		} else if (i == 63) {
			temp = words[2]; words[2] = words[7]; words[7] = temp;
		} else if (i == 79) {
			temp = words[4]; words[4] = words[9]; words[9] = temp;
		}
	}
	for (int i = 0; i < 10; ++i) {
		MDbuf[i] += words[i];
	}
}

void RIPEMD320::MDfinish(dword *MDbuf, byte *strptr, dword lswlen, dword mswlen) {
	unsigned int i;
	dword        X[16];

	memset(X, 0, 16 * sizeof(dword));

	for (i = 0; i<(lswlen & 63); i++) {
		X[i >> 2] ^= (dword)*strptr++ << (8 * (i & 3));
	}

	X[(lswlen >> 2) & 15] ^= (dword)1 << (8 * (lswlen & 3) + 7);

	if ((lswlen & 63) > 55) {
		compress(MDbuf, X);
		memset(X, 0, 16 * sizeof(dword));
	}

	X[14] = lswlen << 3;
	X[15] = (lswlen >> 29) | (mswlen << 3);
	compress(MDbuf, X);
}

byte * RIPEMD320::hash(byte * message) {
	dword         MDbuf[10];
	static byte   hashcode[40];
	dword         X[16];

	MDinit(MDbuf);
	dword length = (dword)strlen((char *)message);

	for (dword nbytes = length; nbytes > 63; nbytes -= 64) {
		for (int i = 0; i < 16; i++) {
			X[i] = byte_to_word(message);
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
