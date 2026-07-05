#include <catch2/catch_test_macros.hpp>
#include "desdi/hilbert_curve.hpp"

TEST_CASE("Hilbert curve 2x2 base case (level 1)", "[hilbert_curve]") {
    uint64_t n = 2;
    // Expected Hilbert mapping for n=2:
    // (0,0) -> 0
    // (0,1) -> 1
    // (1,1) -> 2
    // (1,0) -> 3
    
    REQUIRE(desdi::xy_to_hilbert(n, 0, 0) == 0);
    REQUIRE(desdi::xy_to_hilbert(n, 0, 1) == 1);
    REQUIRE(desdi::xy_to_hilbert(n, 1, 1) == 2);
    REQUIRE(desdi::xy_to_hilbert(n, 1, 0) == 3);
}

TEST_CASE("Hilbert curve round-trip conversion", "[hilbert_curve]") {
    uint64_t n = 16; // Level 4
    
    for (uint64_t d = 0; d < n * n; ++d) {
        uint64_t x, y;
        desdi::hilbert_to_xy(n, d, x, y);
        uint64_t converted_d = desdi::xy_to_hilbert(n, x, y);
        REQUIRE(converted_d == d);
    }
}

TEST_CASE("Compute Hilbert index from normalized coordinates", "[hilbert_curve]") {
    int level = 10;
    uint64_t n = 1ULL << level; // 1024
    
    // (0,0) should go to (0,0) -> index 0
    REQUIRE(desdi::compute_hilbert_index(0.0, 0.0, level) == 0);
    
    // (1,1) should clamp to (1023, 1023) 
    uint64_t expected_max_d = desdi::xy_to_hilbert(n, n - 1, n - 1);
    REQUIRE(desdi::compute_hilbert_index(1.0, 1.0, level) == expected_max_d);
    
    // Out of bounds test should still clamp gracefully
    REQUIRE(desdi::compute_hilbert_index(1.5, 1.5, level) == expected_max_d);
}
