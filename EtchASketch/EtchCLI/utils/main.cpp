#include "image.h"

int main()
{
	Image* lena = new Image();
	lena->readFromFile("lena.png");
	lena->etchToFile("lena.etch");

	return 0;
}
