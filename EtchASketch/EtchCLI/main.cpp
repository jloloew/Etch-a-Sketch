//
//  main.cpp
//  EtchASketch
//
//  Created by Bliss Chapman on 2/7/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#include <iostream>
#include "EtchASketch.hpp"

using std::cout;
using std::endl;

int
main(int argc, char *argv[])
{
    cout << "Welcome to etch ❤️" << endl;
    cout << "ARGC: " << argc << endl;

    etchasketch::Image inputImg = etchasketch::Image(0, 0, NULL);
    etchasketch::ImageFlow inputImgFlow = etchasketch::ImageFlow(inputImg);

    return 0;
}

