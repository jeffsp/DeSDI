# PRD: Milestone 1 — Core Ingestion & Native Parser Engine

| Field | Value |
|---|---|
| **Status** | Draft |
| **Author** | Jeff Perry + Antigravity CLI |
| **Created** | 2026-06-23 |
| **Milestone** | 1 of 3 |
| **Budget** | $20,000 |
| **Timeline** | Months 0–2 |

---

## 1. Overview

Milestone 1 delivers the foundational data ingestion layer for DeSDI:
a pure C++23 library and CLI tool that reads NASA ICESat-2 HDF5 files
natively, spatially shards their contents using a cubed-sphere Hilbert
curve projection, computes deterministic IPFS CIDv1 identifiers for
each shard, and outputs a structured manifest suitable for downstream
registration on the Filecoin Virtual Machine.

No running IPFS node is required. The tool operates entirely offline,
producing a manifest and shard payloads that can be ingested into IPFS
or Filecoin in later milestones.

---

## 2. Goals & Non-Goals

### Goals

1. **Parse ICESat-2 ATL03 and ATL08 HDF5 files** — extract photon-level
   point cloud data, geolocation arrays, and internal metadata
   attributes using native HDF5 C++ bindings.
2. **Spatially shard data via cubed-sphere Hilbert curve** — project
   WGS84 coordinates onto a cubed-sphere, compute Hilbert curve indexes
   at a configurable cell level (default: level 13 ≈ 1.27 km²), and
   partition point data into spatially coherent shards.
3. **Generate deterministic CIDv1 identifiers** — serialize each shard
   as dag-cbor, hash with SHA-256, and produce stable, reproducible
   CIDv1 content identifiers.
4. **Output a structured manifest** — emit a JSON manifest mapping each
   shard's CID to its spatial bounding box, temporal range, source
   granule metadata, and Hilbert cell index. Optionally export as a
   STAC-compliant Item JSON.
5. **Deliver a minimal CLI** — `desdi ingest <file.h5>` as the single
   entry point for Milestone 1.
6. **Header-only C++23 library design** — the core parsing and sharding
   logic lives in `.hpp` files, following the project's coding
   standards.

### Non-Goals (deferred to later milestones)

- IPFS node interaction, pinning, or block transfer
- Smart contract deployment or FVM registry calls
- `.laz` or Cloud Optimized GeoTIFF (COG) parsing
- Web dashboard or query interface
- Multi-file batch ingestion orchestration
- Compute-to-Data bridge or ZKP verification

---

## 3. Target Data Format: ICESat-2

### Why ICESat-2?

ICESat-2's Advanced Topographic Laser Altimeter System (ATLAS) produces
among the densest point cloud datasets in the NASA Earthdata catalog.
ATL03 (Global Geolocated Photon Data) files routinely exceed 1 GB and
contain millions of geolocated photon returns — making them an ideal
stress test for a sharding pipeline.

### HDF5 Internal Structure (ATL03)

```
/
├── orbit_info/            # Orbit parameters
├── ancillary_data/        # Calibration and correction data
├── gt1l/                  # Ground track 1, left beam
│   ├── heights/
│   │   ├── lat_ph         # Photon latitude  (float64[])
│   │   ├── lon_ph         # Photon longitude (float64[])
│   │   ├── h_ph           # Photon height    (float32[])
│   │   ├── signal_conf_ph # Signal confidence (int8[])
│   │   └── delta_time     # Seconds since ATLAS SDP epoch
│   ├── geolocation/
│   │   ├── segment_id
│   │   ├── segment_ph_cnt
│   │   └── reference_photon_index
│   └── bckgrd_atlas/
├── gt1r/                  # Ground track 1, right beam
├── gt2l/, gt2r/, gt3l/, gt3r/  # Remaining beam pairs
└── quality_assessment/
```

### HDF5 Internal Structure (ATL08)

```
/
├── gt1l/
│   ├── land_segments/
│   │   ├── latitude       # Segment center latitude
│   │   ├── longitude      # Segment center longitude
│   │   ├── canopy/        # Canopy height metrics
│   │   ├── terrain/       # Terrain height metrics
│   │   └── segment_id_beg
│   └── signal_photons/
├── gt1r/, gt2l/, ...
└── ancillary_data/
```

---

## 4. Architecture

### 4.1 Component Overview

```
┌─────────────────────────────────────────────────────────────────┐
│                        desdi CLI                                │
│                  (desdi ingest <file.h5>)                       │
└─────────────────────┬───────────────────────────────────────────┘
                      │
          ┌───────────▼───────────┐
          │   hdf5_reader.hpp     │  ← Reads HDF5 via HighFive
          │   (ATL03/ATL08 aware) │     or HDF5 C++ API
          └───────────┬───────────┘
                      │  vector<GeoPoint>
          ┌───────────▼───────────┐
          │  hilbert_sharder.hpp  │  ← Cubed-sphere projection
          │  (spatial partitioning│     + Hilbert curve indexing
          │   at configurable     │
          │   cell level)         │
          └───────────┬───────────┘
                      │  map<HilbertIndex, vector<GeoPoint>>
          ┌───────────▼───────────┐
          │   cid_generator.hpp   │  ← dag-cbor serialization
          │   (CIDv1 / SHA-256)   │     + CIDv1 computation
          └───────────┬───────────┘
                      │  vector<ShardRecord>
          ┌───────────▼───────────┐
          │  manifest_writer.hpp  │  ← JSON manifest output
          │  (JSON + STAC export) │     + optional STAC Item
          └───────────┴───────────┘
                      │
                      ▼
              manifest.json  +  shards/
```

### 4.2 Directory Layout (under `parser/`)

```
parser/
├── CMakeLists.txt                # Top-level CMake config (FetchContent + find_package)
├── include/
│   └── desdi/
│       ├── geo_point.hpp         # GeoPoint struct (lat, lon, h, time, attrs)
│       ├── hdf5_reader.hpp       # HDF5 ingestion (ATL03/ATL08)
│       ├── cubed_sphere.hpp      # WGS84 → cubed-sphere face projection
│       ├── hilbert_curve.hpp     # Hilbert curve index computation
│       ├── hilbert_sharder.hpp   # Spatial sharding orchestrator
│       ├── dag_cbor.hpp          # Minimal dag-cbor encoder
│       ├── cid_generator.hpp     # CIDv1 (SHA-256 + dag-cbor) builder
│       ├── manifest_writer.hpp   # JSON / STAC manifest output
│       └── cli.hpp               # CLI argument parsing
├── src/
│   └── main.cpp                  # CLI entry point (thin — delegates to headers)
├── tests/
│   ├── CMakeLists.txt
│   ├── test_cubed_sphere.cpp
│   ├── test_hilbert_curve.cpp
│   ├── test_hilbert_sharder.cpp
│   ├── test_cid_generator.cpp
│   ├── test_hdf5_reader.cpp
│   ├── test_manifest_writer.cpp
│   ├── fixtures/                 # Synthetic HDF5 test files
│   │   └── generate_fixtures.py  # Script to create synthetic ATL03/ATL08
│   └── integration/
│       └── test_full_pipeline.cpp
└── README.md
```

### 4.3 Key Dependencies

Dependencies are split between **system packages** (installed via
`apt` / `brew`) and **FetchContent** (downloaded and built by CMake
automatically at configure time). No external package manager is
required.

**System packages** (`find_package()`):

| Dependency | Apt Package | Purpose |
|---|---|---|
| HDF5 (C library) | `libhdf5-dev` | Low-level `.h5` file I/O |
| OpenSSL (libcrypto) | `libssl-dev` | SHA-256 hashing for CID generation |

**FetchContent** (header-only or lightweight, built from source):

| Dependency | Repository | Purpose |
|---|---|---|
| [HighFive](https://github.com/BlueBrain/HighFive) | `BlueBrain/HighFive` | Header-only modern C++ wrapper over the HDF5 C API |
| [nlohmann-json](https://github.com/nlohmann/json) | `nlohmann/json` | JSON manifest output |
| [CLI11](https://github.com/CLIUtils/CLI11) | `CLIUtils/CLI11` | Command-line argument parsing |
| [Catch2](https://github.com/catchorg/Catch2) | `catchorg/Catch2` | Unit testing framework |

Example `CMakeLists.txt` snippet:

```cmake
include(FetchContent)

FetchContent_Declare(HighFive
  GIT_REPOSITORY https://github.com/BlueBrain/HighFive.git
  GIT_TAG        v2.10.0
)
FetchContent_Declare(nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        v3.11.3
)
FetchContent_Declare(CLI11
  GIT_REPOSITORY https://github.com/CLIUtils/CLI11.git
  GIT_TAG        v2.4.2
)
FetchContent_Declare(Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.7.1
)

set(HIGHFIVE_USE_BOOST OFF)
set(HIGHFIVE_UNIT_TESTS OFF)
set(HIGHFIVE_EXAMPLES OFF)
FetchContent_MakeAvailable(HighFive nlohmann_json CLI11 Catch2)

find_package(HDF5 REQUIRED COMPONENTS C)
find_package(OpenSSL REQUIRED)
```

> **Note:** The cubed-sphere projection and Hilbert curve are
> implemented from scratch (not using Google's S2 library). This avoids
> the S2 dependency and gives full control over cell-level granularity
> and the projection math.

---

## 5. Sharding Algorithm: Cubed-Sphere Hilbert Curve

### 5.1 Concept

The Earth's surface is projected onto 6 faces of a circumscribed cube.
Each face is subdivided into a 2^L × 2^L grid (where L is the cell
level). A Hilbert space-filling curve maps each 2D grid cell to a 1D
index, preserving spatial locality — nearby points on the surface map to
nearby indexes.

### 5.2 Algorithm Steps

```
Input:  vector<GeoPoint> points, int level = 13
Output: map<uint64_t, Shard>

1. For each point (lat, lon):
   a. Convert (lat, lon) → (x, y, z) on unit sphere (WGS84)
   b. Determine cube face (±X, ±Y, ±Z) by largest |component|
   c. Project onto face → (u, v) ∈ [0, 1] × [0, 1]
   d. Quantize (u, v) → (i, j) on 2^L × 2^L grid
   e. Compute Hilbert index H = hilbert_xy_to_d(i, j, 2^L)
   f. Composite key = (face_id << (2*L)) | H

2. Group points by composite key → map<uint64_t, vector<GeoPoint>>

3. For each group:
   a. Compute bounding box (min_lat, min_lon, max_lat, max_lon)
   b. Compute temporal range (min_time, max_time)
   c. Serialize point data → dag-cbor bytes
   d. SHA-256 hash the dag-cbor bytes
   e. Construct CIDv1 = multibase(base32) + version(1)
      + multicodec(dag-cbor, 0x71) + multihash(sha256, 0x12, digest)
   f. Emit ShardRecord { cid, bbox, time_range, hilbert_index,
      point_count, byte_size }
```

### 5.3 Cell Level Reference

| Level | Grid | Approximate Cell Area | Cells per Face |
|---|---|---|---|
| 10 | 1024 × 1024 | ~100 km² | 1,048,576 |
| 13 | 8192 × 8192 | ~1.27 km² | 67,108,864 |
| 15 | 32768 × 32768 | ~0.08 km² | 1,073,741,824 |

Default is **level 13**. Configurable via `--level` CLI flag.

---

## 6. CID Construction Detail

```
CIDv1 = <multibase-prefix> <cid-version> <multicodec> <multihash>
       = "b"              + 0x01         + 0x71        + (0x12 + 0x20 + sha256_digest)
         (base32lower)     (CIDv1)        (dag-cbor)    (sha-256, 32 bytes, digest)
```

The dag-cbor payload for each shard:

```cbor
{
  "version": 1,
  "source": "ATL03_20210114000526_03381001_006_02.h5",
  "beam": "gt1l",
  "hilbert_cell": 1234567890,
  "level": 13,
  "bbox": [-97.74, 30.26, -97.73, 30.27],
  "time_range": [1687453200, 1687453800],
  "point_count": 42317,
  "data": <binary photon array>
}
```

**CIDs must be deterministic.** Given the same input file and the same
cell level, the tool must produce byte-identical output every time.
This means:
- Canonical dag-cbor encoding (sorted map keys, no indefinite-length)
- No timestamps or random values in the shard payload
- Consistent floating-point serialization

### Known CID Test Vector

The following minimal shard payload serves as a reference test vector.
Unit tests must reproduce this exact CID from this exact input.

**Input (JSON representation of the dag-cbor payload):**

```json
{
  "bbox": [-97.74, 30.26, -97.73, 30.27],
  "beam": "gt1l",
  "data": [],
  "hilbert_cell": 42,
  "level": 13,
  "point_count": 0,
  "source": "test_vector.h5",
  "time_range": [1000000, 1000001],
  "version": 1
}
```

**Canonical dag-cbor encoding rules applied:**
- Map keys sorted lexicographically: `bbox`, `beam`, `data`,
  `hilbert_cell`, `level`, `point_count`, `source`, `time_range`,
  `version`
- Integers use shortest CBOR encoding
- Floats use IEEE 754 double precision (CBOR major type 7, additional
  info 27)
- Empty array `data` encoded as `0x80`
- Strings encoded as UTF-8 with length prefix

**Expected output:**
- The exact dag-cbor byte sequence, SHA-256 digest, and resulting
  CIDv1 string must be computed during issue #11 implementation and
  recorded in the test file as a compile-time constant.
- CI enforces: `assert(compute_cid(test_payload) == EXPECTED_CID)`
- A second reference vector using a non-empty `data` array (3 photon
  records) must also be included.

---

## 7. CLI Specification

### Usage

```
desdi ingest <file.h5> [options]

Arguments:
  <file.h5>              Path to an ICESat-2 HDF5 file (ATL03 or ATL08)

Options:
  --level <int>          Hilbert cell level (default: 13)
  --output-dir <path>    Output directory for manifest + shards (default: ./output)
  --format <json|stac>   Manifest format (default: json)
  --beams <list>         Comma-separated beam names to process (default: all)
  --verbose              Enable detailed progress output
  --version              Print version and exit
  --help                 Print help and exit
```

### Output Structure

```
output/
├── manifest.json          # or manifest.stac.json
└── shards/
    ├── bafybeig...abc.cbor
    ├── bafybeig...def.cbor
    └── ...
```

### Exit Codes

| Code | Meaning |
|---|---|
| 0 | Success |
| 1 | Invalid arguments or missing file |
| 2 | HDF5 read error (corrupt file, missing datasets) |
| 3 | Sharding error |
| 4 | CID generation error |

---

## 8. Manifest Schema (JSON)

```json
{
  "version": 1,
  "generator": "desdi-ingest",
  "generator_version": "0.1.0",
  "source_file": "ATL03_20210114000526_03381001_006_02.h5",
  "source_sha256": "a1b2c3d4...",
  "hilbert_level": 13,
  "created_at": "2026-06-23T21:00:00Z",
  "total_shards": 847,
  "total_points": 12459823,
  "total_bytes": 498372641,
  "global_bbox": [-97.80, 30.20, -97.65, 30.35],
  "global_time_range": [1687453200, 1687456800],
  "shards": [
    {
      "cid": "bafybeig...abc",
      "hilbert_index": 1234567890,
      "face": 2,
      "bbox": [-97.74, 30.26, -97.73, 30.27],
      "time_range": [1687453200, 1687453800],
      "beam": "gt1l",
      "point_count": 42317,
      "byte_size": 587432
    }
  ]
}
```

---

## 9. STAC Export Schema

When `--format stac` is specified, the manifest is output as a
STAC-compliant Item:

```json
{
  "type": "Feature",
  "stac_version": "1.0.0",
  "id": "ATL03_20210114000526_03381001_006_02",
  "geometry": { "type": "Polygon", "coordinates": ["..."] },
  "bbox": [-97.80, 30.20, -97.65, 30.35],
  "properties": {
    "datetime": null,
    "start_datetime": "2021-01-14T00:05:26Z",
    "end_datetime": "2021-01-14T01:05:26Z",
    "desdi:hilbert_level": 13,
    "desdi:total_shards": 847,
    "desdi:total_points": 12459823
  },
  "assets": {
    "shard-0": {
      "href": "ipfs://bafybeig...abc",
      "type": "application/cbor",
      "title": "Shard 0 (gt1l, cell 1234567890)",
      "desdi:hilbert_index": 1234567890,
      "desdi:beam": "gt1l",
      "desdi:point_count": 42317
    }
  },
  "links": []
}
```

---

## 10. Testing Strategy

### Unit Tests (Synthetic Data)

| Test | What It Validates |
|---|---|
| `test_cubed_sphere` | WGS84 → cube face projection accuracy, edge cases at face boundaries |
| `test_hilbert_curve` | Hilbert index computation, known-value checks, round-trip (index ↔ xy) |
| `test_hilbert_sharder` | Point grouping, empty input, single-cell, cross-face tracks |
| `test_cid_generator` | Deterministic CID output, dag-cbor canonical encoding, known test vectors |
| `test_hdf5_reader` | Read synthetic ATL03/ATL08 fixtures, handle missing datasets gracefully |
| `test_manifest_writer` | JSON schema compliance, STAC schema compliance |

### Synthetic Fixture Generator

`tests/fixtures/generate_fixtures.py` creates small HDF5 files (~1 MB)
that mimic the ATL03/ATL08 internal structure with deterministic data.
Uses `h5py` (Python) to generate fixtures consumed by C++ tests.

### Integration Tests (Real Data, CI-only)

**Target granule:**
`ATL03_20210114000526_03381001_006_02.h5` (~600 MB)

This granule was selected because:
- It covers a mid-latitude ground track with moderate photon density
- It is publicly available from NSIDC (version 006)
- It exercises all 6 beam pairs
- Its size is large enough to stress the pipeline but small enough for
  CI time budgets (~2 min to download, ~6 sec to ingest at target
  throughput)

**Earthdata credential handling:**

1. CI variables `EARTHDATA_USER` and `EARTHDATA_PASSWORD` are stored
   as **masked, protected** GitLab CI/CD variables (Settings → CI/CD
   → Variables) — never committed to the repository.
2. The integration test job writes a temporary `.netrc` file:
   ```
   machine urs.earthdata.nasa.gov
   login $EARTHDATA_USER
   password $EARTHDATA_PASSWORD
   ```
3. Download uses `curl -n -L` with redirect-following (NSIDC uses
   OAuth2 redirects through `urs.earthdata.nasa.gov`).
4. The `.netrc` is deleted immediately after download.
5. Downloaded granules are cached in the CI cache keyed by filename +
   SHA-256 to avoid redundant downloads.

**Fallback:** If NSIDC is unreachable or credentials are not set, the
integration test is skipped (not failed) with a clear warning message.
The synthetic-data integration test (#20) always runs.

---

## 11. Performance Targets

| Metric | Definition | Target |
|---|---|---|
| Ingestion throughput | `(input HDF5 file size in MB) / (wall-clock seconds from CLI start to manifest written)`. Measured end-to-end including HDF5 read, sharding, dag-cbor serialization, SHA-256 hashing, and manifest write. Excludes process startup. | ≥ 100 MB/s on a 4-core x86_64 machine (GitLab CI runner baseline) |
| Memory high-water mark | Peak RSS as reported by `/usr/bin/time -v` or equivalent | < 2× input file size. For a 600 MB ATL03 granule, peak RSS must stay below 1.2 GB. |
| CID determinism | Run `desdi ingest` twice on the same input with the same flags. Compare output shard files byte-for-byte and manifest JSON field-for-field. | 100% identical output. Zero byte differences. Verified by a dedicated CI job that runs the pipeline twice and `diff`s the results. |
| Shard size distribution | `max_shard_bytes / median_shard_bytes` | Ratio < 10:1 at level 13 on the reference ATL03 granule. Extreme outliers indicate sharding bugs or degenerate Hilbert cells. |

### How to Measure

The CLI `--verbose` flag emits a timing summary line at exit:

```
[desdi] Ingested 612.4 MB in 5.83s (105.0 MB/s), peak RSS 987 MB,
        847 shards written
```

CI captures this line and parses it for regression tracking. A
performance regression > 20% from the previous commit on the same
runner triggers a warning (not a failure) to avoid flaky gates on
shared CI infrastructure.

---

## 11.1 CI Build Environment

### Docker Image

All parser CI jobs run on a custom Docker image based on
`ubuntu:24.04`. The `Dockerfile` lives at `infra/ci/parser.Dockerfile`
and is rebuilt on changes.

Because FetchContent downloads dependencies at CMake configure time,
the Docker image only needs system packages — no external package
manager bootstrap.

```dockerfile
FROM ubuntu:24.04

ENV DEBIAN_FRONTEND=noninteractive

# System dependencies (HDF5 C lib + OpenSSL for find_package)
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    g++-13 \
    cmake \
    ninja-build \
    git \
    curl \
    pkg-config \
    libhdf5-dev \
    libssl-dev \
    python3 \
    python3-h5py \
    ca-certificates \
    && rm -rf /var/lib/apt/lists/*
```

### GitLab CI Job Configuration (updated)

```yaml
test:parser:
  stage: test
  image: $CI_REGISTRY_IMAGE/ci-parser:latest
  script:
    - cd parser
    - cmake -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
    - cmake --build build
    - cd build && ctest --output-on-failure
  rules:
    - changes:
        - parser/**/*
```

FetchContent downloads are cached automatically by CMake's build
directory. The CI cache key includes `parser/CMakeLists.txt` so
dependency downloads are only re-fetched when versions change.

### Local Development

```bash
# Install system dependencies (Ubuntu/Debian)
sudo apt install libhdf5-dev libssl-dev cmake ninja-build g++-13

# Build — FetchContent handles everything else
cd parser
cmake -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

```bash
# macOS (Homebrew)
brew install hdf5 openssl cmake ninja
cd parser
cmake -B build -G Ninja \
  -DOPENSSL_ROOT_DIR=$(brew --prefix openssl)
cmake --build build
ctest --test-dir build --output-on-failure
```

### Minimum Toolchain Versions

| Tool | Minimum Version | Reason |
|---|---|---|
| GCC | 13+ | C++23 `std::ranges`, `std::expected`, `std::print` |
| Clang | 17+ | C++23 support parity |
| CMake | 3.28+ | `CMAKE_CXX_STANDARD 23`, mature FetchContent |
| HDF5 | 1.14+ | Thread-safe C++ API |
| Python | 3.10+ | Fixture generator (h5py) |

---

## 12. Agent-Sized Issue Decomposition

These are the implementation tasks, each scoped to ~30 minutes of
coding. Issues should be created in GitLab with labels `lane:cdx-any`
and `ready`.

| # | Issue Title | Dependencies | Est. |
|---|---|---|---|
| 1 | `parser: scaffold CMake project with FetchContent deps` | None | 20 min |
| 2 | `parser: implement GeoPoint struct and basic types` | #1 | 15 min |
| 3 | `parser: implement cubed-sphere face projection (cubed_sphere.hpp)` | #2 | 30 min |
| 4 | `parser: implement Hilbert curve xy↔d conversion (hilbert_curve.hpp)` | #2 | 30 min |
| 5 | `parser: unit tests for cubed-sphere projection` | #3 | 20 min |
| 6 | `parser: unit tests for Hilbert curve` | #4 | 20 min |
| 7 | `parser: implement spatial sharder (hilbert_sharder.hpp)` | #3, #4 | 30 min |
| 8 | `parser: unit tests for spatial sharder` | #7 | 25 min |
| 9 | `parser: implement dag-cbor encoder (dag_cbor.hpp)` | #2 | 30 min |
| 10 | `parser: implement CIDv1 generator (cid_generator.hpp)` | #9 | 30 min |
| 11 | `parser: unit tests for dag-cbor + CID generator` | #10 | 25 min |
| 12 | `parser: implement HDF5 reader for ATL03 (hdf5_reader.hpp)` | #2 | 30 min |
| 13 | `parser: implement HDF5 reader for ATL08` | #12 | 20 min |
| 14 | `parser: create synthetic HDF5 fixture generator (Python)` | None | 25 min |
| 15 | `parser: unit tests for HDF5 reader using synthetic fixtures` | #12, #14 | 25 min |
| 16 | `parser: implement JSON manifest writer (manifest_writer.hpp)` | #2 | 20 min |
| 17 | `parser: implement STAC export in manifest writer` | #16 | 25 min |
| 18 | `parser: unit tests for manifest writer (JSON + STAC)` | #17 | 20 min |
| 19 | `parser: implement CLI entry point with CLI11 (cli.hpp + main.cpp)` | #7, #10, #12, #16 | 30 min |
| 20 | `parser: integration test — full pipeline with synthetic data` | #19 | 30 min |
| 21 | `ci: update .gitlab-ci.yml for parser C++ build and test` | #1 | 20 min |
| 22 | `ci: add integration test job with real ATL03 download` | #20, #21 | 25 min |
| 23 | `parser: README with build instructions and usage examples` | #19 | 15 min |

### Dependency Graph

```
#1 (scaffold)
 ├── #2 (types)
 │    ├── #3 (cubed-sphere) ──── #5 (tests)
 │    ├── #4 (hilbert)     ──── #6 (tests)
 │    ├── #9 (dag-cbor)
 │    │    └── #10 (CID gen) ── #11 (tests)
 │    ├── #12 (HDF5 ATL03)
 │    │    ├── #13 (ATL08)
 │    │    └── #15 (tests) ◄── #14 (fixtures)
 │    └── #16 (manifest)
 │         ├── #17 (STAC)  ── #18 (tests)
 │         └───────────────┐
 │                         ▼
 ├── #3 + #4 ──► #7 (sharder) ── #8 (tests)
 │
 └── #7 + #10 + #12 + #16 ──► #19 (CLI)
                                  └── #20 (integration)
                                       └── #21 + #22 (CI)
                                            └── #23 (docs)

#14 (fixtures) ── independent, can run in parallel
```

### Parallelization Opportunities

Issues that can be worked on simultaneously by different agents:

- **Lane A**: #3, #4 (cubed-sphere + Hilbert — independent math modules)
- **Lane B**: #9 → #10 (dag-cbor + CID — independent from spatial code)
- **Lane C**: #12, #14 (HDF5 reader + fixture generator — independent)
- **Lane D**: #16 (manifest writer — independent from all above)

Maximum parallelism: **4 agents** working concurrently after #2 lands.

---

## 13. Risks & Mitigations

| Risk | Impact | Mitigation |
|---|---|---|
| Cubed-sphere face boundary edge cases | Points near face edges produce inconsistent cell assignments | Implement snapping logic with epsilon tolerance; extensive tests at all 12 cube edges |
| HDF5 file format variation across ATL03 versions | Reader crashes on unexpected structure | Defensive reading: check dataset existence before access; support ATL03 v005 and v006 |
| dag-cbor non-determinism | CIDs drift between runs, breaking content-addressing | Use canonical cbor encoding (RFC 8949 §4.2); CI test that runs ingestion twice and asserts identical output |
| Large file memory pressure | ATL03 files can exceed 4 GB | Stream beam-by-beam rather than loading entire file; shard and flush incrementally |
| Hilbert curve implementation bugs | Silent data corruption in spatial indexing | Compare against reference implementations; round-trip tests (index → xy → index) |

---

## 14. Success Criteria

Milestone 1 is complete when:

- [ ] `desdi ingest` successfully processes a real ICESat-2 ATL03 file
      and produces a valid manifest + shard directory
- [ ] CID output is deterministic (verified by running twice in CI)
- [ ] All unit tests pass in GitLab CI
- [ ] Integration test with a real ATL03 granule passes in CI
- [ ] `--format stac` produces a valid STAC Item JSON
- [ ] Parser processes ≥ 100 MB/s on CI runner hardware
- [ ] Code follows C++23 style, header-only, passes `clang-format`
- [ ] `parser/README.md` documents build + usage
