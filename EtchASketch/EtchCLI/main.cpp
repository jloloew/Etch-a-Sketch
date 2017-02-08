//
//  main.cpp
//  EtchASketch
//
//  Created by Bliss Chapman on 2/7/17.
//  Copyright © 2017 Justin Loew. All rights reserved.
//

#include <stdio.h>
#include "EtchASketch.hpp"

int main(int argc, char *argv[]) {
    printf("Welcome to etch ❤️\n");
    printf("ARGC: %d\n", argc);

    etchasketch::Image input_img = etchasketch::Image(0, 0, NULL);
    etchasketch::ImageFlow input_img_flow = etchasketch::ImageFlow(input_img);

    return 0;
}
