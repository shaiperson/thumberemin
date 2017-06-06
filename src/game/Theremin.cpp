#include "../../include/game/Theremin.h"

Theremin::Theremin(uchar midiChannel) :
    capture(StaticConfiguration::defaultCamera),
    midiChannel(midiChannel)
{
    if(!capture.isOpened()) throw runtime_error("Error initializing camera");

    // Has to be defined before screen object is created
    dynconf = DynamicConfiguration(capture.frameSize());

    screen = new InitialScreen;
    tracker = new ColorSampler;
    sound = new SilentSoundGenerator;

    // create MIDI out object
    midiout = new RtMidiOut();
    try {
    } catch (RtMidiError& error) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }

    // choose MIDI port
    try {
        if (!chooseMidiPort(midiout)) {
            cerr << "No se pudo abrir el puerto pedido" << endl;
            exit(EXIT_FAILURE);
        }
    }
    catch ( RtMidiError &error ) {
        error.printMessage();
        exit(EXIT_FAILURE);
    }
}

Theremin::~Theremin() {
    delete screen;
    delete tracker;
    delete midiout;
    capture.release();
}

void Theremin::run() {
    /* wait for user to choose playing mode */
    Mat frame;
    while (keyOptions()) {
        capture >> frame;
        tracker->update(frame);
        screen->update(frame, *tracker);
        sound->update(*tracker);
    }

    if (playingMode) {
        while (keyOptions()) {
            capture >> frame;
            tracker->update(frame);
            screen->update(frame, *tracker);
            sound->update(*tracker);
        }
    }
}

bool Theremin::keyOptions() {
    int key = waitKey(1);
    bool continuePlaying = true;

    /* enter key */
    if (key == 10) {
        cout << "Entering playing mode" << endl;
        switchToPlayingMode();
        continuePlaying = false;
    }
    /* 'q' key */
    if (key == 113) {
        cout << "K, quitting." << endl;
        continuePlaying = false;
    }

    return continuePlaying;
}

void Theremin::switchToPlayingMode() {
    /* set private playingMode flag */
    playingMode = true;

    /* replace inital screen with playing screen */
    delete screen;
    screen = new PlayingScreen;

    /* take color sample from samplingRegion and replace ColorSampler with Tracker */
    Mat frameForSampling;
    capture >> frameForSampling;
    Mat sampleHistogram = tracker->takeSample(frameForSampling);

    delete tracker;
    tracker = new Tracker(sampleHistogram);

    /* replace silent sound generator with range sound generator */
    delete sound;
    sound = new MidiSoundGenerator(midiout, midiChannel);
}

bool Theremin::chooseMidiPort(RtMidiOut *rtmidi) {

    std::string portName;
    unsigned int i = 0, nPorts = rtmidi->getPortCount();
    if ( nPorts == 0 ) {
      std::cout << "No output ports available!" << std::endl;
      return false;
    }

    if ( nPorts == 1 ) {
      std::cout << "\nOpening " << rtmidi->getPortName() << std::endl;
    }
    else {
      for ( i=0; i<nPorts; i++ ) {
        portName = rtmidi->getPortName(i);
        std::cout << "  Output port #" << i << ": " << portName << '\n';
      }

      do {
        std::cout << "\nChoose a port number: ";
        std::cin >> i;
      } while ( i >= nPorts );
    }

    std::cout << "\n";
    rtmidi->openPort( i );

    return true;
}
