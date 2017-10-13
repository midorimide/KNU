#include <Windows.h>

int f(int x) {
	Sleep(4 * 1000);
	return x + 1;
}

int main(int argc, char* argv[]) {
	int res = f(atoi(argv[0]));
	PostThreadMessage((DWORD)atol(argv[1]), WM_APP, 'F', (LPARAM)res);
	return 0;
}
