//
// takes in an input of data point and normalizes them to all be in range from 0 to 1
//

#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <cmath>

using namespace std;

int main() {
    ifstream file("data/maple");
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
        X[i] += minX;
        Y[i] += minY;
        // shrink down to between 0 and 1
        X[i] *= 1000/maxX;
        Y[i] *= 1000/maxY;
        cout << X[i] << '\t' << Y[i] << endl;
    }
    return 0;
}