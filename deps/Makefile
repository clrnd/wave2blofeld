# Simple Makefile for downloading dependencies from GitHub

midifile = midifile-master
audiofile = audiofile-master
tclap = tclap
all: $(midifile) $(audiofile) $(tclap)

$(midifile):
	wget -O $@.zip https://github.com/craigsapp/midifile/archive/master.zip
	unzip -q $@.zip
	rm $(midifile).zip
	$(MAKE) -C $@ library

$(audiofile):
	mkdir $@
	wget -q -P $@ https://rawgit.com/adamstark/AudioFile/master/AudioFile.cpp
	wget -q -P $@ https://rawgit.com/adamstark/AudioFile/master/AudioFile.h

$(tclap):
	wget -q -O $@.tar.gz https://sourceforge.net/projects/tclap/files/tclap-1.2.1.tar.gz/download
	tar xf $@.tar.gz
	rm $@.tar.gz

clean:
	rm -r $(midifile)
	rm -r $(audiofile)
	rm -r $(tclap)
