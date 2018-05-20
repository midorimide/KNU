#include <thread>
#include <iostream>
#include <vector>

using namespace std;

vector<int> power;

int compare(int w1, int w2) {
	if (w1 == -1) return w2;
	if (w2 == -1) return w1;
	if (power[w1] > power[w2]) {
		power[w1] -= power[w2];
		return w1;
	}
	if (power[w1] < power[w2]) {
		power[w2] -= power[w1];
		return w2;
	}
	if (power[w1] == power[w2]) return -1;
}

void winner(int l, int r, int& w) {
	if (l == r) {
		w = l;
		return;
	}
	int m = (l + r) / 2;
	int w1, w2;
	thread thr1(winner, l, m, ref(w1));
	thread thr2(winner, m + 1, r, ref(w2));
	if (thr1.joinable()) thr1.join();
	if (thr2.joinable()) thr2.join();
	w = compare(w1, w2);
}

int main() {
	setlocale(LC_ALL, "");
	int n;
	cout << "¬ведiть кiлькiсть монахiв" << endl;
	cin >> n;
	power.resize(n);
	cout << endl << "¬ведiть силу кожного з монахiв" << endl;
	for (int& val : power) {
		cin >> val;
	}
	int w;
	thread thr(winner, 0, n - 1, ref(w));
	if (thr.joinable()) thr.join();
	if (w == -1) cout << endl << "ѕеремогла дружба!" << endl;
	else cout << endl << "ѕеремiг монах номер " << w + 1 << endl;
	system("pause");
	return 0;
}