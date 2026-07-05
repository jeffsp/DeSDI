#include "desdi/cubed_sphere.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE("Cubed-sphere projection of primary axes", "[cubed_sphere]") {
  using Catch::Matchers::WithinAbs;
  constexpr double margin = 1e-9;

  SECTION("Face 0 (+X): Lat=0, Lon=0") {
    auto res = desdi::project_to_cubed_sphere(0.0, 0.0);
    REQUIRE(res.face == 0);
    REQUIRE_THAT(res.u, WithinAbs(0.5, margin));
    REQUIRE_THAT(res.v, WithinAbs(0.5, margin));
  }

  SECTION("Face 1 (-X): Lat=0, Lon=180") {
    auto res = desdi::project_to_cubed_sphere(0.0, 180.0);
    REQUIRE(res.face == 1);
    REQUIRE_THAT(res.u, WithinAbs(0.5, margin));
    REQUIRE_THAT(res.v, WithinAbs(0.5, margin));
  }

  SECTION("Face 2 (+Y): Lat=0, Lon=90") {
    auto res = desdi::project_to_cubed_sphere(0.0, 90.0);
    REQUIRE(res.face == 2);
    REQUIRE_THAT(res.u, WithinAbs(0.5, margin));
    REQUIRE_THAT(res.v, WithinAbs(0.5, margin));
  }

  SECTION("Face 3 (-Y): Lat=0, Lon=-90") {
    auto res = desdi::project_to_cubed_sphere(0.0, -90.0);
    REQUIRE(res.face == 3);
    REQUIRE_THAT(res.u, WithinAbs(0.5, margin));
    REQUIRE_THAT(res.v, WithinAbs(0.5, margin));
  }

  SECTION("Face 4 (+Z): North Pole") {
    auto res = desdi::project_to_cubed_sphere(90.0, 0.0);
    REQUIRE(res.face == 4);
    REQUIRE_THAT(res.u, WithinAbs(0.5, margin));
    REQUIRE_THAT(res.v, WithinAbs(0.5, margin));
  }

  SECTION("Face 5 (-Z): South Pole") {
    auto res = desdi::project_to_cubed_sphere(-90.0, 0.0);
    REQUIRE(res.face == 5);
    REQUIRE_THAT(res.u, WithinAbs(0.5, margin));
    REQUIRE_THAT(res.v, WithinAbs(0.5, margin));
  }
}

TEST_CASE("Cubed-sphere projection at boundaries", "[cubed_sphere]") {
  using Catch::Matchers::WithinAbs;
  constexpr double margin = 1e-9;

  // A point exactly between +X and +Y faces: Lon = 45, Lat = 0
  // x = cos(0)*cos(45) = ~0.707
  // y = cos(0)*sin(45) = ~0.707
  // This could map to Face 0 or Face 2 depending on FP math, but u or v will
  // be 0 or 1.
  auto res = desdi::project_to_cubed_sphere(0.0, 45.0);
  REQUIRE((res.face == 0 || res.face == 2));

  if (res.face == 0) {
    REQUIRE_THAT(res.u, WithinAbs(1.0, margin)); // y/x = 1 -> normalized to 1
    REQUIRE_THAT(res.v, WithinAbs(0.5, margin)); // z/x = 0 -> normalized to 0.5
  } else {
    REQUIRE_THAT(res.u, WithinAbs(0.0, margin)); // -x/y = -1 -> normalized to 0
    REQUIRE_THAT(res.v, WithinAbs(0.5, margin)); // z/y = 0 -> normalized to 0.5
  }
}
