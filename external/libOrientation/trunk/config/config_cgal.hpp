#ifndef CONFIG_CGAL_H_
#define CONFIG_CGAL_H_

#define ORIENTATIONMATIS_USE_CGAL 0

#if ORIENTATIONMATIS_USE_CGAL

#include <CGAL/Cartesian.h>
#include <CGAL/Polygon_2.h>
#include <CGAL/convex_hull_2.h>

typedef CGAL::Cartesian<double>		K;
typedef K::Point_2					Point;
typedef CGAL::Polygon_2<K>			Polygon_2;

typedef Polygon_2					footprint_type;

#endif // ORIENTATIONMATIS_USE_CGAL

#endif /*CONFIG_CGAL_H_*/