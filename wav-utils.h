#include <stdio.h>
#include <string.h>
#include <iostream>
#include "transforms.h"
#include "kiss_fft.h"
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
    double * realX[2];
    double * imaginX[2];
};

WavFile * getWavFile(const char* filename) {
    FILE *fileIn = fopen(filename, "rb");
    freopen("out.txt", "w", stdout);
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
//        cout << "Finished reading\n";
        cout << "Header's size:    " << sizeof(header) << " bytes" << endl;
        cout << "Chunk id:         " << header.chunkId << endl;
        cout << "Chunk size:       " << header.chunkSize << endl;
        cout << "format:           " << header.format << endl;
        cout << "Sub Chunk id:     " << header.subChunk1Id << endl;
        cout << "Sk size:          " << header.subChunk1Size << endl;
        cout << "Audio format:     " << header.audioFormat << endl;
        cout << "No channels:      " << header.numChannels << endl;
        cout << "Sampling rate:    " << header.sampleRate << " Hz" << endl;
        cout << "Byte rate:        " << header.byteRate << endl;
        cout << "Block align:      " << header.blockAlign << endl;
        cout << "Bit per sample:   " << header.bitsPerSample << endl;
        cout << "Sub Chunk id 2:   " << header.subChunk2Id << endl;
        cout << "Sk2 size:         " << header.subChunk2Size << endl;
        //allocate new memory for data
        wavFile->numSamples = header.subChunk2Size * 8 / header.bitsPerSample / header.numChannels;
        unsigned int channelSize = header.bitsPerSample / 8;
        cout << "Total sample:     " << wavFile->numSamples << endl;
        cout << "Channel size:     " << channelSize << endl;
        for(int i = 0; i < header.numChannels; i++) {
            wavFile->data->sampleChannel[i] = new short int[wavFile->numSamples];
            wavFile->realX[i] = new double[wavFile->numSamples];
            wavFile->imaginX[i] = new double[wavFile->numSamples];
        }
        kiss_fft_cpx * inData = new kiss_fft_cpx[wavFile->numSamples];
        kiss_fft_cpx * outData = new kiss_fft_cpx[wavFile->numSamples];;
        for(int i = 0; i < wavFile->numSamples; i++) {
            for(int j = 0; j < header.numChannels; j++) {
                fread(&wavFile->data->sampleChannel[j][i], 1, sizeof(wavFile->data->sampleChannel[j][i]), fileIn);
            }
            inData[i].r = wavFile->data->sampleChannel[0][i];
            inData[i].i = 0;
        }
//        kiss_fft_cfg cfg = kiss_fft_alloc(10000,false ,0,0);
//        kiss_fft( cfg , inData, outData);
//
        dct(10000, wavFile->data->sampleChannel[0], wavFile->realX[0]);
//        idft(10000, wavFile->data->sampleChannel[0], wavFile->realX[0], wavFile->imaginX[0]);
        for(int i = 0; i < 10000; i++) {
            printf("%d %.5lf\n", i, wavFile->realX[0][i]);
        }

    } else {
        fclose(fileIn);
        return NULL;
    }
    fclose(fileIn);
    return wavFile;
}
