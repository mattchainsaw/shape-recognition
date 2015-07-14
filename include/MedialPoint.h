#ifndef MEDIALPOINT_H
#define MEDIALPOINT_H

#include <vector>
#include <cmath>
#include <limits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2                                     Point;
typedef Kernel::Vector_2                                    Arrow;

class MedialPoint {
private:
    // The coordinates of the point
    Point location;
    // all of the points connecting neighbors
    std::vector<std::pair<MedialPoint*, Arrow> > connected;
    // the points EDF
    double EDF;
    // the points radius
    double radius;
    // the points parent with respect to where it calculated its EDF
    MedialPoint* parent;

public:
    // Aux info
    bool EDFisDone;
    int N;
    // Constructor
    MedialPoint(const Point& point);
    // Getters and Setters
    Point getPoint() const;
    std::vector<std::pair<MedialPoint*, Arrow> > neighbors() const;
    double getEDF() const;
    void   setEDF(const double& edf);
    double getRadius() const;
    void   setRadius(const double& r);
    MedialPoint* getParent() const;
    void setParent(MedialPoint* mp);

    // Adds another medial point to the connectivity neighbors
    void addNeighbor(MedialPoint* mp);
    // returns a vector of medialPoints that have not yet calculated their EDF
    std::vector<MedialPoint*> notDone();
    // calculates the distance between two medial points
    double dist(MedialPoint* mp);
};
#endif // MEDIALPOINT_H