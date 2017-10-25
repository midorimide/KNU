#include <thread>
#include <vector>
#include <algorithm>

#define THREAD_COUNT 100
#define MAX_TICKET 50

std::vector<int> tickets(THREAD_COUNT, 0);
std::vector<bool> choosing(THREAD_COUNT, false);
int resource = 0;

void lock(int thread) {
	do {
		tickets[thread] = 0;
		choosing[thread] = true;
		tickets[thread] = ++(*max_element(tickets.begin(), tickets.end()));
		choosing[thread] = false;
	} while (tickets[thread] >= MAX_TICKET);
	for (int other = 0; other < THREAD_COUNT; ++other) {
		while (choosing[other]);
		while (tickets[other] &&
			(tickets[other] < tickets[thread] ||
			(tickets[other] == tickets[thread] && other < thread)));
	}
}

void unlock(int thread) {
	tickets[thread] = 0;
}

void use_resource(int thread) {
	if (resource != 0) {
		printf("Resource was acquired by process %d (ticket %d), but is still in-use by process %d (ticket %d)!\n",
			thread, tickets[thread], resource, tickets[resource]);
	}
	resource = thread;
	printf("Process %d with ticket %d using resource...\n", thread, tickets[thread]);
	int i = 0;
	while (++i < INT_MAX / 1000);
	resource = 0;
}

void thread_body(int thr) {
	int thread = thr;
	lock(thread);
	use_resource(thread);
	unlock(thread);
}

int main() {
	std::vector<std::thread> threads(THREAD_COUNT);
	for (int i = 0; i < THREAD_COUNT; ++i) {
		threads[i] = std::thread(thread_body, i);
	}
	for (int i = 0; i < THREAD_COUNT; ++i) {
		if (threads[i].joinable()) {
			threads[i].join();
		}
	}
	printf("All threads finish\n");
	system("pause");
	return 0;
}
