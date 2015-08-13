#include <algorithm>
#include "BoWBuilder.h"

using namespace std;

void readme() {
    cout << "Usage: ./search <imgfnfile>" << endl
    << "Read in the dictionary from dict.dat, and BoW from bows.dat."
    << "Extract features from the image, quantize, pool, and then output the ranking according to stored BoW." << endl
    << "If a second argument is given, it is expected to be a list of image files and will be used to render an HTML visualization to result.html." << endl;
}

int main(int argc, char **argv) {
    if (argc != 1 && argc != 2) {
        readme();
        return -1;
    }
    //bool isHTML = (argc == 2);
    bool isHTML = 1;

    BoWBuilder bowbuilder;
    // read in files
    auto dict = bowbuilder.ReadCodebook();

    auto imgfns = ReadAllLinesFromFile(argv[1]);

    // read the BoWs
    ifstream ifs("bows.dat", ios::binary);
    if (!ifs) { throw runtime_error("Cannot open file."); }
    BoWCollection bows = BoWCollection::Deserialize(ifs);
    ifs.close();

    float scores = 0;

    for (int i = 0; i < 1000; i++){
    //for (int i = 0; i < imgfns.size(); i++){
        float eachScore = 0;

        // extract features and quantize
        auto bow = bowbuilder.Quantize(dict, imgfns[i]);

        // comparison (HIK)
        vector<pair<int, float>> dists;
        for (int i = 0; i < bows.bows.size(); i++) {
            float dist = Sum(Zip<float, float>(bows.bows[i].bow, bow, [](float f1, float f2) { return min(f1, f2); }));
            //float dist = std::inner_product(bows.bows[i].bow.begin(), bows.bows[i].bow.end(), bow.begin(), 0.0);
            dists.push_back(pair<int, float>(i, dist));
        }

        // sort and output
        sort(dists, [](const pair<int, float> &p1, const pair<int, float> &p2) { return p1.second > p2.second; });    // descending
        for (int k = 0; k < 4; k++) {
            //for (auto p : dists) {
            //cout << p.first << endl;
            cout << dists[k].first << endl;
            if (floor(dists[k].first/4) == floor(i/4)) ++ eachScore;
        }

        scores += eachScore;

        cout << "Query " << i << " image, " << "the score: " << eachScore << endl;
        cout << "Accumulate NS-scores: " << scores/(i+1) << endl;
        cout << "================================" << endl;

        if (isHTML) {
            //auto files = ReadAllLinesFromFile(argv[1]);
            ofstream ofs("result.html");
            if (!ofs) throw runtime_error("Cannot open file.");
            ofs << "<html><head><style> img {width:250px; border:0px; margin:5px 5px; padding:0px 0px;} .divcss5{text-align:center} </style></head><body><div class=\"divcss5\"><h2>Query</h2><div><img src=\"" << imgfns[i] << "\" alt=\"\" /><br />";
            //ofs << "<html><head><style> img { max-width: 320px; }</style></head><body><h2>Query</h2><img src=\"" << argv[1] << "\" alt=\"\" /><br />";
            ofs << "<div class=\"divcss5\"><h2>Result</h2></div><table><tbody>";
            for (auto p : dists) {
                ofs << "<img alt=\"\" src=\"" << imgfns[p.first] << "\" />";
                //ofs << "<tr><td><img alt=\"\" src=\"" << files[p.first] << "\" /></td><td><p>" << p.second << "</p></td></tr>";
            }
            ofs << "</tbody></table></body></html>";
            ofs.close();
            //cerr << "Html outputted." << endl;
        }

    }
    //cout << scores/(4*imgfns.size()) << endl;
    return 0;
}
