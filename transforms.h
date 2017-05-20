void dft(int numSamples, short int * x, double * realX, double * imaginX) {
    for(int i = 0; i < numSamples; i++) {
//        printf("\r%d%%", i * 100 / numSamples);
//        fflush(stdout);
        double fundFreq = 2 * M_PI * i / numSamples;
        realX[i] = imaginX[i] = 0;
        for(int j = 0; j < numSamples; j++) {
            realX[i] += x[j] * cos(fundFreq * j);
            imaginX[i] -= x[j] * sin(fundFreq * j);
        }
    }
//    printf("\rFinished!");
//    fflush(stdout);
}

void idft(int numSamples, short double * x, double * realX, double * imaginX) {
    for(int i = 0; i < numSamples; i++) {
//        printf("\r%d%%", i * 100 / numSamples);
//        fflush(stdout);
        double fundFreq = 2 * M_PI * i / numSamples;
        double rx = 0, ix = 0;
        for(int j = 0; j < numSamples; j++) {
            rx += realX[j] * cos(fundFreq * j) - imaginX[j] * sin(fundFreq * j);
            ix += realX[j] * sin(fundFreq * j) + imaginX[j] * cos(fundFreq * j);
        }
//        printf("%.5lf %.5lf\n", rx / numSamples, ix / numSamples);
    }
//    printf("\rFinished!");
//    fflush(stdout);
}

void fft(int numSamples, short int * x, float * realX, float * imaginX) {

}


void dct(int numSamples, short int * x, double * X) {
    for(int i = 0; i < numSamples; i++) {

    }
}
