#include "Medial.h"
#include <queue>

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

// need to change from recursive to iterative and change return to void
// finds points with only one neighbor. These are the end points of the medial axis.
// Push ends to heap where compare is active neighbor size. if size is 1, then compute and update
// 3 types of points
// Boundary = points on the boundary with EDF = their radius
// connect  = points that connect two others
// branch   = points that are connected to more than two others
void Medial::CalculateEDF() {
    std::vector<MedialPoint*> points;
    std::vector<MedialPoint*> nextLayer;

    // Calculate EDF for boundary points
    // Their EDF is their radius and is done being computed
    // They are pushed onto vector points
    for (MedialPoint* medialPoint : safekeeping) {
        if (medialPoint->N == 1) { // boundary points
            medialPoint->setEDF(medialPoint->getRadius());
            medialPoint->EDFisDone = true;
            points.push_back(medialPoint);
        }
    }
    // While there are still points in the vector points, take out the medial points and try to compute EDF's for inner
    // points. Once the inner points only have one neighbor left, they are pushed onto the vector to help compute
    // other inner points
    while (!points.empty()) {
        for (MedialPoint* medialPoint : points) {
            std::vector<MedialPoint*> children = medialPoint->notDone(); // <-- only element left to be computed in neighbors
            for (MedialPoint* child : children) {
                // Try to find EDF for child
                child->N -= 1;
                double EDF = child->dist(medialPoint) + medialPoint->getEDF();
                if (EDF > child->getEDF()) {
                    child->setEDF(EDF);
                    child->setParent(medialPoint);
                }
                // check to see is child is ready to grow up and live in the vector, only to be removed one iteration later
                if (child->N == 1) {
                    if (!child->EDFisDone) {
                        child->EDFisDone = true;
                        nextLayer.push_back(child);
                    }
                }
                if (child->N == 0)  // Center
                    focus = child;
            }
        }
        // reset points and transfer over the new points to look at
        points.clear();
        MedialPoint* temp;
        while (!nextLayer.empty()) {
            temp = nextLayer.back();
            nextLayer.pop_back();
            points.push_back(temp);
        }
    }
}

double Medial::CalculateEDF(MedialPoint* mp) {
    if (mp->EDFisDone == false) {
        mp->EDFisDone = true;
        double EDF(0);
        // connected points
        if (mp->neighbors().size() == 2) {
            double option1 = CalculateEDF(mp->neighbors().front().first) + mp->dist(mp->neighbors().front().first);
            double option2 = CalculateEDF(mp->neighbors().back().first) + mp->dist(mp->neighbors().back().first);
            EDF = (option1 < option2) ? option1 : option2;
        }
        // branch points
        else {
            double max = std::numeric_limits<double>::min();
            for (std::pair<MedialPoint *, Arrow> neigh : mp->neighbors()) {
                double temp = CalculateEDF(neigh.first) + mp->dist(neigh.first);
                if (temp > max) {
                    EDF = max;
                    max = temp;
                }
                //if (temp > EDF) EDF = temp;
            }
        }
        mp->setEDF(EDF);
    }
    return mp->getEDF();
}

/*void Medial::CalculateEDF() {
    std::deque<MedialPoint*> EDFPoints;

    // Calculate EDF for boundary points and push them onto deque
    for (MedialPoint* medialPoint : safekeeping) {
        if (medialPoint->neighbors().size() == 1) { // boundary points
            medialPoint->setEDF(medialPoint->getRadius());
            EDFPoints.push_back(medialPoint);
        }
    }

    // Calculate EDF for connect and branch points
    while (!EDFPoints.empty()) {
        MedialPoint* medial = EDFPoints.front();
        EDFPoints.pop_front();
        medial->EDFisDone = true;
        MedialPoint* child;
        if (medial->neighbors().size() == 1) { // boundary points
            child = medial->neighbors().front().first;
            double dist = child->edf(medial);
            // EDF already calculated, move on and give parent
            if (child->getParent() == nullptr) {
                child->setParent(medial);
                child->setEDF(dist);
            }
            else {
                if (dist > child->getEDF()) {
                    child->setEDF(dist);
                    child->setParent(medial);
                }
            }
            child->EDFisDone = true;
            EDFPoints.push_back(child);
        }

        else if (medial->neighbors().size() == 2) { // connect points and branch points not yet reached
            // EDF already calculated, find child
            if (medial->neighbors().front().first == medial->getParent())
                child = medial->neighbors().back().first;
            else
                child = medial->neighbors().front().first;
            double EDF = child->edf(medial);
            if (EDF < child->getEDF()) {
                child->setParent(medial);
                child->setEDF(child->edf(medial));
                child->EDFisDone = true;
                EDFPoints.push_back(child);
            }
        }

        else { // branch point
            int branches = (int)medial->neighbors().size();
            for (auto neigh : medial->neighbors()) {
                if (neigh.first->EDFisDone) --branches;
                else child = neigh.first;
            }
            if (branches == 0) // reached the center
                focus = medial;
            else if (branches == 1) {
                // we found the right child
                child->setParent(medial);
                double EDF = child->edf(medial);
                if (EDF > child->getEDF())
                    child->setEDF(EDF);
                child->EDFisDone = true;
                EDFPoints.push_back(child);
            }
            else { // not done with other branches yet, so wait, but see if edf needs updating
                for (auto neigh : medial->neighbors()) {
                    if (!neigh.first->EDFisDone) {
                        child = neigh.first;
                        double EDF = child->edf(medial);
                        if (EDF > child->getEDF()) {
                            child->setParent(medial);
                            child->setEDF(EDF);
                        }
                    }
                }

            }
        }
    }
}*/

// private member center is the point with highest EDF
// from center find neighbors with highest EDF's and include these points
// void Medial::Prune(const int& branches = 2)
