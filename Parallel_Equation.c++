#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>

#define NX 100  // Number of spatial points
#define NT 1000 // Number of time steps
#define ALPHA 0.01
#define DX 0.01
#define DT 0.0001

void initialize(std::vector<double>& u) {
    std::fill(u.begin(), u.end(), 0.0);
    u[NX / 2] = 100.0; // Initial condition: a delta function in the middle
}

void update(const std::vector<double>& u, std::vector<double>& u_new) {
    #pragma omp parallel for
    for (int i = 1; i < NX - 1; ++i) {
        u_new[i] = u[i] + ALPHA * DT / (DX * DX) * (u[i + 1] - 2 * u[i] + u[i - 1]);
    }
}

int main() {
    std::vector<double> u(NX), u_new(NX);

    initialize(u);

    double start_time = omp_get_wtime();

    for (int n = 0; n < NT; ++n) {
        update(u, u_new);

        // Swap vectors
        std::swap(u, u_new);
    }

    double end_time = omp_get_wtime();

    // Output the final temperature distribution
    for (double temp : u) {
        std::cout << temp << "\n";
    }

    std::cout << "Execution Time: " << end_time - start_time << " seconds\n";

    return 0;
}
