#include "Medial.h"
#include <queue>

struct compa {
    bool operator()(MedialPoint* mp1, MedialPoint* mp2) const {
        return mp1->getEDF() > mp2->getEDF();
    }
};

bool Medial::inside(const Point& p, const std::vector<Point>& shape) {
    return CGAL::bounded_side_2(shape.begin(), shape.end(), p, Kernel()) == CGAL::ON_BOUNDED_SIDE;
}

void addAccuracy(std::vector<Point>& shape, const unsigned int& acc) {
    if (acc > 0) {
        double x,y;
        for (int j=0; j<shape.size(); j+=1+acc) {
            Point from = shape[j];
            Point to = shape[(j+1) % shape.size()];
            for (int i=0; i<acc; i++) {
                x = (i+1)*(to.x() + from.x())/(acc+1);
                y = (i+1)*(to.y() + from.y())/(acc+1);
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

Medial::Medial(std::vector<Point> &shape, const unsigned int& acc) {
    boundary = shape;
    addAccuracy(boundary, acc);
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
    // pop the point with smallest edf and compute neighbors edf's until empty
    while (!points.empty()) {
        // sorts so smalled EDF is in back()
        std::vector<MedialPoint*>::iterator medIter = std::min(points.begin(), points.end());
        MedialPoint* medialPoint = *medIter;
        for (int i = 0; i < medialPoint->neighbors().size(); i++) {
            double EDF = medialPoint->neighbors()[i]->dist(medialPoint) + medialPoint->getEDF();
            if (EDF < medialPoint->neighbors()[i]->getEDF()) {
                medialPoint->neighbors()[i]->setEDF(EDF);
                // if point is already in vector, erase it and push back
                std::vector<MedialPoint*>::iterator iter = std::find(points.begin(), points.end(), medialPoint->neighbors()[i]);
                if (iter != points.end()) points.erase(iter);
                points.push_back(medialPoint->neighbors()[i]);
            }
        }
        points.erase(medIter);
        focus = medialPoint; // eventually center
    }
}


// private member focus is the point with highest EDF
// from focus find neighbors with highest EDF's and include these points
MedialPath Medial::Prune(const unsigned int& branches) {
    MedialPath path(focus);
    for (unsigned int i=0; i<branches;  i++)
        path.addBranch();
    return path;
}