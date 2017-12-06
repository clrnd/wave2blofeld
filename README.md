wave2blofeld
============

A simple tool for transforming wavetables from a WAV file
into Waldorf's Blofeld SysEx midi format.

Wavetables must come in a single channel WAV file with 64
waves next to each other, each with 128 (or 256) samples.
The Blofeld has 38 slots (from 80 to 118) for custom Wavetables,
and each can have a 14 character ASCII name.

Download from the [releases](https://github.com/alvare/wave2blofeld/releases) page.

## Usage

```
USAGE:

   wave2blofeld  -n <number> [-d] [--] [--version] [-h] <infile>
                   <outfile>


Where:

   -n <number>,  --number <number>
     (required)  Wavetable to write to.

   -d,  --double
     Use only half of the samples for each wave. Useful for banks created
     with 256 samples per wave.

   --,  --ignore_rest
     Ignores the rest of the labeled arguments following this flag.

   --version
     Displays version information and exits.

   -h,  --help
     Displays usage information and exits.

   <infile>
     (required)  Input WAV file.

   <outfile>
     (required)  Output midi file.


   A simple tool for transforming wavetables from a WAV file into Waldorf's
   Blofeld SysEx midi format.
```

For example, suppose you have a WAV file `wavetable3.wav` that has
`64*128 = 8192` samples:

```
$ soxi wavetable3.wav
Input File     : 'wavetable3.wav'
Channels       : 1
Sample Rate    : 44100
Precision      : 16-bit
Duration       : 00:00:00.19 = 8192 samples = 13.932 CDDA sectors
File Size      : 16.4k
Bit Rate       : 707k
Sample Encoding: 16-bit Signed Integer PCM
```

Then, you would generate a SysEx file for transfering this
wavetable into slot 82 with the name "Awful Sound" like this:

```
wave2blofeld wavetable3.wav out.mid -s 3 -n "Awful Sound"
```

Then it's just a matter of playing the midi file into the Blofeld with something
like [SysEx Librarian](https://www.snoize.com/SysExLibrarian/).

### Doubled Wavetables

Some programs like [WaveEdit](https://github.com/AndrewBelt/WaveEdit) create
banks with 256 samples per wave. Using the `-d` flag will
skip odd samples so it fits the Blofeld format. This of course
could introduce audible differences.

## Building

First, get the dependencies running make in `deps/`:

```
cd deps/
make
```

After that's done, build it like a normal CMake project:

```
mkdir build && cd build/
cmake ..
make
```
