// Nhan ma tran tuan tu 
#include <iostream> 
#include <cstdlib> 
#include <ctime> 
using namespace std;
void random_matrix(int size, int**& matrix) {
    matrix = new int* [size];
    for (int i = 0; i < size; ++i)
        matrix[i] = new int[size];
    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j)
            matrix[i][j] = rand() % 9 + 1;
}

void display(int size, int** matrix) {
    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j)
            cout << matrix[i][j] << " ";
        cout << endl;
    }
}

int** multiply(int** m1, int** m2, int size) {
    int** c = new int* [size];
    for (int i = 0; i < size; ++i)
        c[i] = new int[size];

    for (int i = 0; i < size; ++i)
        for (int j = 0; j < size; ++j) {
            int t = 0;
            for (int k = 0; k < size; ++k)
                t += m1[i][k] * m2[k][j];
            c[i][j] = t;
        }
    return c;
}

double multiply_and_measure_time(int** m1, int** m2, int size) {
    clock_t start_time, end_time;
    start_time = clock();
    int** c = multiply(m1, m2, size);
    end_time = clock();

    double time_taken = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;

    // Giải phóng bộ nhớ 
    for (int i = 0; i < size; ++i)
        delete[] c[i];
    delete[] c;

    return time_taken;
}

int main() {
    int size;  // Kích thước ma trận vuông 
    int** a, ** b;

    cout << "Nhap kich thuoc ma tran vuong: ";
    cin >> size;

    // Khởi tạo và set giá trị ngẫu nhiên cho ma trận A và B 
    random_matrix(size, a);
    random_matrix(size, b);

    // Tính thời gian nhân hai ma trận và in ra kết quả 
    double time_taken = multiply_and_measure_time(a, b, size);
    cout << "Time taken: " << time_taken << endl;

    // Giải phóng bộ nhớ 
    for (int i = 0; i < size; ++i)
        delete[] a[i];
    delete[] a;

    for (int i = 0; i < size; ++i)
        delete[] b[i];
    delete[] b;

    return 0;
}