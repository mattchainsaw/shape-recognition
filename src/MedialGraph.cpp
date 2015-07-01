#include <iostream> // debug
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include "MedialGraph.h"

#define INF 10000.0

double distance(Point a, Point b) {
    return sqrt((b.x() - a.x())*(b.x() - a.x()) + (b.y() - a.y())*(b.y() - a.y()));
}

void MedialGraph::initialize(const Point& p) {
    for (Vertices::iterator it=vertices.begin(); it!=vertices.end(); it++) {
        it->second.dist = (it->second.point() == p) ? 0:INF;
        it->second.parent = Point(INF,INF);
//        std::cout << "init " << it->second.dist << std::endl;
    }
}

void MedialGraph::relax(const Point& u, const Point& v) {
    if (vertices[v].dist > (vertices[u].dist + distance(u,v))) {
        vertices[v].dist = vertices[u].dist + distance(u,v);
        vertices[v].parent = vertices[u].point();
    }
}

std::vector<Vertex> MedialGraph::getVertices() {
    std::vector<Vertex> d;
    for (Vertices::iterator it=vertices.begin(); it!=vertices.end(); it++) {
        d.push_back(it->second);
    }
    return d;
}

void MedialGraph::add(const Point& p, const Point& q) {
    if (vertices[p].point() != p)
        vertices[p] = Vertex(p);
    if (vertices[q].point() != q)
        vertices[q] = Vertex(q);
    vertices[p].addNeighbor(q);
    vertices[q].addNeighbor(p);
}

std::vector<Vertex> MedialGraph::dijkstra(Point start) {
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
