#include <Windows.h>

int g(int x) {
	Sleep(6 * 1000);
	return x;
}

int main(int argc, char* argv[]) {
	int res = g(atoi(argv[0]));
	PostThreadMessage((DWORD)atol(argv[1]), WM_APP, 'G', (LPARAM)res);
	return 0;
}

