#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

struct WavHeader {
    char chunkId[5];
    unsigned int chunkSize;
    char format[5];
    char subchunk1Id[5];
    unsigned int subchunk1Size;
    unsigned short int audioFormat;
    unsigned short int numChannels;
    unsigned int sampleRate;
    unsigned int byteRate;
    unsigned short int blockAlign;
    unsigned short int bitsPerSample;
    char subchunk2Id[5];//data chunk
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

bool readWavHeader(WavHeader &header, FILE * fileIn) {

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
        fread(header.subchunk1Id, 1, sizeof(header.subchunk1Id) - 1, fileIn);
        header.subchunk1Id[4] = 0;
        fread(&header.subchunk1Size, 1, sizeof(header.subchunk1Size), fileIn);
        fread(&header.audioFormat, 1, sizeof(header.audioFormat), fileIn);
        fread(&header.numChannels, 1, sizeof(header.numChannels), fileIn);
        fread(&header.sampleRate, 1, sizeof(header.sampleRate), fileIn);
        fread(&header.byteRate, 1, sizeof(header.byteRate), fileIn);
        fread(&header.blockAlign, 1, sizeof(header.blockAlign), fileIn);
        fread(&header.bitsPerSample, 1, sizeof(header.bitsPerSample), fileIn);
        do {
            fread(header.subchunk2Id, 1, sizeof(header.subchunk2Id) - 1, fileIn);
            fread(&header.subchunk2Size, 1, sizeof(header.subchunk2Size), fileIn);
//            cout << header.subchunk2Id << " " << header.subchunk2Size << endl;
            header.subchunk2Id[4] = 0;
            if (strcmp(header.subchunk2Id, "data") != 0) {
                fseek(fileIn, header.subchunk2Size, SEEK_CUR);
                continue;
            } else {
                break;
            }
        } while(true);
        cout << "Finished reading\n";
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
        fclose(fileIn);
        return NULL;
    }
    fclose(fileIn);
    return wavFile;
}

int main() {
    getWavFile("/home/tuyenlv/Desktop/ding.wav");
    return 0;
}
