#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <string>
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

int main() {
    WavFile files[2][20];
    int totalType = 2;
    int totalPer = 5;
    int cnt = 0;
    string type[] = {"musics", "dialogue"};
    string basePath = "/home/tuyenlv/data/learning/school/sem8/multimedia-database-system/project/audio-files";
    for(int t = 0; t < totalType; t++) {
        for(int i = 1; i <= totalPer; i++) {
//            printf("\rTrainning...%.2f%%", (float)cnt * 100 / totalType / totalPer);
//            fflush(stdout);
            string file = basePath + "/" + type[t] + "/wav/short/" + int2Str(i) + ".wav";
            files[t][i - 1] = *getWavFile(file.c_str());
            cnt++;
        }
        cout << endl;
    }
//    printf("\rTrainning...%.2f%%", (float)cnt * 100 / totalType / totalPer);
//    fflush(stdout);
    return 0;
}
