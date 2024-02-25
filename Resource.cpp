#include <chrono>
#include <iostream>
#include <vector>
#include <random>

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

    auto start = high_resolution_clock::now();

    randomVector(v1, size);
    randomVector(v2, size);

    for (int i = 0; i < size; ++i) {
        v3.push_back(v1[i] + v2[i]);
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);

    cout << "Time taken by function: " << duration.count() << " microseconds" << endl;

    return 0;
}
