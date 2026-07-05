#include "desdi/geo_point.hpp"
#include <catch2/catch_test_macros.hpp>

TEST_CASE("GeoPoint struct initialization and comparison", "[geo_point]") {
  desdi::GeoPoint pt1{30.26, -97.74, 150.0, 10000.0, 42};

  REQUIRE(pt1.lat == 30.26);
  REQUIRE(pt1.lon == -97.74);
  REQUIRE(pt1.h == 150.0);
  REQUIRE(pt1.time == 10000.0);
  REQUIRE(pt1.attrs == 42);

  desdi::GeoPoint pt2{30.26, -97.74, 150.0, 10000.0, 42};
  REQUIRE(pt1 == pt2);

  desdi::GeoPoint pt3{30.26, -97.74, 150.0, 10000.0, 43};
  REQUIRE(pt1 != pt3);
}
