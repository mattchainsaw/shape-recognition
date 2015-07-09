#include "Medial.h"
#include <iostream>

// Turn debugging log on and off
#define DEBUG

bool Medial::inside(const Point& p, const std::vector<Point>& shape) {
    return CGAL::bounded_side_2(shape.begin(), shape.end(), p, Kernel()) == CGAL::ON_BOUNDED_SIDE;
}

void Medial::calculateRadius(MedialPoint* mp) {
    double min = std::numeric_limits<double>::max();
    double dist,x,y;
    for (int i=0; i<boundary.size(); i++) {
        x = boundary[i].x() - mp->getPoint().x();
        y = boundary[i].y() - mp->getPoint().y();
        dist = std::sqrt(x*x + y*y);
#ifdef DEBUG
            std::cout << "Distance between " << mp->getPoint() << " and " << boundary[i] << " is " << dist << std::endl;
#endif
        if (dist < min) min = dist;
    }
#ifdef DEBUG
        std::cout << "Point " << mp->getPoint() << " had radius of " << dist << std::endl;
#endif
    mp->setRadius(min);
}

void Medial::add(const Point& p, const Point& q) {
    MedialPoint* mp1 = nullptr;
    MedialPoint *mp2 = nullptr;
    for (int i=0; i<safekeeping.size(); i++) {
        if (safekeeping[i]->getPoint() == p) mp1 = safekeeping[i];
        if (safekeeping[i]->getPoint() == q) mp2 = safekeeping[i];
    }
    if (mp1 == nullptr) {
        mp1 = new MedialPoint(p);
        safekeeping.push_back(mp1);
#ifdef DEBUG
            std::cout << "made a new MedialPoint: " << mp1->getPoint() << std::endl;
#endif
    }
    if (mp2 == nullptr) {
        mp2 = new MedialPoint(q);
        safekeeping.push_back(mp2);
#ifdef DEBUG
            std::cout << "made a new MedialPoint: " << mp2->getPoint() << std::endl;
#endif
    }
    mp1->addNeighbor(mp2);
    mp2->addNeighbor(mp1);
    calculateRadius(mp1);
    calculateRadius(mp2);
    focus = mp1;
}
void Medial::rid() {
    for (int i=0; i<safekeeping.size(); i++) {
        MedialPoint *m = safekeeping.back();
        safekeeping.pop_back();
        delete m;
    }
}

Medial::Medial(const Voronoi &v, const std::vector<Point>& shape) {
    boundary = shape;
    Voronoi::Edge_iterator it;
    for (it=v.edges_begin(); it!=v.edges_end(); it++) {
        if (it->is_segment()) {
            Point src = it->source()->point();
            Point trg = it->target()->point();
            if (inside(src, shape)&&
                inside(trg, shape)) {
                add(src, trg);
            }
        }
    }
}

Medial::~Medial() {
    rid();
}

std::vector<MedialPoint*> Medial::getPoints() const {
    return safekeeping;
}

double Medial::CalculateEDF() {
    MedialPoint* mp = focus;
    if (mp->neighbors().size() == 1) {
#ifdef DEBUG
            std::cout << "Only one point" << std::endl;
#endif
        return 0.0;
    }
    return CalculateEDF(mp);
}

double Medial::CalculateEDF(MedialPoint* mp) {
    mp->beenAsked = true;
    if (mp->neighbors().size() == 1) { // then we are on the outside
        mp->setEDF(0);
    }
    else { // somewhere inside
        double d(0);
#ifdef DEBUG
            std::cout << "Point " << mp->getPoint() << " has " << mp->neighbors().size() << "neighbors" << std::endl;
#endif
        for (int i=0; i<mp->neighbors().size(); i++) {
            if (!mp->neighbors()[i].first->beenAsked) {
                double t = CalculateEDF(mp->neighbors()[i].first) + mp->neighbors()[i].second.squared_length();
                if (d < t) d = t;
            }
        }
        mp->setEDF(d);
    }
#ifdef DEBUG
        std::cout << mp->getPoint() << " has an EDF of " << mp->getEDF() << std::endl;
#endif
    return mp->getEDF();
}