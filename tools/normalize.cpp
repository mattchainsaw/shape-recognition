// takes in an input of data point and normalizes them to all be in range.
#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cmath>
using namespace std;

const int RANGE_MIN = 0;
const int RANGE_MAX = 100;

int main(int argc, char** argv) {
    char *fileName;
    if (argc == 2) {
        fileName = argv[1];
    }
    else {
        cout << "Usage: " << endl;
        cout << "./" << argv[0] << " {Data File}" << endl;
        exit(EXIT_FAILURE);
    }
    ifstream file(fileName);
    vector<double> X;
    vector<double> Y;
    double minX = numeric_limits<double>::max();
    double minY = minX;
    double maxX = numeric_limits<double>::min();
    double maxY = maxY;
    double _x, _y;
    while (file >> _x >> _y) {
        X.push_back(_x);
        Y.push_back(_y);
        minX = (minX > _x) ? _x:minX;
        minY = (minY > _y) ? _y:minY;
        maxX = (maxX < _x) ? _x:maxX;
        maxY = (maxY < _y) ? _y:maxY;
    }
    minX = abs(minX);
    minY = abs(minY);
    maxX = abs(maxX + minX);
    maxY = abs(maxY + minY);
    for (int i=0; i<X.size(); i++) {
        // translate to all positive numbers
        X[i] += minX + RANGE_MIN;
        Y[i] += minY + RANGE_MIN;
        // shrink down to between MIN and MAX
        X[i] *= RANGE_MAX/maxX;
        Y[i] *= RANGE_MAX/maxY;
        cout << X[i] << '\t' << Y[i] << endl;
    }
    return 0;
}