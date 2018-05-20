#include <iostream>
#include <thread>
#include <mutex>
#include <queue>
#include <string>

using namespace std;

class Queue {

private:

	queue<int> q;
	mutex mute;
	condition_variable cond_var;
	bool flag;

public:

	void put(int val) {
		unique_lock<mutex> lock(mute);
		q.push(val);
		flag = true;
		cond_var.notify_all();
	}

	int get() {
		unique_lock<mutex> lock(mute);
		while (!flag) cond_var.wait(lock);
		int val = q.front();
		q.pop();
		if (q.empty()) flag = false;
		return val;
	}

	int size() {
		return q.size();
	}

	int front() {
		return q.front();
	}

};

Queue items1, items2;
mutex mute1, mute2;

void Ivanov() {
	srand(time(0));
	while (true) {		
		int item = rand() % 1000;
		items1.put(item);
		string str = "Iванов дiстав предмет вартiстю " + to_string(item) + " зi складу. Всього " + to_string(items1.size()) + " предметiв на вулицi.\n";
		cout << str;
	}
}

void Petrov() {
	while (true) {	
		unique_lock<mutex> lock(mute1);
		items2.put(items1.get());
		string str = "Петров поклав предмет вартiстю " + to_string(items1.front()) + " до машини. Всього " + to_string(items1.size() - 1) + " предметiв на вулицi.\n";
		cout << str;
	}
}

void Necheporuk() {
	int totalValue = 0;
	while (true) {
		unique_lock<mutex> lock(mute2);
		totalValue += items2.front();
		string str = "Нечепорук пiдрахував сумарну вартiсть. Вартiсть поточного предмета " + to_string(items2.front()) + ". Загальна вартiсть " + to_string(totalValue) + ".\n";
		cout << str;
		items2.get();
	}
}

int main() {
	setlocale(LC_ALL, "");
	thread thr1 = thread(Ivanov);
	thread thr2 = thread(Petrov);
	thread thr3 = thread(Necheporuk);
	if (thr1.joinable()) thr1.join();
	if (thr2.joinable()) thr2.join();
	if (thr3.joinable()) thr3.join();
	return 0;
}