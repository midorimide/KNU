#include <thread>
#include <mutex>
#include <string>
#include <iostream>
#include <vector>

using namespace std;

constexpr int N = 100;
constexpr int H = 1000;
int bowl = 0;
bool full = false;
mutex mute;
condition_variable cond_var;

void bear() {
	unique_lock<mutex> lock(mute);
	while (true) {
		while (!full) cond_var.wait(lock);
		bowl = 0;
		full = false;
		cout << "Ведмiдь прокинувся, все з'їв i заснув" << endl;
		cond_var.notify_all();
	}
}

void bee(int index) {
	while (true) {
		unique_lock<mutex> lock(mute);
		string info;
		while (full) {
			info = "Бджола " + to_string(index) + " засинає.\n";
			cout << info;
			cond_var.wait(lock);
		}
		bowl++;
		info = "Бджола " + to_string(index) + "Приносить мед. В мискi " + to_string(bowl) + " порцiй меду.\n";
		cout << info;
		if (bowl == H) {
			info = "Бджола " + to_string(index) + " заповнила глечик i розбудила ведмедя.\n";
			full = true;
			cond_var.notify_all();
		}
	}
}

int main() {
	setlocale(LC_ALL, "Ukrainian");
	thread thr = thread(bear);
	vector<thread> bees(N);
	for (int i = 0; i < N; ++i) {
		bees[i] = thread(bee, i);
	}
	if (thr.joinable()) thr.join();
	for (thread& th : bees)
		if (th.joinable()) th.join();
	return 0;
}