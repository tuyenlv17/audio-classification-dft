#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct WavHeader {
    char chunkId[4];
    unsigned int chunkSize;
    char format[4];
    char subchunk1Id[4];
    unsigned int subchunk1Size;
    unsigned short int audioFormat;
    unsigned short int numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short int blockAlign;
    unsigned short int bitsPerSample;
    char subchunk2Id[4];
    unsigned int subchunk2Size;
} headerTemplate;

struct WavData {
    short int * ftChannel;//first channel, alway exist
    short int * snChannel;//second channel
};

struct WavFile {
    unsigned int numSamples;
    WavHeader * header;
    WavData * data;
    int * features;
};

WavFile * getWavFile(const char* filename) {
    FILE *fileIn = fopen(filename, "rb");
    WavFile * wavFile = new WavFile;
    WavHeader header;
    WavData data;
    wavFile->header = &header;
    wavFile->data = &data;
    //get header
    if (fileIn) {
        fread(&header, 1, sizeof(header), fileIn);
        if (strncmp(header.chunkId, "RIFF", 4 ) != 0) {
            return NULL;
        }
        cout << "Header's size:    " << sizeof(header) << " bytes" << endl;
        cout << "Chunk id:         " << header.chunkId << endl;
        cout << "Chunk size:       " << header.chunkSize << endl;
        cout << "format:           " << header.format << endl;
        cout << "Sub Chunk id:     " << header.subchunk1Id << endl;
        cout << "Sk size:          " << header.subchunk1Size << endl;
        cout << "Audio format:     " << header.audioFormat << endl;
        cout << "No channels:      " << header.numChannels << endl;
        cout << "Sampling rate:    " << header.sampleRate << " Hz" << endl;
        cout << "Byte rate:        " << header.byteRate << endl;
        cout << "Block align:      " << header.blockAlign << endl;
        cout << "Bit per sample:   " << header.bitsPerSample << endl;
        cout << "Sub Chunk id 2:   " << header.subchunk2Id << endl;
        cout << "Sk2 size:         " << header.subchunk2Size << endl;

    } else {
        return NULL;
    }
    return wavFile;
}

int main() {
    getWavFile("/home/tuyenlv/Desktop/mono.wav");
    string s;
    getline(cin, s);
    return 0;
}
