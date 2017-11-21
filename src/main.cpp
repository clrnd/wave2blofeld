#include <numeric>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include "AudioFile.h"
#include "MidiFile.h"

using uchar = unsigned char;

int main(void){
    AudioFile<double> audioFile;
    audioFile.load("../Untitled.wav");
    audioFile.printSummary();

    int numSamples = audioFile.getNumSamplesPerChannel();
    std::cout << "numSamples: " << numSamples << std::endl;

    // 21 bit values => 1048575 to -1048575
    int max = 1048575;
    std::array<int, 64*128> samples;

    for (int i = 0; i < numSamples; i+=2) {
        // between -1 and 1
        double currentSample = audioFile.samples[0][i];

        // normalizing
        samples[i/2] = currentSample*max;
    }

    MidiFile mf;
    //mf.addTrack(1);
    mf.setTicksPerQuarterNote(100);

    std::string name = "Example Whatev";

    for (int wave = 0; wave < 64; ++wave){

        std::vector<uchar> mm(410);

        mm[0] = 0xf0; // SysEx
        mm[1] = 0x3e; // Waldorf ID
        mm[2] = 0x13; // Blofeld ID
        mm[3] = 0x00; // Device ID
        mm[4] = 0x12; // Wavetable Dump
        mm[5] = 0x51; // Wavetable Number
        mm[6] = wave & 0x7f; // Wave Number
        mm[7] = 0x00; // Format

        for (int i = 0; i < 128; ++i){
            mm[8 + 3*i    ] = (samples[i + wave*128] >> 14) & 0x7f;
            mm[8 + 3*i + 1] = (samples[i + wave*128] >>  7) & 0x7f;
            mm[8 + 3*i + 2] = (samples[i + wave*128]      ) & 0x7f;
        }

        for (int i = 0; i < 14; ++i){
            mm[392+i] = name[i] & 0x7f;
        }

        mm[406] = 0x0; // Reserved
        mm[407] = 0x0; // Reserved
        int checksum = std::accumulate(
            mm.begin()+7, mm.end(), 0);
        mm[408] = checksum & 0x7f;
        //mm[408] = 0x7f;

        mm[409] = 0xf7; // End

        mf.addEvent(0, 0, mm);
    }

    //vector<uchar> clear = {0xff, 0x2f, 0x00};
    //mf.addEvent(1, 200*64, clear);

    //mf.writeHex(std::cout);
    mf.write("output.mid");

    return 0;
}
