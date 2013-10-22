#include <iostream>
#include "Model.h"

#define OBJECTS 20000
#define FRAMES 1000

using namespace std;

int main(int argc, char * argv[]) {
    // Check command line arguments
    if (argc < 2) {
        cout << "ERROR: Too few command line arguments." << endl;
        cout << "USAGE: > simulator filename " << endl;
        return 1;
    }
    // Get filename from command line
    char * filename = argv[1];
 
    Model model(OBJECTS);
    model.init();
 
    cout << "Producing animation data" << endl;
    model.record_frame();
    for (int i=1; i<FRAMES; i++) {
        model.advance_frame();
        model.record_frame();
    }
    model.finalize_record(filename);
    cout << "Finished producing timeseries data in " << filename << "." << endl;

    return 0;

 
}
