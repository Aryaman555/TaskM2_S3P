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

    // Fill vectors with random values
    randomVector(v1, size);
    randomVector(v2, size);

    // Parallel addition of vectors v1 and v2
    int total = 0;
    #pragma omp parallel default(none) shared(v1, v2, v3, size) reduction(+:total)
    {
        #pragma omp for
        for (int i = 0; i < size; ++i) {
            v3[i] = v1[i] + v2[i];
            total += v3[i];
        }
    }

    cout << "Total sum using reduction: " << total << endl;

    // Alternative version using a private variable and a critical section
    int total_private = 0;
    #pragma omp parallel for shared(v3) private(total_private)
    for (int i = 0; i < size; ++i) {
        total_private += v3[i];
    }

    total = 0;
    #pragma omp critical
    total += total_private;

    cout << "Total sum using critical section: " << total << endl;

    return 0;
}
