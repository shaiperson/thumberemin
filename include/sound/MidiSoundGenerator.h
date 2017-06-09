#ifndef MIDISOUNDGENERATOR_H
#define MIDISOUNDGENERATOR_H

#include "SoundGenerator.h"
#include "../../lib/rtmidi/RtMidi.h"

// Platform-dependent sleep routines.
#include <unistd.h>
#define SLEEP(milliseconds) usleep( (unsigned long) (milliseconds * 1000.0) )

class MidiSoundGenerator : public SoundGenerator {
public:
    MidiSoundGenerator(RtMidiOut* midiout, uchar channel);
    void update(const TrackingInfo& tracker);
    void lastNoteOff();
private:
    enum keypress {ON, OFF};
    RtMidiOut* midiout;
    uchar channel;
    uchar prevNote; // necessary for sending note OFF message before new note ON
    const uchar velocityNoteON = 64;
    const uchar velocityNoteOFF = 0;
    void sendMidiMessageForNote(uchar midiNote, keypress p);
};

#endif
