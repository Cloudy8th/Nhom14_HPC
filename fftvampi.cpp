// Nhan hai ma tran voi fft va mpi 

#define _USE_MATH_DEFINES 
#include <iostream> 
#include <complex> 
#include <fftw3.h> 
#include <mpi.h> 
#include <iomanip> 
#include <cstdlib> // Để sử dụng hàm rand() 

void fft(fftw_complex* x, int n, int inv) {
    fftw_plan plan = fftw_plan_dft_1d(n, x, x, inv, FFTW_ESTIMATE);
    fftw_execute(plan);
    fftw_destroy_plan(plan);
}

void setRandomValues(fftw_complex* matrix, int size) {
    for (int i = 0; i < size; i++) {
        matrix[i][0] = rand() % 9 + 1; // Giá trị ngẫu nhiên từ 1 đến 9 
        matrix[i][1] = 0;
    }
}

int main(int argc, char** argv) {
    // Khởi tạo MPI 
    MPI_Init(&argc, &argv);
    // Lấy rank và size của quá trình 
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    // Đọc kích thước ma trận từ bàn phím 
    int n;
    if (rank == 0) {
        std::cout << "Enter the size of the square matrix: ";
        std::cin >> n;
    }

    // Broadcast kích thước ma trận cho tất cả các quá trình 
    MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);

    clock_t start_time, end_time;
    start_time = clock();

    // Tạo các ma trận A, B và C 
    fftw_complex* A = fftw_alloc_complex(n * n);
    fftw_complex* B = fftw_alloc_complex(n * n);
    fftw_complex* C = fftw_alloc_complex(n * n);

    // Khởi tạo giá trị ngẫu nhiên cho ma trận A và B 
    if (rank == 0) {
        setRandomValues(A, n * n);
        setRandomValues(B, n * n);
    }

    // Broadcast ma trận A và B cho tất cả các quá trình 
    MPI_Bcast(A, n * n * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(B, n * n * 2, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // Tính FFT của ma trận A và B 
    fft(A, n * n, FFTW_FORWARD);
    fft(B, n * n, FFTW_FORWARD);

    // Nhân ma trận A và B 
    for (int i = 0; i < n * n; i++) {
        C[i][0] = A[i][0] * B[i][0] - A[i][1] * B[i][1];
        C[i][1] = A[i][0] * B[i][1] + A[i][1] * B[i][0];
    }

    // Tính FFT ngược của ma trận C 
    fft(C, n * n, FFTW_BACKWARD);

    // Chia từng phần tử của ma trận C cho n * n để lấy kết quả nhân thực 
    for (int i = 0; i < n * n; i++) {
        C[i][0] /= (n * n);
        C[i][1] /= (n * n);
    }

    end_time = clock();

    // In kết quả (chỉ quá trình rank 0) 
    if (rank == 0) {
        double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
        std::cout << "Time taken: " << std::fixed << std::setprecision(10) <<
            time_taken;
    }

    // Giải phóng bộ nhớ 
    fftw_free(A);
    fftw_free(B);
    fftw_free(C);

    // Kết thúc MPI 
    MPI_Finalize();

    return 0;
}