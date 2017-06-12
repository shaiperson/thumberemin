#include "../../include/game/Theremin.h"

MidiSoundGenerator* globalMidiSoundGeneratorPointerForSigterm = NULL;

Theremin::Theremin(int width, int height, uchar midiLow, int totalNotes) :
    capture(StaticConfiguration::defaultCamera, width, height)
{
    if(!capture.isOpened()) throw runtime_error("Error initializing camera");

    // Has to be defined before screen object is created
    dynconf = DynamicConfiguration(capture.frameSize(), midiLow, totalNotes);

    screen = new InitialScreen;
    tracker = new ColorSampler;
    sound = new SilentSoundGenerator;

    // assign SIGTERM handler
    signal(SIGINT, cleanExit);
    signal(SIGTERM, cleanExit);

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

void cleanExit(int) {
    if (globalMidiSoundGeneratorPointerForSigterm) {
        // interruption occurred after MidiSoundGenerator was initialized
        globalMidiSoundGeneratorPointerForSigterm->lastNoteOff();
    }
    exit(0);
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

    while (playingMode) {
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

        if (playingMode)
            ((MidiSoundGenerator*)sound)->lastNoteOff();

        continuePlaying = false;
        playingMode = false;
    }

    if (key == 32) {
	    cout << "Restarting." << endl;

        ((MidiSoundGenerator*)sound)->lastNoteOff();

	    delete screen;
    	delete tracker;
    	delete sound;

        screen = new InitialScreen;
	    tracker = new ColorSampler;
    	sound = new SilentSoundGenerator;

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
    sound = new MidiSoundGenerator(midiout, 0);
    globalMidiSoundGeneratorPointerForSigterm = (MidiSoundGenerator*)sound;
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
