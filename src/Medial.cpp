#include "Medial.h"
#include <queue>

std::vector<MedialPoint *>::iterator ready(std::vector<::MedialPoint *> &pts) {
    std::vector<MedialPoint *>::iterator looper;
    std::vector<MedialPoint *>::iterator m = pts.begin();
    double d(std::numeric_limits<double>::max());
    double D = d;
    for (looper = pts.begin(); looper != pts.end(); looper++) {
        int n(0);
        for (MedialPoint *mp : (*looper)->neighbors()) {
            if (!mp->done) {
                ++n;
            }
            else
                if (d > mp->getEDF() + mp->dist(*looper)) d = mp->getEDF() + mp->dist(*looper);
        }
        if (n == 1 && D > d) {
            m = looper;
            D = d;
        }
    }
    return m;
}

bool Medial::inside(const Point& p, const std::vector<Point>& shape) {
    return CGAL::bounded_side_2(shape.begin(), shape.end(), p, Kernel()) == CGAL::ON_BOUNDED_SIDE;
}

void addAccuracy(std::vector<Point>& shape) {
    double x,y;
    for (int j=0; j<shape.size(); j+=2) {
        Point from = shape[j];
        Point to = shape[(j+1) % shape.size()];
        Point mid = CGAL::midpoint(to,from);
        shape.emplace(shape.begin() + j, mid);
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

void Medial::setCenter() {
   for (int i=0; i<safekeeping.size(); i++) {
        if (focus->getEDF() < safekeeping[i]->getEDF())
            focus = safekeeping[i];
    }
}

Medial::Medial(std::vector<Point> &shape, const bool &acc) {
    boundary = shape;
    if (acc) addAccuracy(boundary);
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
    // Calculate EDF for boundary points
    // Their EDF is their radius and is done being computed
    // They are pushed onto vector points
    for (MedialPoint *medialPoint : safekeeping) {
        if (medialPoint->neighbors().size() == 1) { // boundary points
            medialPoint->setEDF(medialPoint->getRadius());
            medialPoint->done = true;
        }
    }

    // copy safekeeping so we can get rid of points without ruining safekeeping
    std::vector<MedialPoint *> points = safekeeping;
    std::vector<MedialPoint *>::iterator it;
    for (it = points.begin(); it != points.end(); ) {
        if ((*it)->done) it = points.erase(it);
        else ++it;
    }
    while (!points.empty()) {
        it = ready(points);
        double EDF(0);
        for (MedialPoint *mp : (*it)->neighbors()) {
            if (mp->done) {
                double temp = mp->dist(*it) + mp->getEDF();
                if (temp > EDF) EDF = temp;
            }
        }
        (*it)->setEDF(EDF);
        (*it)->done = true;
        it = points.erase(it);
    }
    focus = (*it);
/*
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
        std::vector<MedialPoint*>::iterator medIter = std::min(points.begin(), points.end());
        MedialPoint* medialPoint = *medIter;
        medialPoint->done = true;

        int active(0);
        for (MedialPoint *mp : medialPoint->neighbors())
            if (!mp->done) ++active;

        if (active == 1)

        for (int i = 0; i < medialPoint->neighbors().size(); i++) {
            double EDF = medialPoint->neighbors()[i]->dist(medialPoint) + medialPoint->getEDF();
            if (medialPoint->neighbors()[i]->getEDF() == std::numeric_limits<double>::max()
                || (!medialPoint->neighbors()[i]->done && EDF > medialPoint->neighbors()[i]->getEDF())) {
                medialPoint->neighbors()[i]->setEDF(EDF);
                // if point is already in vector, erase it and push back
                std::vector<MedialPoint*>::iterator iter = std::find(points.begin(), points.end(), medialPoint->neighbors()[i]);
                if (iter != points.end()) points.erase(iter);
                points.push_back(medialPoint->neighbors()[i]);
            }
        }
        points.erase(medIter);
    }
    */
    //setCenter();
}


// private member focus is the point with highest EDF
// from focus find neighbors with highest EDF's and include these points
MedialPath Medial::Prune(const unsigned int& branches) {
    MedialPath path(focus);
    for (unsigned int i=0; i<branches; i++) {
        path.addBranch();
        std::cout << i << std::endl;
    }
    return path;
}