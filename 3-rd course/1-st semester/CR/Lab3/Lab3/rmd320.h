#pragma once

typedef    unsigned char        byte;
typedef    unsigned long        dword;

#define BYTES_TO_DWORD(strptr)                    \
            (((dword) *((strptr)+3) << 24) | \
             ((dword) *((strptr)+2) << 16) | \
             ((dword) *((strptr)+1) <<  8) | \
             ((dword) *(strptr)))

void MDinit(dword *MDbuf);

void compress(dword *MDbuf, dword *X);

void MDfinish(dword *MDbuf, byte *strptr, dword lswlen, dword mswlen);