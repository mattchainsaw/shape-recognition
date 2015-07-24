#ifndef MEDIALPATH_H
#define MEDIALPATH_H

#include "MedialPoint.h"

class MedialPath {

private:
    // center will point to the Medial Point with the highest EDF and will be immutable once assigned in constructor
    MedialPoint *center;
    // walker will be the current point we are looking at when making branches
    MedialPoint *walker;
    // next will keep track of the next branch to add if we decide to add more
    MedialPoint *next;
    // path will be the set of points in the path
    std::vector<MedialPoint *> path;

    void CalculateNext();

public:
    // constructor
    // assigns mp as center and finds next point
    MedialPath(MedialPoint *mp);

    // adds a branch to the path
    // branch is decided by highest EDF of points that have not yet been connected
    void addBranch();

    // returns the path
    std::vector<MedialPoint *> Path() const;
};

// output operator
static std::ostream &operator<<(std::ostream &out, const MedialPath &medial) {
    std::vector<MedialPoint *> mp = medial.Path();
    out << mp[0]->getPoint() << " " << mp[1]->getPoint() << std::endl;
    for (int i=1; i< mp.size()-1; i++) {
        if (mp[i]->neighbors().size() == 1) { // end of branch
            MedialPoint* nextRoot = mp[i+1]->neighbors()[0];
            for (MedialPoint* neigh : mp[i+1]->neighbors()) {
                if (neigh->checkInEMA() && neigh->getEDF() > nextRoot->getEDF())
                    nextRoot = neigh;
            }
            out << nextRoot->getPoint() << " " << mp[i+1]->getPoint() << std::endl;

        }
        else
            out << mp[i]->getPoint() << " " << mp[i+1]->getPoint() << std::endl;
    }
    return out;
}

#endif // MEDIALPATH_H