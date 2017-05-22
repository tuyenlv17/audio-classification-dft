#include <complex>
#include <iostream>
using namespace std;
typedef std::complex<double> Complex;

template <typename T>
void arr2Comp(int N, T * arr,Complex * x) {
    for(int i = 0; i < N; i++) {
        x[i].real() = arr[i];
        x[i].imag() = 0;
    }
}

void dft(int numSamples, Complex * x, Complex * X, bool isInv) {
    int co = 1;
    if(isInv) {
        co = numSamples;
    }
    for(int i = 0; i < numSamples; i++) {
        double fundFreq = 2 * M_PI * i / numSamples;
        X[i].real() = X[i].imag() = 0;
        for(int j = 0; j < numSamples; j++) {
            Complex expCom = std::polar(1.0, -fundFreq * j);
            X[i] += x[j] * expCom;
        }
        X[i] /= numSamples;
    }
}


template <typename T>
void dct(int numSamples, T * x, double * X) {
    for(int i = 0; i < numSamples; i++) {
        double ci = sqrt((double)(i == 0 ? 1 : 2) / numSamples);
        double fundFreq = M_PI * i / numSamples;
        X[i] = 0;
        for(int j = 0; j < numSamples; j++) {
            X[i] += x[j] * cos(fundFreq * (j + 0.5));
        }
        X[i] *= ci;
    }
}

//Cooley–Tukey FFT
void fft(int N, Complex * x) {
    if(N <= 1) {
        return;
    }
    Complex * segment[2];//even and odd index sets
    segment[0] = new Complex[N / 2];
    segment[1] = new Complex[N / 2];
    for(int i = 0; i < N; i++) {
        segment[i % 2][i / 2] = x[i];
    }
    fft(N / 2, segment[0]);//even
    fft(N / 2, segment[1]);//old
    for(int i = 0; i < N / 2; i++) {
        Complex tp = std::polar(1.0, -2 * M_PI * i / N) * segment[1][i];
        x[i] = segment[0][i] + tp;
        x[i + N / 2] = segment[0][i] - tp;
    }
    delete[] segment[0];
    delete[] segment[1];
}

template <typename T>
void fct(int numSamples, T * x, double * X) {
    Complex * xComp = new Complex[numSamples * 2];
    arr2Comp(numSamples, x, xComp);
    for(int i = numSamples; i < 2 * numSamples; i++) {
        xComp[i] = xComp[2 * numSamples - i - 1];
    }
    fft(2 * numSamples, xComp);
    for(int i = 0; i < numSamples; i++) {
        xComp[i] *= std::polar(1.0, -M_PI * i / 2 / numSamples);
        X[i] = real(xComp[i]);
    }
    delete[] xComp;
}
