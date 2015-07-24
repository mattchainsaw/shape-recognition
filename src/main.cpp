#include <iostream>
#include <fstream>
#include <Medial.h>

using namespace std;

// The number of branches to find from the pruning function
// Change this value for more or less branches
const unsigned int NUMBER_OF_BRANCHES = 4;

// If you want more accuracy, set this to true
const bool ACCURACY = true;

// default output file info
string DEFAULT_OUTPUT_NAME            = "out";
string DEFAULT_MEDIAL_INFO_EXTENSION  = ".medial";
string DEFAULT_PRUNING_PATH_EXTENSION = ".path";

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
    string file, pathOut, medialOut;
    if (argc == 2) {
        file = argv[1];
        medialOut = (DEFAULT_OUTPUT_NAME + DEFAULT_MEDIAL_INFO_EXTENSION);
        pathOut   = (DEFAULT_OUTPUT_NAME + DEFAULT_PRUNING_PATH_EXTENSION);
    }
    else if (argc == 3) {
        file = argv[1];
        medialOut = (argv[2] + DEFAULT_MEDIAL_INFO_EXTENSION);
        pathOut   = (argv[2] + DEFAULT_PRUNING_PATH_EXTENSION);
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
    load(file.c_str(), poly);
    // Get internal voronoi diagram for medial axis
    Medial medial(poly, ACCURACY);
    // calculate EDF
    medial.CalculateEDF();
    // Pruning function
    MedialPath path = medial.Prune(NUMBER_OF_BRANCHES);

    ofstream outFile;
    outFile.open(medialOut.c_str());
    outFile << medial << endl;
    outFile.close();

    outFile.open(pathOut.c_str());
    outFile << path << endl;
    outFile.close();

    cout << "Medial information has been written to " << medialOut << endl;
    cout << "Pruning path has been written to " << pathOut << endl;

    return 0;
}