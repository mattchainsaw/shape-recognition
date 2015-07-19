#ifndef MEDIALPATH_H
#define MEDIALPATH_H

#include "MedialPoint.h"

class MedialPath {

private:
    // center will point to the Medial Point with the highest EDF and will be immutable once assigned in constructor
    MedialPoint* center;
    // walker will be the current point we are looking at when making branches
    MedialPoint* walker;
    // next will keep track of the next branch to add if we decide to add more
    MedialPoint* next;
    // path will be the set of points in the path
    std::vector<MedialPoint*> path;

    void CalculateNext();

public:
    // constructor
    // assigns mp as center and finds next point
    MedialPath(MedialPoint* mp);
    // adds a branch to the path
    // branch is decided by highest EDF of points that have not yet been connected
    void addBranch();
    // returns the path
    std::vector<MedialPoint*> Path() const;
};

// output operator
static std::ostream &operator<<(std::ostream &out, const MedialPath &medial) {
    std::vector<MedialPoint *> mp = medial.Path();
    for (MedialPoint* M : mp) {
        for (MedialPoint *P : M->neighbors()) {
            if (P->checkInEMA()) {
                out << M->getPoint() << " " << P->getPoint() << std::endl;
            }
        }
    }
    return out;
}

#endif // MEDIALPATH_H