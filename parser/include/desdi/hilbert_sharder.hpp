#pragma once

#include "desdi/cubed_sphere.hpp"
#include "desdi/geo_point.hpp"
#include "desdi/hilbert_curve.hpp"
#include <algorithm>
#include <cstdint>
#include <map>
#include <vector>

namespace desdi
{

struct ShardMetadata
{
    uint64_t composite_index;
    int face;
    uint64_t hilbert_index;
    double min_lat, max_lat;
    double min_lon, max_lon;
    double min_time, max_time;
    size_t point_count;
};

struct Shard
{
    ShardMetadata meta;
    std::vector<GeoPoint> points;
};

/**
 * @brief Groups a vector of GeoPoints into spatial shards based on a
 * Cubed-Sphere Hilbert index.
 *
 * @param points The input vector of GeoPoints.
 * @param level The Hilbert cell level (default 13).
 * @return std::map<uint64_t, Shard> Map of composite_key to Shard.
 */
inline std::map<uint64_t, Shard>
shard_points (const std::vector<GeoPoint> &points, int level = 13)
{
    std::map<uint64_t, Shard> shards;

    for (const auto &pt : points)
    {
        auto proj = project_to_cubed_sphere (pt.lat, pt.lon);
        uint64_t h = compute_hilbert_index (proj.u, proj.v, level);

        // Composite key = (face_id << (2*L)) | H
        uint64_t composite_key =
            (static_cast<uint64_t> (proj.face) << (2 * level)) | h;

        auto &shard = shards[composite_key];

        if (shard.points.empty ())
        {
            shard.meta.composite_index = composite_key;
            shard.meta.face = proj.face;
            shard.meta.hilbert_index = h;
            shard.meta.min_lat = shard.meta.max_lat = pt.lat;
            shard.meta.min_lon = shard.meta.max_lon = pt.lon;
            shard.meta.min_time = shard.meta.max_time = pt.time;
            shard.meta.point_count = 0;
        }
        else
        {
            shard.meta.min_lat = std::min (shard.meta.min_lat, pt.lat);
            shard.meta.max_lat = std::max (shard.meta.max_lat, pt.lat);
            shard.meta.min_lon = std::min (shard.meta.min_lon, pt.lon);
            shard.meta.max_lon = std::max (shard.meta.max_lon, pt.lon);
            shard.meta.min_time = std::min (shard.meta.min_time, pt.time);
            shard.meta.max_time = std::max (shard.meta.max_time, pt.time);
        }

        shard.points.push_back (pt);
        shard.meta.point_count++;
    }

    return shards;
}

} // namespace desdi
