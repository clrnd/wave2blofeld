#include <numeric>
#include <string>
#include <iostream>
#include <array>
#include <vector>
#include <cstdlib>
#include <cerrno>

#include "tclap/CmdLine.h"
#include "AudioFile.h"
#include "MidiFile.h"

using uchar = unsigned char;

struct Options {
    std::string infile;
    std::string outfile;
    bool half;
};

Options parseOptions(int argc, char* argv[]){
    TCLAP::CmdLine cmd("WAV to Blofeld SysEx", ' ', "0.1");

    TCLAP::UnlabeledValueArg<std::string>
        infile("infile", "Input WAV file.", true, "", "infile");
    cmd.add(infile);

    TCLAP::UnlabeledValueArg<std::string>
        outfile("outfile", "Output midi file.", true, "", "outfile");
    cmd.add(outfile);

    TCLAP::SwitchArg
        half("d", "double", "Use only half of the samples.", cmd, false);

    cmd.parse(argc, argv);

    return Options {
        .infile = infile.getValue(),
        .outfile = outfile.getValue(),
        .half = half.getValue(),
    };
}


int main(int argc, char* argv[]){
    Options opts = parseOptions(argc, argv);

    AudioFile<double> audioFile;

    if (!audioFile.load(opts.infile)) {
        exit(EXIT_FAILURE);
    }

    audioFile.printSummary();

    int numSamples = audioFile.getNumSamplesPerChannel();
    if (!opts.half and numSamples != 64*128) {
        std::cerr << "File's first channel must have 64*128 = "
                  << 64*128 << " samples!" << std::endl;
        exit(EXIT_FAILURE);
    } else if (opts.half and numSamples != 64*128*2) {
        std::cerr << "File's first channel must have 64*256 = "
                  << 64*128*2 << " samples!" << std::endl;
        exit(EXIT_FAILURE);
    }

    // 21 bit values => 1048575 to -1048575
    int max = 1048575;
    std::array<int, 64*128> samples;

    for (int i = 0; i < numSamples; i++) {
        double currentSample;

        // if the file has the double amount of samples, skip every other one
        if (opts.half) {
            currentSample = audioFile.samples[0][i*2];
        } else {
            currentSample = audioFile.samples[0][i];
        }

        // currentSample is between -1 and 1, normalizing
        samples[i] = currentSample*max;
    }

    MidiFile mf;
    mf.setTicksPerQuarterNote(100);

    std::string name = "Example Whatev";

    std::cout << "Generating..." << std::endl;
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

        mm[409] = 0xf7; // End

        mf.addEvent(0, 0, mm);
    }

    //mf.writeHex(std::cout);
    mf.write(opts.outfile);

    std::cout << "Done!" << std::endl;

    return 0;
}
