#include "MedialPoint.h"

MedialPoint::MedialPoint(const Point& point)
        : location(point), EDF(-1.0), EDFisDone(false), N(0) {}

// Getters and Setters
Point MedialPoint::getPoint() const { return location; }
std::vector<std::pair<MedialPoint*, Arrow> > MedialPoint::neighbors() const { return connected; }
double MedialPoint::getEDF() const { return EDF; }
void MedialPoint::setEDF(const double& edf) { EDF = edf; }
double MedialPoint::getRadius() const { return radius; }
void MedialPoint::setRadius(const double& r) { radius = r; }
MedialPoint* MedialPoint::getParent() const { return parent; }
void MedialPoint::setParent(MedialPoint* mp) { parent = mp; }

// Adds a neighbor to the medialpoint
void MedialPoint::addNeighbor(MedialPoint* mp) {
    Point neighLoc = mp->getPoint();
    Arrow arrow(neighLoc,location);
    std::pair<MedialPoint*, Arrow> newNeigh = std::make_pair(mp, arrow);
    connected.push_back(newNeigh);
    ++N;
}

// Gives all neighbors who have not recieved an EDF yet
std::vector<MedialPoint*> MedialPoint::notDone() {
    std::vector<MedialPoint*> ND;
    for (int i=0; i<connected.size(); i++) {
        if (connected[i].first->EDF == -1)
            ND.push_back(connected[i].first);
    }
    return ND;
}

// compute distance between two medialpoints
double MedialPoint::dist(MedialPoint* mp) {
    double x = location.x() - mp->getPoint().x();
    double y = location.y() - mp->getPoint().y();
    return std::sqrt(x*x + y*y);
}