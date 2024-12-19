// Nhan ma tran voi fft 
#define _USE_MATH_DEFINES 
#include <iostream> 
#include <iomanip> 
#include <complex> 
#include <ctime> 
#include <cstdlib> 
#include <fftw3.h> 

using namespace std;

void set_random_values(complex<double>* matrix, int size) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            matrix[i * size + j] = complex<double>(rand() % 9 + 1, rand() % 9 +
                1);
        }
    }
}

void multiply(complex<double>* a, complex<double>* b, int m, int n, int p,
    complex<double>* c, fftw_plan fft_plan, fftw_plan ifft_plan) {
    fftw_execute(fft_plan); // FFT for matrix a 
    fftw_execute(fft_plan); // FFT for matrix b 

    for (int i = 0; i < n; i++) a[i] *= b[i];

    fftw_execute(ifft_plan); // Inverse FFT for result 

    for (int i = 0; i < m; i++) {
        for (int j = 0; j < p; j++) {
            c[i * p + j] = a[(i + j) % n] / static_cast<double>(n);
        }
    }
}

int main() {
    int size;
    cout << "Enter the size of the square matrix: ";
    cin >> size;

    complex<double>* A = new complex<double>[size * size];
    complex<double>* B = new complex<double>[size * size];
    complex<double>* C = new complex<double>[size * size];

    // Set random values for matrices A and B using rand function 
    for (int i = 0; i < size * size; i++) {
        A[i] = complex<double>(rand() % 9 + 1, rand() % 9 + 1);
        B[i] = complex<double>(rand() % 9 + 1, rand() % 9 + 1);
    }

    // Create FFTW plans 
    fftw_plan fft_plan = fftw_plan_dft_1d(size,
        reinterpret_cast<fftw_complex*>(A), reinterpret_cast<fftw_complex*>(A),
        FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_plan ifft_plan = fftw_plan_dft_1d(size,
        reinterpret_cast<fftw_complex*>(A), reinterpret_cast<fftw_complex*>(A),
        FFTW_BACKWARD, FFTW_ESTIMATE);

    clock_t start, end;
    // Multiply matrices A and B using FFT 
    start = clock();
    multiply(A, B, size, size, size, C, fft_plan, ifft_plan);
    end = clock();

    double time_use = static_cast<double>(end - start) / CLOCKS_PER_SEC;
    cout << setprecision(16) << fixed << "Time use: " << time_use << " seconds"
        << endl;

    // Deallocate memory and destroy FFTW plans 
    fftw_destroy_plan(fft_plan);
    fftw_destroy_plan(ifft_plan);
    delete[] A;
    delete[] B;
    delete[] C;

    return 0;
}