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
    vector<float> idf(bowb.DICT_SIZE);
    auto dict = bowb.BuildCodebookAndQuantize(imgfns, features, bows, idf);
    // after the above line, each feature in feature is a Mat: SIFT number*128
    // each bow in bows is: k*1

    // store the codebook
    bowb.WriteCodebook(dict);

    // store the idf
    ofstream ofs_idf("idf.dat", ios::binary);
    if (!ofs_idf) { throw runtime_error("Cannot open file."); }
    size_t idfSize = idf.size();
    ofs_idf.write((char *)&idfSize, sizeof(idfSize));
    ofs_idf.write((char *)&idf[0], idf.size() * sizeof(float));
    ofs_idf.close();

    // store the BoW
    ofstream ofs("bows.dat", ios::binary);
    if (!ofs) { throw runtime_error("Cannot open file."); }
    bows.Serialize(ofs);
    ofs.close();
}
