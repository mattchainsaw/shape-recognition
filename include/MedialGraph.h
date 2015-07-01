#ifndef MEDIAL_GRAPH
#define MEDIAL_GRAPH
#include <iostream> // debug
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2                                          Point;
typedef std::map<Point,double> Neighbors; // map another vertex with a distance

class Vertex {
private:
    Point location;
    Neighbors neighbors;
public:
    double dist;    // used for Dijkstra's
    Point parent;
      
    Vertex(Point p = Point(0,0))
            : location(p) {}

    bool operator<(const Vertex& other) const {
        return return (other.location < location);
    }
    bool operator>(const Vertex& other) const {
        return (other.location > location);
    }
    bool operator!=(const Vertex& other) const {
        return (other.location != location);
    }
    bool operator==(const Vertex& other) const {
        return (other.location == location);
    }

    Point point() {
        return location;
    }
    Neighbors getNeighbors() {
        return neighbors;
    }
    void addNeighbor(Point p) {
        neighbors[p] = distance(location,p);
    }
};
typedef std::map<Point,Vertex> Vertices;  // individual points wil be graphs vertices

struct compare {
    bool operator()(const Vertex& v1, const Vertex& v2) {
        return v1.dist < v2.dist;
    }
};

class MedialGraph {
private:
    Vertices vertices;

    void initialize(const Point& p);

    void relax(const Point& u, const Point& v);

    std::vector<Vertex> getVertices();

public:
    MedialGraph() {}

    void add(const Point& p, const Point& q);

    std::vector<Vertex> dijkstra(Point start);

};
#endif // MEDIAL_GRAPH