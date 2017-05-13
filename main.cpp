#include <iostream>
#include <stdlib.h>
#include <math.h>
#include "wav-utils.h"

using namespace std;

int main() {
    WavFile * wavFile = getWavFile("/home/tuyenlv/Desktop/1.wav");
    dft(10000, wavFile->data->sampleChannel[0], wavFile->realX[0], wavFile->imaginX[0]);
    return 0;
}
