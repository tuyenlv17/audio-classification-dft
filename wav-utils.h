#include <stdio.h>
#include <string.h>
#include <iostream>
#include <climits>
#include "transforms.h"
using namespace std;

struct WavHeader {
    char chunkId[5];
    unsigned int chunkSize;
    char format[5];
    char subChunk1Id[5];
    unsigned int subChunk1Size;
    unsigned short int audioFormat;
    unsigned short int numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short int blockAlign;
    unsigned short int bitsPerSample;
    char subChunk2Id[5];//data chunk
    unsigned int subChunk2Size;
} headerTemplate;

struct FourierComplex {
    double * real;
    double * imag;
};

struct WavData {
    short int * sampleChannel[2];
};

struct WavFile {
    unsigned int numSamples;
    WavHeader * header;
    WavData * data;
    int * features;
    double meanEnergy;
    double zcRate;
    double silientRate;
    double bandwidth;
    double specCentroid;
    double * dct;
};

void calMeanEnergy(WavFile * wavFile) {
    wavFile->meanEnergy = 0;
    double scaleFactor = 50000;
    for(int i = 0; i < wavFile->numSamples; i++) {
        int amplitude = wavFile->data->sampleChannel[0][i];
        wavFile->meanEnergy += amplitude * amplitude / scaleFactor;
    }
    wavFile->meanEnergy /= wavFile->numSamples;
}

template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

void calZcRate(WavFile * wavFile) {
    wavFile->zcRate = 0;
//    double scaleFactor = 100;
    for(int i = 1; i < wavFile->numSamples; i++) {
        int lastAmplitude = wavFile->data->sampleChannel[0][i - 1];
        int amplitude = wavFile->data->sampleChannel[0][i];
        wavFile->zcRate += abs(sgn(amplitude) - sgn(lastAmplitude));
    }
    wavFile->zcRate /= wavFile->numSamples * 2;
    wavFile->zcRate *= 100;
}

void calSilientRate(WavFile * wavFile) {
    int amplThred = 400;
    int timeThred = 0.01 * wavFile->header->sampleRate;
    wavFile->silientRate = 0;
    int cnt = 0;
    for(int i = 0; i < wavFile->numSamples; i++) {
        int amplitude = abs(wavFile->data->sampleChannel[0][i]);
        if(amplitude > amplThred) {
            if(cnt >= timeThred) {
                wavFile->silientRate += cnt;
            }
            cnt = 0;
        } else {
            cnt++;
        }
    }
    wavFile->silientRate += cnt;
    wavFile->silientRate /= wavFile->numSamples;
    wavFile->silientRate *= 100;
}

void calSpecCentroid(WavFile * wavFile) {
    wavFile->specCentroid = 0;
    double scaleFactor = 1;
    double sW = 0;
    for(int i = 0; i < wavFile->numSamples; i++) {
        double magnitude = abs(wavFile->dct[i]);
        wavFile->specCentroid += magnitude * i;
        sW += magnitude;
    }
    wavFile->specCentroid /= sW;
}

void calBandwidth(WavFile * wavFile) {

}


WavFile * getWavFile(const char* filename) {
    FILE *fileIn = fopen(filename, "rb");
    WavFile * wavFile = new WavFile;
    WavHeader header;
    WavData data;
    wavFile->header = &header;
    wavFile->data = &data;
    //get header
    if (fileIn) {
        char * ignoredChunkData, * idTmp;
        fread(header.chunkId, 1, sizeof(header.chunkId) - 1, fileIn);
        header.chunkId[4] = 0;
        if (strcmp(header.chunkId, "RIFF") != 0) {
            fclose(fileIn);
            return NULL;
        }
        fread(&header.chunkSize, 1, sizeof(header.chunkSize), fileIn);
        fread(header.format, 1, 4, fileIn);
        header.format[4] = 0;
        if (strcmp(header.format, "WAVE") != 0) {
            fclose(fileIn);
            return NULL;
        }
        fread(header.subChunk1Id, 1, sizeof(header.subChunk1Id) - 1, fileIn);
        header.subChunk1Id[4] = 0;
        fread(&header.subChunk1Size, 1, sizeof(header.subChunk1Size), fileIn);
        fread(&header.audioFormat, 1, sizeof(header.audioFormat), fileIn);
        fread(&header.numChannels, 1, sizeof(header.numChannels), fileIn);
        fread(&header.sampleRate, 1, sizeof(header.sampleRate), fileIn);
        fread(&header.byteRate, 1, sizeof(header.byteRate), fileIn);
        fread(&header.blockAlign, 1, sizeof(header.blockAlign), fileIn);
        fread(&header.bitsPerSample, 1, sizeof(header.bitsPerSample), fileIn);
        do {
            fread(header.subChunk2Id, 1, sizeof(header.subChunk2Id) - 1, fileIn);
            fread(&header.subChunk2Size, 1, sizeof(header.subChunk2Size), fileIn);
//            cout << header.subChunk2Id << " " << header.subChunk2Size << endl;
            header.subChunk2Id[4] = 0;
            if (strcmp(header.subChunk2Id, "data") != 0) {
                fseek(fileIn, header.subChunk2Size, SEEK_CUR);
                continue;
            } else {
                break;
            }
        } while(true);
        //allocate new memory for data
        wavFile->numSamples = header.subChunk2Size * 8 / header.bitsPerSample / header.numChannels;
        wavFile->numSamples = 1 << (int)(log(wavFile->numSamples) / log(2));
        unsigned int channelSize = header.bitsPerSample / 8;
//        cout << "Finished reading\n";
//        cout << "Header's size:    " << sizeof(header) << " bytes" << endl;
//        cout << "Chunk id:         " << header.chunkId << endl;
//        cout << "Chunk size:       " << header.chunkSize << endl;
//        cout << "format:           " << header.format << endl;
//        cout << "Sub Chunk id:     " << header.subChunk1Id << endl;
//        cout << "Sk size:          " << header.subChunk1Size << endl;
//        cout << "Audio format:     " << header.audioFormat << endl;
//        cout << "No channels:      " << header.numChannels << endl;
//        cout << "Sampling rate:    " << header.sampleRate << " Hz" << endl;
//        cout << "Byte rate:        " << header.byteRate << endl;
//        cout << "Block align:      " << header.blockAlign << endl;
//        cout << "Bit per sample:   " << header.bitsPerSample << endl;
//        cout << "Sub Chunk id 2:   " << header.subChunk2Id << endl;
//        cout << "Sk2 size:         " << header.subChunk2Size << endl;
//        cout << "Total sample:     " << wavFile->numSamples << endl;
//        cout << "Channel size:     " << channelSize << endl;

        for(int i = 0; i < header.numChannels; i++) {
            wavFile->data->sampleChannel[i] = new short int[wavFile->numSamples];
        }

        for(int i = 0; i < wavFile->numSamples; i++) {
            for(int j = 0; j < header.numChannels; j++) {
                fread(&wavFile->data->sampleChannel[j][i], 1, sizeof(wavFile->data->sampleChannel[j][i]), fileIn);
            }
        }
//        dct(wavFile->numSamples, wavFile->data->sampleChannel[0], wavFile->realX[0]);
//        idft(10000, wavFile->data->sampleChannel[0], wavFile->realX[0], wavFile->imaginX[0]);

//        Complex * x, * X;
//        x = new Complex[wavFile->numSamples];
//        X = new Complex[wavFile->numSamples];
//        comp2Arr(wavFile->numSamples, wavFile->data->sampleChannel[0], x);
////        dft(wavFile->numSamples, x, X, false);
////        dft(testNum, x, X, false);
//        fft(testNum, x);
        wavFile->dct = new double[wavFile->numSamples];
        fct(wavFile->numSamples, data.sampleChannel[0], wavFile->dct);
        calMeanEnergy(wavFile);
        calSilientRate(wavFile);
        calZcRate(wavFile);
        calSpecCentroid(wavFile);
        calBandwidth(wavFile);
//        freopen("out.txt", "w", stdout);
//        for(int i = 0; i < wavFile->numSamples; i++) {
////            printf("%5d %.5lf %.5lf\n", i, X[i].real(), X[i].imag());
////            printf("%.5lf\n", X[i]);
////            cout << data.sampleChannel[0][i] << endl;
//            printf("%.5lf %.5lf\n", );
//        }
            printf("%.5lf %.5lf %.5lf %.5lf\n", wavFile->meanEnergy, wavFile->zcRate, wavFile->silientRate, wavFile->specCentroid);
    } else {
        fclose(fileIn);
        return NULL;
    }
    fclose(fileIn);
    return wavFile;
}
