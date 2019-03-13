# Thumberemin

A virtual musical instrument application inspired by the [Theremin](https://en.wikipedia.org/wiki/Theremin "Wikipedia"). The application allows you to control the instrument's pitch by moving your thumb up and down in front of a webcam using a motion tracking algorithm. Quick demo:

[![MIDI back end demo](https://img.youtube.com/vi/hG-Z1Z72Hyc/0.jpg)](https://www.youtube.com/watch?v=hG-Z1Z72Hyc)

## Motion tracking

Motion tracking in this application is based on Mean Shift ([OpenCV's Mean Shift tutorial](https://docs.opencv.org/3.4/db/df8/tutorial_py_meanshift.html "OpenCV Mean Shift tutorial") was very useful for this.). The user's finger is tracked and its position on the screen is used to determine the pitch to be played. This logic can be summarized as follows:

1. Record a color sample of the user's thumb (or any object for that matter) and calculate a histogram for it.
2. Capture a camera frame, back-project the sample histogram onto it (see [Back Projection](https://en.wikipedia.org/wiki/Histogram_equalization#Back_projection "Wikipedia")) and obtain a confidence map that assigns each pixel a probability of belonging to the thumb.
3. Detect the thumbs's current location by locally maximizing confidence values using Mean Shift.
4. Calculate a pitch frequency according to the finger's position relative to the _playing region_ and feed it to the audio back end to generate sound.
5. Go back to 2.

### SSE implementation

As a final course project for a University of Buenos Aires (UBA) Computer Organization course, a motion tracking application using this very tracking algorithm was also implemented purely with Intel x86 Streaming SIMD Extensions. A substantial improvement in performance was achieved, with the SSE vectorial implementation running up to 20 to 30 times faster than different sequential C++ implementations. While the application is not currently limited by frame-to-frame performance, such improvements are interesting since they provide leeway for further processing of camera frame information, such as camera noise reduction.

The SSE implementation is available [in this repository](https://github.com/ballcue/meanshift-motion-tracking). A very detailed description of the algorithm, its re-implementation to fit the SIMD model and time benchmarking results are included in report-ES.pdf. This document is written in Spanish since it was presented as a lab report for the related school project.

## Sound

This implementation allows for an easy replacement of the sound back end by creating a subclass of the abstract class `SoundGenerator`. The new class defines its `update` method which handles updating playback according to tracking information provided to it with each invocation.

### Low-level audio back end

Such a `SoundGenerator` subclass, called `RangeSoundGenerator`, is included as an example and is used by default in the `master` branch. In each call to `update`, it determines the frequency to be played out of a statically defined set of frequencies and explicitly produces an array of samples to be fed to the system through the [PortAudio](http://www.portaudio.com/ "PortAudio") audio I/O API.

This is a very low-level approach that is mostly intended to allow for a proof of concept of the application. It is quite inflexible and leads to lesser audio quality.

A demo of the application running on this low-level back end is available here:

[![Low-level back end demo](https://img.youtube.com/vi/IAlZaFgW-5o/0.jpg)](https://www.youtube.com/watch?v=IAlZaFgW-5o)

### MIDI back end

A higher-level audio back end implementation is possible through MIDI. In this approach, a `SoundGenerator` subclass can take care of updating playback by writing MIDI notes into a port for a running MIDI back end to read and reproduce. A working implementation of this approach is available in the `theremoog` branch.

[The application demo](https://youtu.be/hG-Z1Z72Hyc "MIDI back end demo") shown in the first video, linked in the beginning of this README, is using such a back end. This is a result of a collaboration with the creators of the [RaffoSynth](https://github.com/nicoroulet/moog "RaffoSynth"), where their virtual Minimoog is running as a MIDI back end. In this form, this project was presented in the University of Buenos Aires (UBA) Computer Science Fair 2017.

