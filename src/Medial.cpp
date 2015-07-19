#include "Medial.h"
#include <queue>

// Accuracy is the number of midpoints inserted into original boundary.
// If 0, then no change will be made
// otherwise X midpoints will be inserted into boundary
#define ACCURACY 1

struct compa {
    bool operator()(MedialPoint* mp1, MedialPoint* mp2) const {
        return mp1->getEDF() > mp2->getEDF();
    }
};

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
        if (dist < min) min = dist;
    }
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
    }
    if (mp2 == nullptr) {
        mp2 = new MedialPoint(q);
        safekeeping.push_back(mp2);
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

// 3 types of points
// Boundary = points on the boundary with EDF = their radius
// connect  = points that connect two others
// branch   = points that are connected to more than two others
void Medial::CalculateEDF() {
    std::vector<MedialPoint *> points;

    // Calculate EDF for boundary points
    // Their EDF is their radius and is done being computed
    // They are pushed onto vector points
    for (MedialPoint *medialPoint : safekeeping) {
        if (medialPoint->neighbors().size() == 1) { // boundary points
            medialPoint->setEDF(medialPoint->getRadius());
            points.push_back(medialPoint);
        }
    }
    // While there are still points in the vector points, take out the medial points and try to compute EDF's for inner
    // points. Once the inner points only have one neighbor left, they are pushed onto the vector to help compute
    // other inner points
    while (!points.empty()) {
        // sorts so smalled EDF is in back()
        std::sort(points.begin(), points.end(), compa());
        MedialPoint *medialPoint = points.back();
        points.pop_back();
        for (int i = 0; i < medialPoint->neighbors().size(); i++) {
            double EDF = medialPoint->neighbors()[i]->dist(medialPoint) + medialPoint->getEDF();
            if (EDF < medialPoint->neighbors()[i]->getEDF()) {
                medialPoint->neighbors()[i]->setEDF(EDF);
                points.push_back(medialPoint->neighbors()[i]);
            }
        }
        focus = medialPoint; // eventually center
    }
}


// private member focus is the point with highest EDF
// from focus find neighbors with highest EDF's and include these points
MedialPath Medial::Prune(const int& branches) {
    MedialPath path(focus);
    for (int i=0; i<branches;  i++)
        path.addBranch();
    return path;
}