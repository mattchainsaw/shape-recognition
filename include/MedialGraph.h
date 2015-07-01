#ifndef MEDIAL_GRAPH_H
#define MEDIAL_GRAPH_H
#include <iostream> // debug
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel                        Kernel;
typedef Kernel::Point_2                                                            Point;
typedef std::map<Point,double>                                                     Neighbors; // map another vertex with a distance
typedef CGAL::Delaunay_triangulation_2<Kernel>                                     Delaunay;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<Delaunay>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<Delaunay> AP;
typedef CGAL::Voronoi_diagram_2<Delaunay,AT,AP>                                    Voronoi;

class Vertex {
private:
    Point location;
    Neighbors neighbors;
    double dist;    // used for Dijkstra's
    Point parent;

public:

    Vertex(Point p = Point(0,0))
            : location(p) {}

    bool operator<(const Vertex& other) const {
        return (other.location < location);
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
    Point getLocation() const {
        return location;
    }
    void setLocation(const Point& p) {
        location = p;
    }
    Neighbors getNeighbors() const {
        return neighbors;
    }
    void addNeighbor(Point p) {
        neighbors[p] = std::sqrt((p.x() - location.x())*(p.x() - location.x()) +
                                 (p.y() - location.y())*(p.y() - location.y()));
    }
    double getDist() const {
        return dist;
    }
    void setDist(const double& d) {
        dist = d;
    }
    Point getParent() const {
        return parent;
    }
    void setParent(const Point& p) {
        parent = p;
    }

};
typedef std::map<Point,Vertex> Vertices;  // individual points wil be graphs vertices

// for Vertex heap
struct compare {
    bool operator()(const Vertex& v1, const Vertex& v2) {
        return v1.getDist() < v2.getDist();
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
    MedialGraph(const Voronoi& v, const std::vector<Point>& shape);
    void add(const Point& p, const Point& q);

    std::vector<Vertex> dijkstra(Point start);

};
#endif // MEDIAL_GRAPH