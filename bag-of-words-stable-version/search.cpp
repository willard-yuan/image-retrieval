#include <algorithm>
#include "BoWBuilder.h"

using namespace std;

void readme() {
    cout << "Usage: ./search <imgfile> <imgfnfile>" << endl
    << "Read in the dictionary from dict.dat, and BoW from bows.dat."
    << "Extract features from the image, quantize, pool, and then output the ranking according to stored BoW." << endl
    << "If a second argument is given, it is expected to be a list of image files and will be used to render an HTML visualization to result.html." << endl;
}

int main(int argc, char **argv) {
    if (argc != 2 && argc != 3) {
        readme();
        return -1;
    }
    bool isHTML = (argc == 3);

    BoWBuilder bowbuilder;
    // read in files
    auto dict = bowbuilder.ReadCodebook();
    // extract features and quantize
    auto bow = bowbuilder.Quantize(dict, argv[1]);
    // read the BoWs
    ifstream ifs("bows.dat", ios::binary);
    if (!ifs) { throw runtime_error("Cannot open file."); }
    BoWCollection bows = BoWCollection::Deserialize(ifs);
    ifs.close();

    // comparison (HIK)
    vector<pair<int, float>> dists;
    for (int i = 0; i < bows.bows.size(); i++) {
        // histogram intersection kernel distance, this distance spend too much time to compute.
        float dist = Sum(Zip<float, float>(bows.bows[i].bow, bow, [](float f1, float f2) { return min(f1, f2); }));

        // cosin distance
        //float dist = std::inner_product(bows.bows[i].bow.begin(), bows.bows[i].bow.end(), bow.begin(), 0.0);

        dists.push_back(pair<int, float>(i, dist));
    }

    // sort and output
    sort(dists, [](const pair<int, float> &p1, const pair<int, float> &p2) { return p1.second > p2.second; });    // descending
    for (auto p : dists) { cout << p.first << endl; }

    if (isHTML) {
        auto files = ReadAllLinesFromFile(argv[2]);
        ofstream ofs("result.html");
        if (!ofs) throw runtime_error("Cannot open file.");
        ofs << "<html><head><style> img {width:250px; hight:250px; border:0px; margin:5px 5px; padding:0px 0px;} .divcss5{text-align:center} </style></head><body><div class=\"divcss5\"><h2>Query</h2><div><img src=\"" << argv[1] << "\" alt=\"\" /><br />";
        //ofs << "<html><head><style> img { max-width: 320px; }</style></head><body><h2>Query</h2><img src=\"" << argv[1] << "\" alt=\"\" /><br />";
        ofs << "<div class=\"divcss5\"><h2>Result</h2></div><table><tbody>";
        for (int i = 0; i < 40; i++) {
        //for (auto p : dists) {
            ofs << "<img alt=\"\" src=\"" << files[dists[i].first] << "\" />";
            //ofs << "<img alt=\"\" src=\"" << files[p.first] << "\" />";
            //ofs << "<tr><td><img alt=\"\" src=\"" << files[p.first] << "\" /></td><td><p>" << p.second << "</p></td></tr>";

        }
        ofs << "</tbody></table></body></html>";
        ofs.close();
        cerr << "Html outputted." << endl;
    }

    return 0;
}
