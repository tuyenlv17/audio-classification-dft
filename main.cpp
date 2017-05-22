#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <fstream>
#include <iomanip>
#include "wav-utils.h"
using namespace std;

template <typename T>
string int2Str(T n) {
    if(n == 0) {
        return "0";
    }
    string sgn = "";
    if(n < 0) {
        sgn = "-";
    }
    string res = "";
    while(n > 0) {
        res = (char)(n % 10 + '0') + res;
        n /= 10;
    }
    return sgn + res;
}

void exportModel(int totalType, int totalPer,WavFile files[][20]) {
    ofstream model;
    model.open ("model.txt");
    model << totalType << " " << totalPer << endl;
    for(int t = 0; t < totalType; t++) {
        for(int i = 1; i <= totalPer; i++) {
                model << files[t][i].meanEnergy << " " << files[t][i].silientRate << " " << files[t][i].zcRate << " " << files[t][i].specCentroid << endl;
        }
    }
    model.close();
}

void importModel(int totalType, int totalPer,WavFile files[][20]) {
    ifstream model;
    model.open ("model.txt");
    model >> totalType >> totalPer;
    for(int t = 0; t < totalType; t++) {
        for(int i = 1; i <= totalPer; i++) {
            model >> files[t][i].meanEnergy >> files[t][i].silientRate >> files[t][i].zcRate >> files[t][i].specCentroid;
        }
    }
    model.close();
}

double pow2(double a) {
    return a * a;
}

double l1Norm(WavFile f1, WavFile f2) {
    return abs(f1.silientRate -f2.silientRate) + abs(f1.meanEnergy - f2.meanEnergy)
            + abs(f1.specCentroid - f2.specCentroid) + abs(f1.zcRate - f1.zcRate);
}

double l2Norm(WavFile f1, WavFile f2) {
    return sqrt(pow2(f1.silientRate -f2.silientRate) + pow2(f1.meanEnergy - f2.meanEnergy)
            + pow2(f1.specCentroid - f2.specCentroid) + pow2(f1.zcRate - f1.zcRate));
}

bool fileExist(const char *fileName){
    std::ifstream infile(fileName);
    return infile.good();
}

void classify(int totalType, int totalPer, WavFile files[][20], string file) {
    WavFile * curr = getWavFile(file.c_str());
    double dis[2][20];
    double minDis[2], maxDis[2], meanDis[2];
    meanDis[0] = meanDis[1] = minDis[0] = minDis[1] = maxDis[0] = maxDis[1] = 0;
    for(int i = 0; i < totalType; i++) {
        for(int j = 0; j < totalPer; j++) {
            dis[i][j] = l2Norm(*curr, files[i][j]);
            minDis[i] = min(minDis[i], dis[i][j]);
            maxDis[i] = max(maxDis[i], dis[i][j]);
            meanDis[i] += dis[i][j] / totalPer;
        }
    }
}

int main() {
    WavFile files[2][20];
    int totalType = 2;
    int totalPer = 20;
    int cnt = 0;
    string type[] = {"musics", "dialogue"};
    string basePath = "/home/tuyenlv/data/learning/school/sem8/multimedia-database-system/project/audio-files";
    for(int t = 0; t < totalType; t++) {
        for(int i = 1; i <= totalPer; i++) {
            printf("\rTrainning...%.2f%%", (float)cnt * 100 / totalType / totalPer);
            fflush(stdout);
            string file = basePath + "/" + type[t] + "/wav/short/" + int2Str(i) + ".wav";
            files[t][i - 1] = *getWavFile(file.c_str());
            cnt++;
        }
        cout << endl;
    }
    printf("\rTrainning...%.2f%%", (float)cnt * 100 / totalType / totalPer);
    fflush(stdout);


    exportModel(totalType, totalPer, files);

//    importModel(&totalType, &totalPer, files);
//
//    while(true) {
//        string file;
//        cout << "File path >>";
//        getline(cin, file);
//        if(!fileExist(file.c_str())) {
//            cout << "File not exist\n";
//            continue;
//        }
//        classify(file);
//    }
    return 0;
}
