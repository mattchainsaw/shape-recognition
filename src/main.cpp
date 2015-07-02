// Standard library
#include <fstream>
#include <cstdlib>
#include "MedialGraph.h"

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
    char *file;
    if (argc == 2) {
        file = argv[1];
    }
    else {
        cout << "Usage: " << endl;
        cout << "./" << argv[0] << " {Data File}" << endl;
        exit(EXIT_FAILURE);
    }

    // load data for polygon
    vector<Point> poly;
    load(file, poly);

    // Make voronoi diagram
    Voronoi vd;
    vd.insert(poly.begin(), poly.end());

    // Get internal voronoi diagram for medial axis
    MedialGraph graph(vd, poly);

    vector<Vertex> v = graph.dijkstra();
    for (int i=0; i<v.size(); i++)
        cout << v[i].getParent() << ' ' << v[i].getLocation() << endl;

    return 0;
}
