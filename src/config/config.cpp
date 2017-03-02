#include "../../include/config/config.h"

const int StaticConfiguration::defaultCamera = 0;
const string StaticConfiguration::gameWindowName = "Detección de movimiento usando Mean Shift";

const Size StaticConfiguration::trackingWindowSize = Size(50, 50);
const Scalar StaticConfiguration::samplingWindowColor = Scalar(255,191,0);
const int StaticConfiguration::samplingWindowThickness = 2;

const Scalar StaticConfiguration::trackingMarkerColor = Scalar(255,191,0);
const int StaticConfiguration::trackingMarkerThickness = 1;

const size_t StaticConfiguration::termCritIters = 10;

vector<string> StaticConfiguration::initialInstructionsLines =
    {"Cubrir de manera", "completa con el", "objeto a seguir y", "tocar una tecla"};

DynamicConfiguration::DynamicConfiguration(const Size& fsz, const string& modeOption) {
    /* Size of the frames fed by the camera in use */
    frameSize = Size(fsz);

    /* Denotes the subregion of the playing region from where the color sample is taken for tracking */
    samplingRegion = Rect (
        frameSize.width - 4*StaticConfiguration::trackingWindowSize.width,
        frameSize.height - 2*StaticConfiguration::trackingWindowSize.height,
        StaticConfiguration::trackingWindowSize.width,
        StaticConfiguration::trackingWindowSize.height
    );

    /* Radius of circular tracking marker */
    trackingMarkerRadius = StaticConfiguration::trackingWindowSize.width / 2;

    runningMode =
        modeOption == "idiomatic" ? IDIOMATIC :
        modeOption == "pointers" ? POINTERS :
        ASM ;
}

DynamicConfiguration dynconf;
