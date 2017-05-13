void dft(int numSamples, short int * x, double * realX, double * imaginX) {
    for(int i = 0; i < numSamples; i++) {
//        printf("\r%d%%", i * 100 / numSamples);
//        fflush(stdout);
        double fundFreq = 2 * M_PI * i / numSamples;
        realX[i] = imaginX[i] = 0;
        for(int j = 0; j < numSamples; j++) {
            realX[i] += x[j] * cos(fundFreq * j);
            imaginX[i] += x[j] * sin(fundFreq * j);
        }
    }
//    printf("\rFinished!");
//    fflush(stdout);
}

void fft(int numSamples, short int * x, float * realX, float * imaginX) {

}

