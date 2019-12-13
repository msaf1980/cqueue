#include <assert.h>
#include <cstdlib>
#include <thread>
#include <vector>

#include "benchmark/benchmark.hpp"
#include <ringqueue.h>

using namespace std;
using namespace benchmark;

#define TEST_COUNT 1000

class QueueRingPushPopBench : public Benchmark {
  public:
	BENCH_CONSTRUCT(QueueRingPushPopBench);

	uint32_t count = 100000;

	int64_t inline bench() {

		int64_t start = Now();

		ringqueue_t queue = ringqueue_new(count);

		for (uint32_t i = 0; i < count; i++) {
			assert(ringqueue_push(queue, &start) == 0);
		}
		for (uint32_t i = 0; i < count; i++) {
			assert(ringqueue_popfront(queue) != NULL);
		}
		ringqueue_free(queue);
		free(queue);

		int64_t duration = Now() - start;
		return duration;
	}
};

void reader(ringqueue_t queue, int *running) {
	while(1) {

		if (*running == 0)
			break;
	};
}

class MultiQueueRingPushPopBench : public Benchmark {
  public:
	BENCH_CONSTRUCT(MultiQueueRingPushPopBench);

	uint32_t count = 100000;

	int64_t inline bench() {
		int running = 1;
		size_t i;
		vector<thread> rthreads;
		rthreads.reserve(this->threads);

		int64_t start = Now();

		ringqueue_t queue = ringqueue_new(count);

		for (i = 0; i < this->threads; i++) {
			rthreads.push_back(move(thread(reader, queue, &running)));
		}

		for (i = 0; i < count; i++) {
			assert(ringqueue_push(queue, &start) == 0);
		}
		for (i = 0; i < count; i++) {
			assert(ringqueue_popfront(queue) != NULL);
		}
		running = 0;
		for (i = 0; i < this->threads; i++) {
			rthreads[i].join();
		}
		ringqueue_free(queue);
		free(queue);

		int64_t duration = Now() - start;
		return duration;
	}
};

int main() {
	BenchmarkStdoutReporter r;

	{
		srand(0);
		QueueRingPushPopBench b("QueueRing (100000)", "PushPop", &r);
		b.set_skip_diviation(300.0l);
		b.run(TEST_COUNT, 1);
	}
	for (size_t i = 1; i < std::thread::hardware_concurrency(); i++) {
		srand(0);
		MultiQueueRingPushPopBench b("MultiQueueRing (100000)", "PushPop", &r);
		b.threads = i;
		b.set_skip_diviation(300.0l);
		b.run(TEST_COUNT, 1);
	}
	return 0;
}
