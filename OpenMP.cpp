#include <chrono>
#include <iostream>
#include <vector>
#include <random>
#include <omp.h>

using namespace std;
using namespace std::chrono;

void randomVector(vector<int>& vec, int size) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 99);

    for (int i = 0; i < size; ++i) {
        vec.push_back(dis(gen));
    }
}

int main() {
    unsigned long size = 100000000;
    vector<int> v1, v2, v3;
    v1.reserve(size);
    v2.reserve(size);
    v3.resize(size);

    auto start = high_resolution_clock::now();

    randomVector(v1, size);
    randomVector(v2, size);

    #pragma omp parallel for
    for (int i = 0; i < size; ++i) {
        v3[i] = v1[i] + v2[i];
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by OpenMP parallel program: " << duration.count() << " microseconds" << endl;

    return 0;
}
