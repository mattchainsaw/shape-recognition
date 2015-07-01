// Standard library
#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>

// CGAL
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>

#include "MedialGraph.h"

// Definitions
typedef CGAL::Exact_predicates_inexact_constructions_kernel                        Kernel;
typedef CGAL::Delaunay_triangulation_2<Kernel>                                     Delaunay;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<Delaunay>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<Delaunay> AP;
typedef CGAL::Voronoi_diagram_2<Delaunay,AT,AP>                                    Voronoi;
typedef Kernel::Point_2                                                            Point;
typedef Kernel::Triangle_2                                                         Triangle;

using namespace std;

// data input
void load(const char* str, vector<Point>& vp) {
  ifstream file(str);
  while (!file.is_open()) {
    cout << "Error finding file." << endl;
    exit (EXIT_FAILURE);
  }
  double x,y;
  while (file >> x >> y) {
    vp.push_back(Point(x,y));
  }
}

bool inside(const Point& p, const vector<Point>& shape, Kernel traits) {
  return CGAL::bounded_side_2(shape.begin(), shape.end(), p, traits) == CGAL::ON_BOUNDED_SIDE;
}

MedialGraph makeGraph(const Voronoi& v, const vector<Point>& shape) {
  MedialGraph mg;
  for (Voronoi::Edge_iterator it=v.edges_begin(); it!=v.edges_end(); it++) {
    if (it->is_segment()) {
      Point src = it->source()->point();
      Point trg = it->target()->point();
      if (inside(src, shape, Kernel()) &&
          inside(trg, shape, Kernel())) {
        mg.add(src, trg);
        cout << it->source()->point() << " " << it->target()->point() << endl;
      }
    }
  }
  return mg;
}

int main(int argc, char** argv) {

  // Get input file
  char* file;
  if (argc == 2) {
    file = argv[1];
  }
  else {
    cout << "Usage: " << endl;
    cout << "./" << argv[0] << " {Data File}" << endl;
    exit (EXIT_FAILURE);
  }

  // load data for polygon
  vector<Point> poly;
  load(file,poly);
  
  // Make voronoi diagram
  Voronoi vd;
  vd.insert(poly.begin(), poly.end());

  // Get internal voronoi diagram for medial axis  
  MedialGraph graph = makeGraph(vd, poly);
  vector<Vertex> vvvvv = graph.dijkstra(poly[0]);
  Point back = vvvvv.back().point();
  vvvvv = graph.dijkstra(back);
/*  for (vector<Vertex>::iterator it=vvvvv.begin(); it!=vvvvv.end(); it++) {
    cout << (*it).parent << " " << (*it).point() << endl;
  }*/

  return 0;
}