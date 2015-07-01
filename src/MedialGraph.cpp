#include <iostream> // debug
#include <algorithm>
#include <vector>
#include <map>
#include <cmath>
#include "MedialGraph.h"

#define INF 10000.0

double pointDistance(const Point& p, const Point& q) {
    return std::sqrt((p.x() - q.x())*(p.x() - q.x()) + (p.y() - q.y())*(p.y() - q.y()));
}

bool inside(const Point& p, const std::vector<Point>& shape) {
    return CGAL::bounded_side_2(shape.begin(), shape.end(), p, Kernel()) == CGAL::ON_BOUNDED_SIDE;
}

void MedialGraph::initialize(const Point& p) {
    for (Vertices::iterator it=vertices.begin(); it!=vertices.end(); it++) {
        it->second.setDist((it->second.getLocation() == p) ? 0:INF);
        it->second.setParent(Point(INF,INF));
    }
}

void MedialGraph::relax(const Point& u, const Point& v) {
    if (vertices[v].getDist() > (vertices[u].getDist() + pointDistance(u,v))) {
        vertices[v].setDist(vertices[u].getDist() + pointDistance(u,v));
        vertices[v].setParent(vertices[u].getLocation());
    }
}

std::vector<Vertex> MedialGraph::getVertices() {
    std::vector<Vertex> d;
    for (Vertices::iterator it=vertices.begin(); it!=vertices.end(); it++) {
        d.push_back(it->second);
    }
    return d;
}

MedialGraph::MedialGraph(const Voronoi &v, const std::vector<Point>& shape) {
    for (Voronoi::Edge_iterator it=v.edges_begin(); it!=v.edges_end(); it++) {
        if (it->is_segment()) {
            Point src = it->source()->point();
            Point trg = it->target()->point();
            if (inside(src, shape)&&
                inside(trg, shape)) {
                add(src, trg);
                std::cout << it->source()->point() << " " << it->target()->point() << std::endl;
            }
        }
    }
}

void MedialGraph::add(const Point& p, const Point& q) {
    if (vertices[p].getLocation() != p)
        vertices[p] = Vertex(p);
    if (vertices[q].getLocation() != q)
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
        v.push_back(vertex.getLocation());
        for (Neighbors::iterator it=vertex.getNeighbors().begin(); it!=vertex.getNeighbors().end(); it++) {
            relax(vertex.getLocation(), it->first);
        }
    }
    return v;
}