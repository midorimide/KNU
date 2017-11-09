#include <thread>
#include <vector>
#include <algorithm>
#include <time.h>

#define THREAD_COUNT 100
#define MAX_TICKET 10

using namespace std;

vector<int> tickets(THREAD_COUNT, 0);
vector<int> choosing(THREAD_COUNT, 0);
int resource = 0;

void lock(int thread) {
	int max;
	do {
		choosing[thread] = 1;
		max = *max_element(tickets.begin(), tickets.end());
		if (++max <= MAX_TICKET) {
			tickets[thread] = max;
			choosing[thread] = 0;
			break;
		}
		choosing[thread] = 0;
		this_thread::sleep_for(chrono::milliseconds(100));
	} while (max > MAX_TICKET);
	for (int other = 0; other < THREAD_COUNT; ++other) {
		while (choosing[other] == 1);
		while (tickets[other] &&
			(tickets[other] < tickets[thread] ||
			(tickets[other] == tickets[thread] && other < thread)));
	}
}

void unlock(int thread) {
	tickets[thread] = 0;
	printf("Thread %d finished\n", thread);
}

void use_resource(int thread) {
	if (resource != 0) {
		printf("Resource was acquired by process %d (ticket %d), but is still in-use by process %d (ticket %d)!\n",
			thread, tickets[thread], resource, tickets[resource]);
	}
	resource = thread;
	this_thread::sleep_for(chrono::milliseconds(200 + rand() % 300));
	printf("Process %d with ticket %d using resource...\n", thread, tickets[thread]);
	resource = 0;
}

void thread_body(int thread) {
	lock(thread);
	use_resource(thread);
	unlock(thread);
}

int main() {
	srand(time(NULL));
	vector<thread> threads(THREAD_COUNT);
	for (int i = 0; i < THREAD_COUNT; ++i) {
		threads[i] = thread(thread_body, i);
	}
	for (auto &thread : threads) {
		if (thread.joinable()) {
			thread.join();
		}
	}
	printf("All threads finished\n");
	system("pause");
	return 0;
}
