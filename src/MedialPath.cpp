#include "MedialPath.h"

void MedialPath::CalculateNext() {
    MedialPoint *safe = new MedialPoint(Point(0, 0));
    safe->setEDF(-1);
    MedialPoint *temp = safe;
    for (MedialPoint *medialPoint : path) {
        for (MedialPoint *options : medialPoint->neighbors()) {
            if (!options->checkInEMA() && options->getEDF() > temp->getEDF()) {
                temp = options;
            }
        }
    }
    if (temp == safe)
        next = nullptr;
    else
        next = temp;
    delete safe;
}

MedialPath::MedialPath(MedialPoint *mp)
        : center(mp), walker(mp) {
    mp->putInEMA();
    next = center->neighbors()[0];
    path.push_back(center);
    CalculateNext();
}

void MedialPath::addBranch() {
    if (next == nullptr) return;
    walker = next;
    path.push_back(walker);
    walker->putInEMA();
    MedialPoint *nextInBranch;
    while (walker->neighbors().size() != 1) { // while not on boundary point
        MedialPoint *safe = new MedialPoint(Point(0, 0));
        safe->setEDF(-1);
        nextInBranch = safe;
        for (MedialPoint *neigh : walker->neighbors()) {
            if (!neigh->checkInEMA() && neigh->getEDF() > nextInBranch->getEDF()) {
                nextInBranch = neigh;
            }
        }
        walker = nextInBranch;
        walker->putInEMA();
        path.push_back(walker);
        delete safe;
    }
    CalculateNext();
}

std::vector<MedialPoint *> MedialPath::Path() const { return path; }