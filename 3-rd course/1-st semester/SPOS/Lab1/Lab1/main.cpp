#include <Windows.h>
#include <iostream>
#include <string>

using namespace std;

const int time_elapse = 5000;

int f_val = -1, g_val = -1;
bool f_ret = false, g_ret = false;
PROCESS_INFORMATION f_pi, g_pi;
UINT m_uResolution, timerID;
bool user_promt = false;
DWORD t_start, t_end;

UINT setMultimediaTimer();

int binaryFunction() {
	return f_val + g_val;
}

void exitProgramm() {
	TerminateProcess(f_pi.hProcess, 0);
	TerminateProcess(g_pi.hProcess, 0);
	t_end = GetTickCount();
	cout << t_end - t_start << " miliseconds" << endl;
	timeKillEvent(timerID);
	timeEndPeriod(m_uResolution);
	system("pause");
	exit(0);
}

void checkFunctions() {
	if (f_val == 0) {
		cout << "Function F returns 0, result = 0" << endl;
		exitProgramm();
		return;
	}
	if (g_val == 0) {
		cout << "Function G returns 0, result = 0" << endl;
		exitProgramm();
		return;
	}
	if (f_ret && g_ret) {
		cout << "Result = " << binaryFunction() << endl;
		exitProgramm();
		return;
	}
}

int userPromt() {
	cout << "Hmm, looks like the computation continues too long." << endl;
	cout << "What should we do?" << endl;
	cout << "1.Continue" << endl;
	cout << "2.Continue without promt" << endl;
	cout << "3.Cancel" << endl;
	int choice;
	cin >> choice;
	system("cls");
	return choice;
}

PROCESS_INFORMATION startFunction(LPCSTR name, LPSTR info) {
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	CreateProcess(name, info, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi);

	return pi;
}

void cancellation() {
	checkFunctions();
	if (!f_ret) {
		cout << "F function hangs..." << endl;
	}
	if (!g_ret) {
		cout << "G function hangs..." << endl;
	}
	exitProgramm();
}

void msgProc(UINT msg, WPARAM wParam, LPARAM lParam) {
	switch (msg) {
	case WM_APP:
		switch (wParam) {
		case 'F':
			f_ret = true;
			f_val = lParam;
			break;
		case 'G':
			g_ret = true;
			g_val = lParam;
			break;
		default:
			break;
		}
		if (!user_promt) {
			checkFunctions();
		}
		break;
	case WM_HOTKEY:
		switch (wParam) {
		case 1:
			if (!user_promt) {
				cancellation();
			}
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void CALLBACK timerFunction(UINT wTimerID, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2) {
	if (!user_promt) {
		user_promt = true;
		switch (userPromt()) {
		case 1:
			timeKillEvent(timerID);
			timeEndPeriod(m_uResolution);
			timerID = setMultimediaTimer();
			break;
		case 2:
			timeKillEvent(timerID);
			timeEndPeriod(m_uResolution);
			break;
		case 3:
			cancellation();
			break;
		default:
			break;
		}
		checkFunctions();
		user_promt = false;
	}
}

UINT setMultimediaTimer() {
	TIMECAPS tc;
	timeGetDevCaps(&tc, sizeof(TIMECAPS));
	m_uResolution = min(max(tc.wPeriodMin, 0), tc.wPeriodMax);
	timeBeginPeriod(m_uResolution);

	return timeSetEvent(
		time_elapse,
		m_uResolution,
		timerFunction,
		NULL,
		TIME_PERIODIC);
}

int main() {
	string s;
	cin >> s;
	system("cls");

	s += " " + to_string(GetCurrentThreadId());

	t_start = GetTickCount();
	f_pi = startFunction("Lab1_F_Function.exe", (LPSTR)s.c_str());
	g_pi = startFunction("Lab1_G_Function.exe", (LPSTR)s.c_str());

	timerID = setMultimediaTimer();

	MSG msg;

	RegisterHotKey(NULL, 1, NULL, VK_ESCAPE);

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		msgProc(msg.message, msg.wParam, msg.lParam);
	}
	return 0;
}

