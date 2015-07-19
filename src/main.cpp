#include <iostream>
#include <fstream>
#include <Medial.h>

using namespace std;

// data input
void load(const char *str, vector<Point> &vp) {
    ifstream file(str);
    while (!file.is_open()) {
        cout << "Error finding file." << endl;
        exit(EXIT_FAILURE);
    }
    double x, y;
    while (file >> x >> y) {
        vp.push_back(Point(x, y));
    }
}

int main(int argc, char **argv) {
    // Get input file
    const char *file, *medialOut, *pathOut;
    if (argc == 2) {
        file = argv[1];
        medialOut = "out.medial";
        pathOut = "out.path";
    }
    else if (argc == 3) {
        file = argv[1];
        medialOut = argv[2];
        pathOut = "out.path";
    }
    else if (argc == 4) {
        file = argv[1];
        medialOut = argv[2];
        pathOut = argv[3];
    }
    else {
        cout << "Usages: " << endl;
        cout << "./" << argv[0] << " {Input File}" << endl;
        cout << "./" << argv[0] << " {Input File} {Output for medial info} {Output for path info}" << endl;
        cout << endl << "Default output files are \"out.medial\" and \"out.path\"" << endl;
        exit(EXIT_FAILURE);
    }
    // load data for polygon
    vector<Point> poly;
    load(file, poly);
    // Get internal voronoi diagram for medial axis
    Medial medial(poly);
    // calculate EDF
    medial.CalculateEDF();
    MedialPath path = medial.Prune();

    ofstream outFile;
    outFile.open(medialOut);
    outFile << medial << endl;
    outFile.close();

    outFile.open(pathOut);
    outFile << path << endl;
    outFile.close();

    return 0;
}