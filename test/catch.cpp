#define CATCH_CONFIG_RUNNER
#include "catch.hpp"
#include "../include/in-house-tracking/iht.h"

/*
    - main structure basically copied from Catch reference page
    - added to allow running multiple mean shift cases with loop
*/

#include <iostream>
#include <vector>

using namespace std;

int prevMultipleOf(int n, int d) {
    if (n < d)
        return d;
    return n - n % d;
}

struct randomMeanshiftCase {
    Mat image;
    Rect ihtCvWindow, cvWindow;
    window ihtAsmWindow, ihtPtrsWindow;
    randomMeanshiftCase() :
        image(100, 100, CV_16UC1),
        ihtCvWindow(
            rand() % image.cols/4,
            rand() % image.rows/4,

            rand() % (image.cols*3/4),
            /*
            prevMultipleOf ( rand() % (image.cols*3/4), 4 ),
            */

            rand() % (image.rows*3/4)
        ),
        cvWindow(ihtCvWindow)
    {
        ihtAsmWindow = {ihtCvWindow.x, ihtCvWindow.y, ihtCvWindow.width, ihtCvWindow.height};
        ihtPtrsWindow = {ihtCvWindow.x, ihtCvWindow.y, ihtCvWindow.width, ihtCvWindow.height};
        for (size_t i = 0; i < image.rows; ++i) {
            for (size_t j = 0; j < image.cols; ++j) {
                image.at<short>(i,j) = rand() % SHRT_MAX;
            }
        }
    }
};

bool operator==(const Rect& rect, const window& window) {
    return rect == Rect(window.x, window.y, window.width, window.height);
}

bool operator==(const window& window, const Rect& rect) {
    return rect == window;
}

bool operator==(const window& w1, const window& w2) {
    return Rect(w1.x, w1.y, w1.width, w1.height) == w2;
}

ostream& operator<<(ostream& o, const window& window) {
    o << Rect(window.x, window.y, window.width, window.height);
    return o;
}

int main( int argc, char* argv[] ) {
  // global setup...

  int result = Catch::Session().run( argc, argv );

  vector<randomMeanshiftCase> cases(20);

  size_t iters = 100;
  for (auto it = cases.begin(); it != cases.end(); ++it) {
      // cout << it->ihtCvWindow.width % 4 << " ";

      IHT_meanShift_ASM(it->image.data, it->image.rows, it->image.cols, it->image.step, &it->ihtAsmWindow, iters);
      IHT_meanShift(it->image.data, it->image.rows, it->image.cols, it->image.step, &it->ihtPtrsWindow, iters);
      IHT_meanShift_CV(it->image, it->ihtCvWindow, iters);
      meanShift(it->image, it->cvWindow, TermCriteria(TermCriteria::COUNT, iters, 0));

      if (!(it->ihtPtrsWindow == it->ihtCvWindow)) {
          cout << "ihtPtrsWindow != ihtCvWindow" << endl;
          cout << "Case index " << it-cases.begin() << ". Windows:" << endl;
          cout << "ihtPtrsWindow " << it->ihtPtrsWindow << endl;
          cout << "ihtCvWindow " << it->ihtCvWindow << endl;
          return 1;
      }

      if (!(it->ihtPtrsWindow == it->cvWindow)) {
          cout << "ihtPtrsWindow != cvWindow" << endl;
          cout << "Case index " << it-cases.begin() << ". Windows:" << endl;
          cout << "ihtPtrsWindow " << it->ihtPtrsWindow << endl;
          cout << "cvWindow " << it->cvWindow << endl;
          return 1;
      }

      if (!(it->ihtAsmWindow == it->ihtPtrsWindow)) {
          cout << "ihtAsmWindow != ihtPtrsWindow" << endl;
          cout << "Case index " << it-cases.begin() << ". Windows:" << endl;
          cout << "ihtAsmWindow " << it->ihtAsmWindow << endl;
          cout << "ihtPtrsWindow " << it->ihtPtrsWindow << endl;
          return 1;
      }

      if (!(it->ihtAsmWindow == it->cvWindow)) {
          cout << "ihtAsmWindow != cvWindow" << endl;
          cout << "Case index " << it-cases.begin() << ". Windows:" << endl;
          cout << "ihtAsmWindow " << it->ihtAsmWindow << endl;
          cout << "cvWindow " << it->cvWindow << endl;
          return 1;
      }
  }

  cout << "Mean shift tests all OK" << endl;

  return ( result < 0xff ? result : 0xff );
}
