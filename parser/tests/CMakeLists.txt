add_executable(desdi_tests test_geo_point.cpp test_cubed_sphere.cpp test_hilbert_curve.cpp test_hilbert_sharder.cpp)

target_link_libraries(desdi_tests PRIVATE
    desdi_lib
    Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(desdi_tests)
