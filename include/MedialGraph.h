#include <iostream> // debug
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2                                          Point;

typedef std::map<Point,double> Neighbors; // map another vertex with a distance

#define INF 10000.0

double distance(Point a, Point b) {
  return sqrt((b.x() - a.x())*(b.x() - a.x()) + (b.y() - a.y())*(b.y() - a.y()));
}

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

    void initialize(const Point& p) {
      for (Vertices::iterator it=vertices.begin(); it!=vertices.end(); it++) {
        it->second.dist = (it->second.point() == p) ? 0:INF;
        it->second.parent = Point(INF,INF);
//        std::cout << "init " << it->second.dist << std::endl;
      }
    }

    void relax(const Point& u, const Point& v) {
      if (vertices[v].dist > (vertices[u].dist + distance(u,v))) {
        vertices[v].dist = vertices[u].dist + distance(u,v);
        vertices[v].parent = vertices[u].point();
      }
    }

    std::vector<Vertex> getVertices() {
      std::vector<Vertex> d;
      for (Vertices::iterator it=vertices.begin(); it!=vertices.end(); it++) {
        d.push_back(it->second);
      }
      return d;
    }

  public:
    MedialGraph() {}

    void add(const Point& p, const Point& q) {
      if (vertices[p].point() != p)
        vertices[p] = Vertex(p);
      if (vertices[q].point() != q)
        vertices[q] = Vertex(q);
      vertices[p].addNeighbor(q);
      vertices[q].addNeighbor(p);
    }

    std::vector<Vertex> dijkstra(Point start) {
      initialize(start);
      std::vector<Vertex> v;
      std::vector<Vertex> heap = getVertices();
      std::make_heap (heap.begin(), heap.end(), compare());
      while (!heap.empty()) {
        std::sort_heap(heap.begin(), heap.end());
        std::pop_heap(heap.begin(), heap.end());
        Vertex vertex = heap.back();
        heap.pop_back();
        v.push_back(vertex.point());
        for (Neighbors::iterator it=vertex.getNeighbors().begin(); it!=vertex.getNeighbors().end(); it++) {
          relax(vertex.point(), it->first);
        }
      }
      return v;
    }

};




