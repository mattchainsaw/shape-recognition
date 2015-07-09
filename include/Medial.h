#ifndef MEDIAL_H
#define MEDIAL_H

#include "MedialPoint.h"
#include <vector>
#include <cmath>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel                        Kernel;
typedef Kernel::Point_2                                                            Point;
typedef CGAL::Delaunay_triangulation_2<Kernel>                                     Delaunay;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<Delaunay>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<Delaunay> AP;
typedef CGAL::Voronoi_diagram_2<Delaunay,AT,AP>                                    Voronoi;

class Medial {
private:

    std::vector<Point> boundary;
    MedialPoint* focus;
    std::vector<MedialPoint*> safekeeping;

    bool inside(const Point& p, const std::vector<Point>& shape);
    void calculateRadius(MedialPoint* mp);
    void add(const Point& p, const Point& q);
    void rid();

public:
    Medial(const Voronoi &v, const std::vector<Point>& shape);
    ~Medial();
    std::vector<MedialPoint*> getPoints() const;
    double CalculateEDF();
    double CalculateEDF(MedialPoint* mp);
};

static std::ostream& operator<<(std::ostream& out, const Medial& medial) {
    std::vector<MedialPoint*> mp = medial.getPoints();
    for (int i=0; i<mp.size(); i++) {
        for (int j=0; j<mp[i]->neighbors().size(); j++) {
            out << mp[i]->getPoint() << " " << mp[i]->neighbors()[j].first->getPoint() << " " <<
            mp[i]->getEDF()  << " " << mp[i]->getRadius() << std::endl;
        }
    }
    return out;
}
#endif // MEDIAL_H