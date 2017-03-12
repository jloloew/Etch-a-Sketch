#include <stdio.h>
#include "image.h"

using namespace std;

/*
void usage() {
    return;
}
*/

int
main(int argc, char * const argv[])
{
    cout << "Welcome to the etcher utility, the one and only .etch file generator!" << endl;

    // Parse arguments.
    /*
    string inFile;
    long imgWidth = -1, imgHeight = -1;
    int ch;
    while ((ch = getopt(argc, argv, "i")) != -1) {
        switch (ch) {
            case 'i':
                inFile = string(optarg);
                break;
            case '?':
            default:
                usage();
        }
    }
    */

	Image* imageToEtch = new Image();
	imageToEtch->readFromFile("lena.png");
	imageToEtch->etchToFile("lena.etch");

    cout << "I have written your etch file to lena.etch 🎉" << endl;

	return 0;
}
