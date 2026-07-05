#pragma once

#include <cmath>
#include <numbers>
#include <algorithm>

namespace desdi {

/**
 * @brief Result of projecting a WGS84 point onto the cubed-sphere.
 */
struct ProjectionResult {
    int face;     ///< Cube face index (0 to 5)
    double u;     ///< Normalized u coordinate [0.0, 1.0]
    double v;     ///< Normalized v coordinate [0.0, 1.0]
    
    bool operator==(const ProjectionResult&) const = default;
};

/**
 * @brief Projects a latitude and longitude to a normalized cubed-sphere face.
 * 
 * Maps the unit sphere to a cube, determining which face the point lies on,
 * and outputs the normalized (0 to 1) 2D coordinates on that face.
 * 
 * @param lat Latitude in decimal degrees
 * @param lon Longitude in decimal degrees
 * @return ProjectionResult containing the face and normalized u,v coordinates.
 */
inline ProjectionResult project_to_cubed_sphere(double lat, double lon) {
    constexpr double deg2rad = std::numbers::pi / 180.0;
    
    double rlat = lat * deg2rad;
    double rlon = lon * deg2rad;
    
    // Convert to 3D Cartesian coordinates on the unit sphere
    double x = std::cos(rlat) * std::cos(rlon);
    double y = std::cos(rlat) * std::sin(rlon);
    double z = std::sin(rlat);
    
    double ax = std::abs(x);
    double ay = std::abs(y);
    double az = std::abs(z);
    
    int face = 0;
    double u = 0.0, v = 0.0;
    
    // Determine the face and project
    if (ax >= ay && ax >= az) {
        if (x > 0) {
            face = 0; // +X
            u = y / x; v = z / x;
        } else {
            face = 1; // -X
            u = y / -x; v = z / -x;
        }
    } else if (ay >= ax && ay >= az) {
        if (y > 0) {
            face = 2; // +Y
            u = -x / y; v = z / y;
        } else {
            face = 3; // -Y
            u = -x / -y; v = z / -y;
        }
    } else {
        if (z > 0) {
            face = 4; // +Z
            u = y / z; v = -x / z;
        } else {
            face = 5; // -Z
            u = y / -z; v = -x / -z;
        }
    }
    
    // Normalize u, v from [-1, 1] to [0, 1]
    u = (u + 1.0) / 2.0;
    v = (v + 1.0) / 2.0;
    
    // Clamp to exactly [0, 1] to handle floating-point precision issues
    u = std::clamp(u, 0.0, 1.0);
    v = std::clamp(v, 0.0, 1.0);
    
    return {face, u, v};
}

} // namespace desdi
