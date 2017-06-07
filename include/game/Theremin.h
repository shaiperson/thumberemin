#ifndef THEREMIN_H
#define THEREMIN_H

#include "../global.h"
#include "../config/config.h"
#include "../io/Capture.h"
#include "../gui/PlayingScreen.h"
#include "../gui/InitialScreen.h"
#include "../tracking/TrackingInfo.h"
#include "../tracking/ColorSampler.h"
#include "../tracking/Tracker.h"
#include "../sound/SoundGenerator.h"
#include "../sound/SilentSoundGenerator.h"
#include "../sound/MidiSoundGenerator.h"

#include "../../lib/rtmidi/RtMidi.h"

class Theremin {
public:
    Theremin(int width, int height);
    ~Theremin();
    void run();

private:
    Capture capture;
    Screen* screen;
    TrackingInfo* tracker;
    SoundGenerator* sound;

    uchar midiChannel;
    RtMidiOut* midiout;

    bool playingMode; // TODO make polymorphic with respect to run()?

    void switchToPlayingMode();
    bool keyOptions();
    bool chooseMidiPort( RtMidiOut *rtmidi );

};

#endif
