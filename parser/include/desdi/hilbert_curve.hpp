#pragma once

#include <algorithm>
#include <cstdint>

namespace desdi {

/**
 * @brief Converts 2D grid coordinates to a 1D Hilbert index.
 *
 * @param n Grid size (must be a power of 2, e.g., 2^level)
 * @param x X coordinate (0 to n-1)
 * @param y Y coordinate (0 to n-1)
 * @return uint64_t 1D Hilbert index
 */
inline uint64_t xy_to_hilbert(uint64_t n, uint64_t x, uint64_t y) {
  uint64_t rx, ry, s, d = 0;
  for (s = n / 2; s > 0; s /= 2) {
    rx = (x & s) > 0;
    ry = (y & s) > 0;
    d += s * s * ((3 * rx) ^ ry);

    // Rotate and flip quadrant for next scale
    if (ry == 0) {
      if (rx == 1) {
        x = n - 1 - x;
        y = n - 1 - y;
      }
      std::swap(x, y);
    }
  }
  return d;
}

/**
 * @brief Converts a 1D Hilbert index to 2D grid coordinates.
 *
 * @param n Grid size (must be a power of 2)
 * @param d 1D Hilbert index
 * @param x Output X coordinate
 * @param y Output Y coordinate
 */
inline void hilbert_to_xy(uint64_t n, uint64_t d, uint64_t &x, uint64_t &y) {
  uint64_t rx, ry, s, t = d;
  x = y = 0;
  for (s = 1; s < n; s *= 2) {
    rx = 1 & (t / 2);
    ry = 1 & (t ^ rx);

    // Rotate and flip quadrant
    if (ry == 0) {
      if (rx == 1) {
        x = s - 1 - x;
        y = s - 1 - y;
      }
      std::swap(x, y);
    }
    x += s * rx;
    y += s * ry;
    t /= 4;
  }
}

/**
 * @brief Quantizes normalized (u,v) to a grid of size 2^level, then computes
 * the Hilbert index.
 *
 * @param u Normalized U coordinate [0.0, 1.0]
 * @param v Normalized V coordinate [0.0, 1.0]
 * @param level Resolution level (e.g., 13)
 * @return uint64_t Computed Hilbert index
 */
inline uint64_t compute_hilbert_index(double u, double v, int level) {
  uint64_t n = 1ULL << level;

  // Convert to grid coordinates, clamping at n-1 for exactly 1.0
  uint64_t x = std::min(static_cast<uint64_t>(u * n), n - 1);
  uint64_t y = std::min(static_cast<uint64_t>(v * n), n - 1);

  return xy_to_hilbert(n, x, y);
}

} // namespace desdi
