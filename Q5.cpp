#include <iostream>
#include <thread>
#include <vector>
#include <chrono>
#include <random>
#include <mutex>

using namespace std::chrono;

const int nthreads = 1;
int64_t n = 10;
std::mutex mtx;

int sum[nthreads] = {0};

void thread_sum(int tid, std::vector<int> &a)
{
    int start = (n / nthreads) * tid;
    int end = (n / nthreads) * (tid + 1);
    if (tid == nthreads - 1)
        end += (n % nthreads);
    for (int i = start; i < end; i++)
        sum[tid] += a[i];
}

int main(int argc, char **argv)
{
    srand(time(NULL));
    int i;
    std::vector<std::thread> threads;
    std::vector<int> a;
    n = atoi(argv[1]);
    for (i = 0; i < n; i++)
    {
        a.push_back(rand() % 10);
        printf("%d ", a[i]);
    }
    printf("\n");
    for (i = 0; i < nthreads; i++)
        threads.push_back(std::thread(thread_sum, i, std::ref(a)));
    for (auto &th : threads)
        th.join();
    int total_sum = 0;
    for (i = 0; i < nthreads; i++)
        total_sum += sum[i];
    std::cout << "SUM : " << total_sum << std::endl;
}