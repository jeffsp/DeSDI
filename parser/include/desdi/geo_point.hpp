#pragma once

#include <cstdint>

namespace desdi {

/**
 * @brief Represents a single point extracted from an ICESat-2 HDF5 file.
 */
struct GeoPoint {
    double lat;         ///< Latitude in decimal degrees (WGS84)
    double lon;         ///< Longitude in decimal degrees (WGS84)
    double h;           ///< Elevation/Height in meters
    double time;        ///< Timestamp (e.g., GPS seconds or delta time)
    uint32_t attrs;     ///< Additional bit-packed attributes (confidence, flags, etc.)

    // Enable default equality comparison
    bool operator==(const GeoPoint&) const = default;
};

} // namespace desdi
