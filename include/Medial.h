#ifndef MEDIAL_H
#define MEDIAL_H

// Turn on/off debugging logs
#define DEBUG

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

#ifdef DEBUG
#include <iostream>
#endif


class Medial {
private:

    std::vector<Point> boundary;
    MedialPoint* focus;
    std::vector<MedialPoint*> safekeeping;

    bool inside(const Point& p, const std::vector<Point>& shape) {
        return CGAL::bounded_side_2(shape.begin(), shape.end(), p, Kernel()) == CGAL::ON_BOUNDED_SIDE;
    }

    void calculateRadius(MedialPoint* mp) {
        double min = std::numeric_limits<double>::max();
        double dist,x,y;
        for (std::vector<Point>::iterator it=boundary.begin(); it!=boundary.end(); it++) {
            x = it->x() - mp->getPoint().x();
            y = it->y() - mp->getPoint().y();
            dist = std::sqrt(x*x + y*y);
            if (dist < min) min = dist;
        }
#ifdef DEBUG
        std::cout << "Point " << mp->getPoint() << " had radius of " << dist << std::endl;
#endif
        mp->setRadius(dist);
    }

    void add(const Point& p, const Point& q) {
        MedialPoint* mp1 = nullptr;
        MedialPoint *mp2 = nullptr;
        for (int i=0; i<safekeeping.size(); i++) {
            if (safekeeping[i]->getPoint() == p) mp1 = safekeeping[i];
            if (safekeeping[i]->getPoint() == q) mp2 = safekeeping[i];
        }
        if (mp1 == nullptr) {
            mp1 = new MedialPoint(p);
            safekeeping.push_back(mp1);
#ifdef DEBUG
            std::cout << "made a new MedialPoint: " << mp1->getPoint() << std::endl;
#endif
        }
        if (mp2 == nullptr) {
            mp2 = new MedialPoint(q);
            safekeeping.push_back(mp2);
#ifdef DEBUG
            std::cout << "made a new MedialPoint: " << mp2->getPoint() << std::endl;
#endif
        }
        mp1->addNeighbor(mp2);
        mp2->addNeighbor(mp1);
        calculateRadius(mp1);
        calculateRadius(mp2);
        focus = mp1;
    }
    void rid() {
        for (int i=0; i<safekeeping.size(); i++) {
            MedialPoint *m = safekeeping.back();
            safekeeping.pop_back();
            delete m;
        }
    }

public:
    Medial(const Voronoi &v, const std::vector<Point>& shape) {
        boundary = shape;
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

    ~Medial() {
        rid();
    }

    std::vector<MedialPoint*> getPoints() const {
        return safekeeping;
    }

    double CalculateEDF() {
        MedialPoint* mp = focus;
        if (mp->neighbors().size() == 1) {
#ifdef DEBUG
            std::cout << "Only one point" << std::endl;
#endif
            return 0.0;
        }
        return CalculateEDF(mp);
    }

    double CalculateEDF(MedialPoint* mp) {
        mp->beenAsked = true;
        if (mp->neighbors().size() == 1) { // then we are on the outside
            mp->setEDF(0);
        }
        else { // somewhere inside
            double d(0);
#ifdef DEBUG
            std::cout << "Point " << mp->getPoint() << " has " << mp->neighbors().size() << "neighbors" << std::endl;
#endif
            for (int i=0; i<mp->neighbors().size(); i++) {
                if (!mp->neighbors()[i].first->beenAsked) {
                    double t = CalculateEDF(mp->neighbors()[i].first) + mp->neighbors()[i].second.squared_length();
                    if (d < t) d = t;
                }
            }
            mp->setEDF(d);
        }
#ifdef DEBUG
        std::cout << mp->getPoint() << " has an EDF of " << mp->getEDF() << std::endl;
#endif
        return mp->getEDF();
    }
};


std::ostream& operator<<(std::ostream& out, const Medial& medial) {
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
