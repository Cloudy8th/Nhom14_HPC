//Thuat toan fft 
#include <complex> 
#include <iostream> 
#include <valarray> 

const double PI = 3.141592653589793238460;

typedef std::complex<double> Complex;
typedef std::valarray<Complex> CArray;

// Hàm thực hiện FFT 
void fft(CArray& x)
{
    const size_t N = x.size();
    if (N <= 1) return;

    // Chia dãy số thành 2 phần bằng nhau 
    CArray even = x[std::slice(0, N / 2, 2)];
    CArray  odd = x[std::slice(1, N / 2, 2)];

    // Đệ quy FFT cho cả 2 phần 
    fft(even);
    fft(odd);

    // Tính toán các hệ số biến đổi 
    for (size_t k = 0; k < N / 2; ++k)
    {
        Complex t = std::polar(1.0, -2 * PI * k / N) * odd[k];
        x[k] = even[k] + t;
        x[k + N / 2] = even[k] - t;
    }
}

// Hàm thực hiện IFFT (inverse FFT) 
void ifft(CArray& x)
{
    // Đảo ngược dãy số và áp dụng FFT 
    x = x.apply(std::conj);
    fft(x);
    x = x.apply(std::conj);

    // Chia tất cả các phần tử cho N 
    x /= x.size();
}

int main()
{
    // Số lượng phần tử trong dãy số 
    const size_t N = 4;

    // Dãy số ban đầu 
    Complex test[N] = { 1.0, 1.0, 1.0, 1.0 };
    CArray data(test, N);

    // Thực hiện FFT và in kết quả ra màn hình 
    fft(data);
    std::cout << "FFT = " << std::endl;
    for (size_t i = 0; i < N; ++i)
    {
        std::cout << data[i] << std::endl;
    }

    // Thực hiện IFFT và in kết quả ra màn hình 
    ifft(data);
    std::cout << "IFFT = " << std::endl;
    for (size_t i = 0; i < N; ++i)
    {
        std::cout << data[i] << std::endl;
    }

    return 0;
}