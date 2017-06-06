#include "../../include/sound/MidiSoundGenerator.h"

MidiSoundGenerator::MidiSoundGenerator(RtMidiOut* midiout, uchar channel) : midiout(midiout), channel(channel) { }

void MidiSoundGenerator::update(const TrackingInfo& tracker) {
    // send note OFF for previous note
    sendMidiMessageForNote(prevNote, OFF);

    // send note ON for new note
    uchar currNote = dynconf.pixel2MidiNote[tracker.current().y];
    sendMidiMessageForNote(currNote, ON);

    // update prevNote
    prevNote = currNote;
}

void MidiSoundGenerator::sendMidiMessageForNote(uchar midiNote, keypress p) {
    uchar statusByte = (p == ON ? 144 : 128) + channel;

    vector<uchar> message(3);
    message[0] = statusByte;
    message[1] = midiNote;
    message[2] = p == ON ? velocityNoteON : velocityNoteOFF;

    // SLEEP( 500 );
    midiout->sendMessage(&message);
    // SLEEP( 500 );
}
