#include "desdi/hilbert_sharder.hpp"
#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_floating_point.hpp>

TEST_CASE ("Hilbert Sharder: Empty input", "[hilbert_sharder]")
{
    std::vector<desdi::GeoPoint> points;
    auto shards = desdi::shard_points (points, 13);
    REQUIRE (shards.empty ());
}

TEST_CASE ("Hilbert Sharder: Single cell", "[hilbert_sharder]")
{
    using Catch::Matchers::WithinAbs;

    std::vector<desdi::GeoPoint> points = {{30.260, -97.740, 150.0, 1000.0, 0},
                                           {30.261, -97.741, 151.0, 1001.0, 0},
                                           {30.262, -97.742, 149.0, 1002.0, 0}};

    // Very close points should fall into the same cell at a low level (e.g.
    // level 5)
    auto shards = desdi::shard_points (points, 5);

    REQUIRE (shards.size () == 1);

    auto &shard = shards.begin ()->second;
    REQUIRE (shard.meta.point_count == 3);
    REQUIRE (shard.points.size () == 3);

    // Check bounding box and time range
    REQUIRE_THAT (shard.meta.min_lat, WithinAbs (30.260, 1e-9));
    REQUIRE_THAT (shard.meta.max_lat, WithinAbs (30.262, 1e-9));
    REQUIRE_THAT (shard.meta.min_lon, WithinAbs (-97.742, 1e-9));
    REQUIRE_THAT (shard.meta.max_lon, WithinAbs (-97.740, 1e-9));

    REQUIRE_THAT (shard.meta.min_time, WithinAbs (1000.0, 1e-9));
    REQUIRE_THAT (shard.meta.max_time, WithinAbs (1002.0, 1e-9));
}

TEST_CASE ("Hilbert Sharder: Cross-face tracks", "[hilbert_sharder]")
{
    std::vector<desdi::GeoPoint> points = {
        {0.0, 0.0, 10.0, 100.0, 0},  // Face 0 (+X)
        {0.0, 90.0, 10.0, 100.0, 0}, // Face 2 (+Y)
        {90.0, 0.0, 10.0, 100.0, 0}  // Face 4 (+Z)
    };

    auto shards = desdi::shard_points (points, 10);

    // These points are far apart on different faces, so they should be in 3
    // distinct shards
    REQUIRE (shards.size () == 3);
}
