#include <string.h>
#include "rmd320.h"      

dword f1(dword x, dword y, dword z) {
	return x ^ y ^ z;
}

dword f2(dword x, dword y, dword z) {
	return (x & y) | (~x & z);
}

dword f3(dword x, dword y, dword z) {
	return (x | ~y) ^ z;
}

dword f4(dword x, dword y, dword z) {
	return (x & z) | (y & ~z);
}

dword f5(dword x, dword y, dword z) {
	return x ^ (y | ~z);
}

dword k[10] = { 0x00000000, 0x5A827999, 0x6ED9EBA1, 0x8F1BBCDC, 0xA953FD4E, 0x50A28BE6, 0x5C4DD124, 0x6D703EF3, 0x7A6D76E9, 0x00000000 };

dword r[160] = {	0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15,
					7, 4, 13, 1, 10, 6, 15, 3, 12, 0, 9, 5, 2, 14, 11, 8,
					3, 10, 14, 4, 9, 15, 8, 1, 2, 7, 0, 6, 13, 11, 5, 12,
					1, 9, 11, 10, 0, 8, 12, 4, 13, 3, 7, 15, 14, 5, 6, 2,
					4, 0, 5, 9, 7, 12, 2, 10, 14, 1, 3, 8, 11, 6, 15, 13,
					5, 14, 7, 0, 9, 2, 11, 4, 13, 6, 15, 8, 1, 10, 3, 12,
					6, 11, 3, 7, 0, 13, 5, 10, 14, 15, 8, 12, 4, 9, 1, 2,
					15, 5, 1, 3, 7, 14, 6, 9, 11, 8, 12, 2, 10, 0, 4, 13,
					8, 6, 4, 1, 3, 11, 15, 0, 5, 12, 2, 13, 9, 7, 10, 14,
					12, 15, 10, 4, 1, 5, 8, 7, 6, 2, 13, 14, 0, 3, 9, 11 };

dword s[160] = {	11, 14, 15, 12, 5, 8, 7, 9, 11, 13, 14, 15, 6, 7, 9, 8,
					7, 6, 8, 13, 11, 9, 7, 15, 7, 12, 15, 9, 11, 7, 13, 12,
					11, 13, 6, 7, 14, 9, 13, 15, 14, 8, 13, 6, 5, 12, 7, 5,
					11, 12, 14, 15, 14, 15, 9, 8, 9, 14, 5, 6, 8, 6, 5, 12,
					9, 15, 5, 11, 6, 8, 13, 12, 5, 12, 13, 14, 11, 8, 5, 6,
					8, 9, 9, 11, 13, 15, 15, 5, 7, 7, 8, 11, 14, 14, 12, 6,
					9, 13, 15, 7, 12, 8, 9, 11, 7, 7, 12, 7, 6, 15, 13, 11,
					9, 7, 15, 11, 8, 6, 6, 14, 12, 13, 5, 14, 13, 13, 7, 5,
					15, 5, 8, 11, 14, 14, 6, 14, 6, 9, 12, 9, 12, 5, 15, 8,
					8, 5, 12, 9, 12, 5, 14, 6, 8, 13, 6, 5, 15, 13, 11, 11 };

dword rotate(dword x, dword n) {
	return (x << n) | (x >> (32 - n));
}

void MDinit(dword *MDbuf) {
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

void compress(dword *MDbuf, dword *X) {
	dword words[10] = { MDbuf[0], MDbuf[1], MDbuf[2], MDbuf[3], MDbuf[4], MDbuf[5], MDbuf[6], MDbuf[7], MDbuf[8], MDbuf[9] };
	dword (*f[])(dword, dword, dword) = { f1, f2, f3, f4, f5 };
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

void MDfinish(dword *MDbuf, byte *strptr, dword lswlen, dword mswlen) {
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
