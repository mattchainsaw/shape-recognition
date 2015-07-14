#ifndef IMAGE_H
#define IMAGE_H

#include <vector>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>

// using exact predicates and inexact constructor as that is what other files
// use. Could probably be changed to int.
typedef CGAL::Exact_predicates_inexact_constructions_kernel::Point_2 Point;
typedef std::vector<std::vector<bool> > BV;
typedef std::vector<Point> Points;

// Class for turning Matlab binary images into polygons for medial axis
class Image {
private:
    BV shape; // BW Image
    int rows;
    int cols;

    int cross(const Point &O, const Point &A, const Point &B);

    Points convex_hull(Points P);

public:
    Image() { }

    Image(const BV &s, const int &r, const int &c)
            : shape(s), rows(r), cols(c) { }

    BV getBV() const { return shape; }

    void setBV(const BV &s, const int &r, const int &c) {
        shape = s;
        rows = r;
        cols = c;
    }

    int Rows() const { return rows; }

    int Cols() const { return cols; }

    void setRows(const int &r) { rows = r; }

    void setCols(const int &c) { cols = c; }

    // makes a shape into a polygon
    // BV is just a binary image of 0's and 1's
    // polygon() will give a polygon as a list of connected points
    Points polygon();

};

#endif // IMAGE_H
