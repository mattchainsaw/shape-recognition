#ifndef MEDIALPOINT_H
#define MEDIALPOINT_H

#include <vector>
#include <cmath>
#include <limits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point;

class MedialPoint {
private:
    // The coordinates of the point
    Point location;
    // all of the points connecting neighbors
    std::vector<MedialPoint *> connected;
    // the points EDF
    double EDF;
    // the points radius
    double radius;
    // used for pruning function
    bool isInEMA;

public:

    bool done;

    // Constructor
    MedialPoint(const Point &point);

    // Getters and Setters
    Point getPoint() const;

    std::vector<MedialPoint *> neighbors() const;

    double getEDF() const;

    void setEDF(const double &edf);

    double getRadius() const;

    void setRadius(const double &r);

    // Adds another medial point to the connectivity neighbors
    void addNeighbor(MedialPoint *mp);

    // calculates the distance between two medial points
    double dist(MedialPoint *mp);

    void putInEMA();

    bool checkInEMA() const;
};

#endif // MEDIALPOINT_H