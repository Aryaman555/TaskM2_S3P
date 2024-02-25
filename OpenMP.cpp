#include <chrono>
#include <cstdlib>
#include <iostream>
#include <ctime>
#include <omp.h>

using namespace std::chrono;
using namespace std;

void randomVector(int vector[], int size) {
    for (int i = 0; i < size; i++) {
        vector[i] = rand() % 100;
    }
}

int main() {
    unsigned long size = 100000000;
    srand(time(0));
    int *v1, *v2, *v3;
    v1 = (int *)malloc(size * sizeof(int));
    v2 = (int *)malloc(size * sizeof(int));
    v3 = (int *)malloc(size * sizeof(int));
    randomVector(v1, size);
    randomVector(v2, size);

    auto start = high_resolution_clock::now();

    int total = 0; // Shared variable to store the total sum
    int chunk_size = 1000; // Chunk size for scheduling

    #pragma omp parallel default(none) shared(v1, v2, v3, size, chunk_size) private(total)
    {
        // Compute the vector addition with specified chunk size for scheduling
        #pragma omp for schedule(static, chunk_size)
        for (int i = 0; i < size; i++) {
            v3[i] = v1[i] + v2[i];
        }

        // Compute the total sum using reduction clause
        #pragma omp for reduction(+:total)
        for (int i = 0; i < size; i++) {
            total += v3[i];
        }
    }

    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken by OpenMP version: " << duration.count() << " microseconds" << endl;

    cout << "Total sum: " << total << endl;

    free(v1);
    free(v2);
    free(v3);

    return 0;
}
