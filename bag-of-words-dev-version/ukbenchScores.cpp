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

    // open the idf
    ifstream ifs_idf("idf.dat", ios::binary);
    if (!ifs_idf) { throw runtime_error("Cannot open file."); }
    size_t idfSize = 0;
    ifs_idf.read((char *)&idfSize, sizeof(size_t));
    vector<float> idf(idfSize);
    ifs_idf.read((char *)&idf[0], sizeof(float) * idfSize);
    ifs_idf.close();

    // open the coordinate of sift feature
    ifstream ifs_pointCoords("pointCoords.dat", ios::binary);
    if (!ifs_pointCoords) { throw runtime_error("Cannot open file."); }
    int coordinateAllRowsSize = 0;
    ifs_pointCoords.read((char *)&coordinateAllRowsSize, sizeof(int));
    vector<vector<Point2f>> coordinateAllRows(coordinateAllRowsSize);
    for (int i = 0; i < coordinateAllRowsSize; i++) {
        int coodRowSize = 0;
        ifs_pointCoords.read((char *)&coodRowSize, sizeof(int));
        vector<Point2f> coodRow;
        coodRow.resize(coodRowSize);
        coordinateAllRows[i].resize(coodRowSize);
        ifs_pointCoords.read((char *)&coodRow[0], sizeof(Point2f) * coodRowSize);
        for (int j = 0; j < coodRowSize; j++){
            coordinateAllRows[i][j].x = coodRow[j].x;
            coordinateAllRows[i][j].y = coodRow[j].y;
        }
    }
    ifs_pointCoords.close();

    // open the words
    ifstream ifs_words("words.dat", ios::binary);
    if (!ifs_words) { throw runtime_error("Cannot open file."); }
    BoWCollection words = BoWCollection::Deserialize(ifs_words);
    ifs_words.close();

    auto imgfns = ReadAllLinesFromFile(argv[1]);

    // read the BoWs
    ifstream ifs("bows.dat", ios::binary);
    if (!ifs) { throw runtime_error("Cannot open file."); }
    BoWCollection bows = BoWCollection::Deserialize(ifs);
    ifs.close();

    float scores = 0;

    //for (int i = 0; i < 1000; i++){
    for (int i = 0; i < imgfns.size(); i++){
        float eachScore = 0;

        // extract features and quantize
        vector<int> word;
        vector<Point2f> queryCood (100);
        auto bow = bowbuilder.Quantize(dict, idf, argv[1], word, queryCood);

        // comparison (HIK)
        vector<pair<int, float>> dists;
        for (int i = 0; i < bows.bows.size(); i++) {
            float dist = Sum(Zip<float, float>(bows.bows[i].bow, bow, [](float f1, float f2) { return min(f1, f2); }));
            dists.push_back(pair<int, float>(i, dist));
        }

        // sort and output
        sort(dists, [](const pair<int, float> &p1, const pair<int, float> &p2) { return p1.second > p2.second; });    // descending
        for (auto p : dists) { cout << p.first << endl; }

        int reRankingDepth = 8;

        // get the match key points
        vector<vector<pair<int, int>>> idx(words.bows.size());
        for(int i = 0; i < reRankingDepth; i++)
        //for(int i = 0; i < words.bows.size(); i++)
        {
            for (int j = 0; j < bow.size(); j++) {
                int index = dists[i].first;
                for (int k = 0; k < words.bows[index].bow.size(); k++){
                    if (word[j] == (int)(words.bows[index].bow)[k]){
                        idx[i].push_back(pair<int, int>(j, k));
                        cout << j << " , " << k << endl;
                    }
                }
            }
        }

        vector<pair<int, float>> reRankScores;


        for (int i = 0; i < reRankingDepth; i++){
            // Localize the object
            vector<Point2f> query;
            vector<Point2f> scene;
            for( int j = 0; j < idx[i].size(); j++){
                // Get the keypoints from the good matches
                cout << idx[i][j].first << " _,_ " << idx[i][j].second << endl;
                query.push_back(queryCood[idx[i][j].first]);
                scene.push_back(queryCood[idx[i][j].second]);
            }

            // compute homography using RANSAC
            Mat mask;
            Mat H = findHomography( query, scene, CV_RANSAC, 3, mask);
            int inliers_cnt = 0, outliers_cnt = 0;
            for (int j = 0; j < mask.rows; j++){
                if (mask.at<uchar>(j) == 1){
                    inliers_cnt++;
                }else {
                    outliers_cnt++;
                }
            }
            int index = dists[i].first;
            float score = (float)inliers_cnt/idx[i].size();
            reRankScores.push_back(pair<int, float>(index, score));
        }

        // sort and output
        sort(reRankScores, [](const pair<int, float> &p1, const pair<int, float> &p2) { return p1.second > p2.second; });    // descending

        // re-ranking result
        for (int i = 0; i < reRankingDepth; i++){
            dists[i] = reRankScores[i];
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
