#include "Medial.h"
#include <iostream>

// Turn debugging log on and off
//#define DEBUG

// Accuracy is the number of midpoints inserted into original boundary.
// If 0, then no change will be made
// otherwise X midpoints will be inserted into boundary
#define ACCURACY 1

bool Medial::inside(const Point& p, const std::vector<Point>& shape) {
    return CGAL::bounded_side_2(shape.begin(), shape.end(), p, Kernel()) == CGAL::ON_BOUNDED_SIDE;
}

void addAccuracy(std::vector<Point>& shape) {
    if (ACCURACY > 0) {
        double x,y;
        for (int j=0; j<shape.size(); j+=1+ACCURACY) {
            Point from = shape[j];
            Point to = shape[(j+1) % shape.size()];
            for (int i=0; i<ACCURACY; i++) {
                x = (i+1)*(to.x() + from.x())/(ACCURACY+1);
                y = (i+1)*(to.y() + from.y())/(ACCURACY+1);
                Point mid(x,y);
                shape.emplace(shape.begin() + j +  i, mid);
            }
        }
    }
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
        std::cout << "Point " << mp->getPoint() << " had radius of " << min << std::endl;
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

Medial::Medial(const std::vector<Point>& shape) {
    boundary = shape;
    addAccuracy(boundary);
    // Make voronoi diagram
    Voronoi v;
    v.insert(boundary.begin(), boundary.end());
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

// need to change from recursive to iterative and change return to void
// finds points with only one neighbor. These are the end points of the medial axis.
// Push ends to heap where compare is active neighbor size. if size is 1, then compute and update
double Medial::CalculateEDF(MedialPoint* mp) {
    mp->beenAsked = true;
    if (mp->getNeighborSize() == 1) { // then we are on the outside
        mp->setEDF(mp->getRadius());
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
            else {
                double t = mp->neighbors()[i].first->getEDF() + mp->neighbors()[i].second.squared_length();
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

// private member center is the point with highest EDF
// from center find neighbors with highest EDF's and include these points
// void Medial::Prune(const int& branches = 2)