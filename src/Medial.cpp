#include "Medial.h"
#include <queue>

std::vector<MedialPoint *>::iterator ready(std::vector<::MedialPoint *> &pts) {
    std::vector<MedialPoint *>::iterator looper;
    std::vector<MedialPoint *>::iterator m = pts.begin();
    double d(std::numeric_limits<double>::max());
    double D = d;
    if (pts.size() > 1) {
        for (looper = pts.begin(); looper != pts.end(); looper++) {
            int n(0);
            for (MedialPoint *mp : (*looper)->neighbors()) {
                if (!mp->done) {
                    ++n;
                }
                if (d > mp->getEDF() + mp->dist(*looper))
                    d = mp->getEDF() + mp->dist(*looper);
            }
            if (n < 2 && D > d) {
                m = looper;
                D = d;
            }
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
    MedialPoint *mp1 = nullptr;
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


bool medGT(MedialPoint *i, MedialPoint *j) { return i->getEDF() > j->getEDF(); }
void Medial::CalculateEDF() {

    std::vector<MedialPoint *> H;

    for (MedialPoint *medialPoint : safekeeping) {
        medialPoint->neighborsDone = 0;
        if (medialPoint->neighbors().size() == 1) { // boundary points
            medialPoint->setEDF(medialPoint->getRadius());
            medialPoint->done = true;
            H.push_back(medialPoint);
        }
        else {
            medialPoint->setEDF(0);
            medialPoint->done = false;
        }
    }

    MedialPoint *walker;
    while (!H.empty()) {
        std::sort(H.begin(),H.end(),medGT);
        for (int i=0; i < H.size(); i++)
            std::cerr << H[i]->getEDF() << ' ';
        std::cerr << std::endl;

        walker = H.back();
        H.pop_back();

        for (MedialPoint *neigh : walker->neighbors()) {
            if (!neigh->done) {
                double edf = walker->getEDF() + walker->dist(neigh);
                if (edf > neigh->getEDF()) neigh->setEDF(edf);
                ++neigh->neighborsDone;
                if (neigh->neighborsDone+1 == neigh->neighbors().size()) {
                    H.push_back(neigh);
                    neigh->done = true;
                    ++walker->neighborsDone;
                }
            }
        }
    }
    setCenter();
}

// private member focus is the point with highest EDF
// from focus find neighbors with highest EDF's and include these points
MedialPath Medial::Prune(const unsigned int& branches) {
    MedialPath path(focus);
    for (unsigned int i=0; i<branches; i++) {
        path.addBranch();
    }
    return path;
}
