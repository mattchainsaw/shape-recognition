#ifndef MEDIALPOINT_H
#define MEDIALPOINT_H

#include <vector>
#include <limits>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2                                     Point;
typedef Kernel::Vector_2                                    Arrow;

class MedialPoint {
private:
    Point location;
    std::vector<std::pair<MedialPoint*, Arrow> > connected;
    int neighborSIZE;
    double EDF;
    double radius;
    MedialPoint* parent;

public:
    bool readyToDie;
    bool beenAsked;
    MedialPoint(const Point& point)
            : location(point), neighborSIZE(0), EDF(-1.0), readyToDie(false), beenAsked(false) {}
    Point getPoint() const {return location;}
    void setPoint(const Point& p) {location = p;}
    std::vector<std::pair<MedialPoint*, Arrow> > neighbors() const {return connected;}
    void addNeighbor(MedialPoint* mp) {
        Point neighLoc = mp->getPoint();
        Arrow arrow(neighLoc,location);
        std::pair<MedialPoint*, Arrow> newNeigh = std::make_pair(mp, arrow);
        connected.push_back(newNeigh);
        ++neighborSIZE;
    }
    void removeNeighbor(MedialPoint* mp) {
        for (auto it = connected.begin(); it != connected.end(); it++) {
            if (it->first == mp)
                connected.erase(it);
        }
        delete mp;
    }
    int getNeighborSize() const {return neighborSIZE;}
    void resetNeighborSize() {neighborSIZE = connected.size();}
    double getEDF() const {return EDF;}
    void   setEDF(const double& edf) {EDF = edf;}
    double getRadius() const {return radius;}
    void   setRadius(const double& r) {radius = r;}
    MedialPoint* getParent() const {return parent;}
    void setParent(MedialPoint* mp) {parent = mp;}

};

#endif // MEDIALPOINT_H