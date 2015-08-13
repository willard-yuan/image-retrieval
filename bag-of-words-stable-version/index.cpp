//
//  main.cpp
//  BoVW
//
//  Created by willard on 15/7/21.
//  Copyright (c) 2015年 wilard. All rights reserved.
//

#include "BoWBuilder.h"

using namespace std;

void readme() {
    cout << "Usage: ./feature <infile>" << endl
    << "Build BoW representation for the given list of images."
    << "infile is a text file, containing several lines, each of which is a image file name." << endl
    << "The dictionary would be stored in dict.dat, and the quantized BoW vectors would be stored in bows.dat.";
}

int main(int argc, char **argv) {
    if (argc != 2) {
        readme();
        return -1;
    }

    auto imgfns = ReadAllLinesFromFile(argv[1]);
    BoWBuilder bowb;
    vector<Mat> features;
    BoWCollection bows;
    auto dict = bowb.BuildCodebookAndQuantize(imgfns, features, bows);

    // store the codebook
    bowb.WriteCodebook(dict);

    // store the BoW
    ofstream ofs("bows.dat", ios::binary);
    if (!ofs) { throw runtime_error("Cannot open file."); }
    bows.Serialize(ofs);
    ofs.close();
}
