#include <stdio.h>
#include <unistd.h>
#include "image.h"

using namespace std;

void usage() {
  cout << "Usage: etch-convert -i /path/to/input/image.png" << endl;
  return;
}

int
main(int argc, char * const argv[])
{
  cout << "Welcome to etch-convert, the one and only .png ➡️  .etch file converter!" << endl;

  // Parse arguments.
  string inputFile;
  int ch;
  while ((ch = getopt(argc, argv, "i:")) != -1) {
      switch (ch) {
          case 'i':
              inputFile = string(optarg);
              break;
          case '?':
          default:
              usage();
              exit(EXIT_FAILURE);
      }
  }

  // Validate arguments.
  if (inputFile.length() <= 4) { // .png = 4 chars
    usage();
    exit(EXIT_FAILURE);
  }

  // Attempt to create image from input file name.
	Image* inputImage = new Image();
	inputImage->readFromFile(inputFile);

  // Attempt to etch image to file.
  string outputFile = inputFile.substr(0, inputFile.find("."));
  outputFile.append(".etch");
	inputImage->etchToFile(outputFile);

  cout << "Your etched file has been written to " << outputFile << " 🎉" << endl;

	return EXIT_SUCCESS;
}
