#ifndef MEDIAL_H
#define MEDIAL_H

#include "MedialPoint.h"
#include "MedialPath.h"
#include <vector>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef Kernel::Point_2 Point;
typedef CGAL::Delaunay_triangulation_2<Kernel> Delaunay;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<Delaunay> AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<Delaunay> AP;
typedef CGAL::Voronoi_diagram_2<Delaunay, AT, AP> Voronoi;

class Medial {
private:

    // Boundary of points in order.
    std::vector<Point> boundary;
    // after EDF is computed the focus is pointing to the center.
    MedialPoint *focus;
    // All of the medial axis points together for easy access and delete
    std::vector<MedialPoint *> safekeeping;

    // Test to see whether of not a point is inside the polygon boundary
    // Used in computing voronoi
    bool inside(const Point &p, const std::vector<Point> &shape);

    // Calculates the radius of a medial point inside of the polygon
    void calculateRadius(MedialPoint *mp);

    // Add a pair of points that resemble a line segment into the medial points
    // If the points are already in the medial points, then there is just an update in connectivity
    // The radius is also computed in this scope
    void add(const Point &p, const Point &q);

    // helper for destructor
    void rid();

public:
    // Constructor
    Medial(std::vector<Point> &shape, const unsigned int &acc);

    // Destructor
    ~Medial();

    // Returns a vector of pointers to all of the medial points
    std::vector<MedialPoint *> getPoints() const;

    // Calculates the EDF for all point in the object.
    void CalculateEDF();

    MedialPath Prune(const unsigned int &branches = 2);

};

// output operator
static std::ostream &operator<<(std::ostream &out, const Medial &medial) {
    std::vector<MedialPoint *> mp = medial.getPoints();
    for (int i = 0; i < mp.size(); i++) {
        for (int j = 0; j < mp[i]->neighbors().size(); j++) {
            out << mp[i]->getPoint() << " " << mp[i]->neighbors()[j]->getPoint() << " " <<
            mp[i]->getEDF() << " " << mp[i]->getRadius() << std::endl;
        }
    }
    return out;
}

#endif // MEDIAL_H