#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <unordered_map>
#include <cassert>
#include "concurrent-hash-map.hpp"
using namespace std;

static const int OPS_PER_THREAD = 100000;

double get_time() {
    return chrono::duration<double>(
        chrono::high_resolution_clock::now().time_since_epoch()
    ).count();
}

int main(int argc, char* argv[]) {
    int num_threads = 4; // default

    if (argc > 1) {
        num_threads = atoi(argv[1]);
    }

    ConcurrentHashMap<int, int> chm(256);

    vector<thread> threads;

    double start = get_time();

    for (int t = 0; t < num_threads; t++) {
        threads.emplace_back([&chm, t]() {
            std::mt19937 rng(t + 1);
            std::uniform_int_distribution<int> dist(1, 200);

            for (int i = 0; i < OPS_PER_THREAD; i++) {
                int key = dist(rng);
                int op = key % 10;

                if (op < 5) {
                    // 50% get
                    chm.get(key);
                } else if (op < 8) {
                    // 30% put
                    chm.put(key, key * 10);
                } else {
                    // 20% remove
                    chm.remove(key);
                }
            }
        });
    }

    for (auto& th : threads) {
        th.join();
    }

    double end = get_time();

    int total_ops = num_threads * OPS_PER_THREAD;
    double time_taken = end - start;

    cout << "Threads: " << num_threads << endl;
    cout << "Total ops: " << total_ops << endl;
    cout << "Time: " << time_taken << " seconds" << endl;
    cout << "Throughput: " << (total_ops / time_taken) << " ops/sec" << endl;

    return 0;
}