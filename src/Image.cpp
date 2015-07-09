#include "Image.h"

int Image::cross(const Point &O, const Point &A, const Point &B) {
    return (A.x() - O.x()) * (B.y() - O.y()) - (A.y() - O.y()) * (B.x() - O.x());
}

Points Image::convex_hull(Points P) {
    int k = 0;
    unsigned long n = P.size();
    Points Hull(2*n);
    std::sort(P.begin(), P.end());
    for (int i = 0; i < n; ++i) {
        while (k >= 2 && cross(Hull[k-2], Hull[k-1], P[i]) <= 0)
            k--;
        Hull[k++] = P[i];
    }
    for (int i = n-2, t = k+1; i >= 0; i--) {
        while (k >= t && cross(Hull[k-2], Hull[k-1], P[i]) <= 0)
            k--;
        Hull[k++] = P[i];
    }
    Hull.resize(k);
    return Hull;
}

Points Image::polygon() {
    Points p;
    for (int r=0; r<rows; r++) {
        for (int c=0; c<cols; c++) {
            if (shape[r][c])
                p.push_back(Point(r,c));
        }
    }
    return convex_hull(p);
}

std::ostream& operator<<(std::ostream& out, const Image& im) {
    BV s = im.getBV();
    for (int r=0; r<im.Rows(); ) {
        for (int c=0; c<im.Cols(); c++) {
            out << (s[r][c] ? "1":"0");
        }
        if (++r != im.Rows())
            out << std::endl;
    }
    return out;
}

std::istream& operator>>(std::istream& in, Image& im) {
    int R,C;
    BV s;
    in >> R >> C;
    im.setRows(R);
    im.setCols(C);
    int t;
    for (int r=0; r<R; r++) {
        std::vector<bool> vb;
        for (int c=0; c<C; c++) {
            in >> t;
            vb.push_back(t==1);
        }
        s.push_back(vb);
    }
    im.setBV(s,R,C);
    return in;
}