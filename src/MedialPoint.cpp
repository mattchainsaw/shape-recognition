#include "MedialPoint.h"

MedialPoint::MedialPoint(const Point &point)
        : location(point), EDF(std::numeric_limits<double>::max()), isInEMA(false) { }

// Getters and Setters
Point MedialPoint::getPoint() const { return location; }

std::vector<MedialPoint *> MedialPoint::neighbors() const { return connected; }

double MedialPoint::getEDF() const { return EDF; }

void MedialPoint::setEDF(const double &edf) { EDF = edf; }

double MedialPoint::getRadius() const { return radius; }

void MedialPoint::setRadius(const double &r) { radius = r; }

// Adds a neighbor to the medialpoint
void MedialPoint::addNeighbor(MedialPoint *mp) {
    connected.push_back(mp);
}

// compute distance between two medial points
double MedialPoint::dist(MedialPoint *mp) {
    double x = location.x() - mp->getPoint().x();
    double y = location.y() - mp->getPoint().y();
    return std::sqrt(x * x + y * y);
}

void MedialPoint::putInEMA() { isInEMA = true; }

bool MedialPoint::checkInEMA() const { return isInEMA; }