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

void MedialGraph::initialize() {
    for (Vertices::iterator it=vertices.begin(); it!=vertices.end(); it++) {
        it->second.setDist((it->second.getLocation() == head) ? 0:INF);
        std::cout << (head==(it->second.getLocation())) << std::endl;
        it->second.setParent(Point(INF,INF));
    }
}

void MedialGraph::relax(Vertex& u, Vertex& v) {
    if (v.getDist() > (u.getDist() + pointDistance(u.getLocation(),v.getLocation()))) {
        std::cout << v.getDist() << " > " << u.getDist() << " + " << pointDistance(u.getLocation(), v.getLocation()) << std::endl;
        v.setDist(u.getDist() + pointDistance(u.getLocation(),v.getLocation()));
        v.setParent(u.getLocation());
        u.addNeighbor(v.getLocation());
    }
    else {
        std::cout << v.getDist() << " <= " << u.getDist() << " + " << pointDistance(u.getLocation(), v.getLocation()) << std::endl;
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
    Point min(0,0);
    Voronoi::Edge_iterator it;
    for (it = v.edges_begin(); it!=v.edges_end(); it++) {
        if (it->is_segment() && min > it->source()->point() && inside(it->source()->point(),shape))
            min =it->source()->point();
        if (it->is_segment() && min > it->target()->point() && inside(it->source()->point(),shape))
            min =it->target()->point();
    }
    CGAL::Vector_2<Kernel> MINX = min.x() < 0 ? CGAL::Vector_2<Kernel>(-min.x(), 0) : CGAL::Vector_2<Kernel>(0, 0);
    CGAL::Vector_2<Kernel> MINY = min.y() < 0 ? CGAL::Vector_2<Kernel>(0, -min.y()) : CGAL::Vector_2<Kernel>(0, 0);
    if (min.y() < 0) min = Point(min.x(),(-min.y()));
    for (it=v.edges_begin(); it!=v.edges_end(); it++) {
        if (it->is_segment()) {
            Point src = it->source()->point();
            Point trg = it->target()->point();
            if (inside(src, shape)&&
                inside(trg, shape)) {
                src = src + MINX + MINY;
                trg = trg + MINX + MINY;
                head = src;
                add(src, trg);
                //if () std::cout << it->source()->point() << " " << it->target()->point() << std::endl;
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
    vertices[q].setParent(p);
}

std::vector<Vertex> MedialGraph::dijkstra() {
    initialize();
    std::vector<Vertex> v;
    std::vector<Vertex> heap = getVertices();
    std::make_heap (heap.begin(), heap.end(), compare());
    while (!heap.empty()) {
        std::pop_heap(heap.begin(), heap.end());
        Vertex vertex = heap.back();
        heap.pop_back();
        std::push_heap(heap.begin(), heap.end(), compare());
        for (Neighbors::iterator it=vertex.getNeighbors().begin(); it!=vertex.getNeighbors().end(); it++) {
            Point point = it->first;
            relax(vertex,vertices[point]);
        }
        v.push_back(vertex.getLocation());
    }
    return v;
}