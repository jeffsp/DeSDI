# PRD: Milestone 3 — Institutional NASA Pilot & Retrieval Showcase

| Field | Value |
|---|---|
| **Status** | Draft |
| **Author** | Jeff Perry |
| **Created** | 2026-09-27 |
| **Milestone** | 3 of 3 |
| **Budget** | $55,000 |
| **Timeline** | Months 5–6 |

---

## 1. Overview

Milestone 3 validates the DeSDI vision in production by addressing the real-world accessibility and retrieval bottlenecks of massive scientific datasets. Traditionally, researchers working with NASA earth observation data (such as ICESat-2) must download multi-gigabyte HDF5 files to local workstations even when studying small, localized regions.

This milestone executes the live onboarding of **1–5+ TB of NASA ICESat-2 (ATL24) laser altimetry products** onto Filecoin Mainnet storage providers. It delivers an interactive, publicly hosted **Spatial Web Explorer and Retrieval Endpoint**, allowing scientists to query geographic bounding boxes, locate matching spatial CAR chunks anchored on the FVM registry (Milestone 2), and stream localized point subsets directly from decentralized storage without fetching multi-terabyte raw blobs.

The milestone culminates in comprehensive developer documentation, benchmark reports, and an open-science case study demonstrating the 10x retrieval speedup and bandwidth savings.

---

## 2. Goals & Non-Goals

### Goals

1. **Live Mainnet Onboarding (1–5+ TB)** — Process, shard, package, and execute active storage deals for a representative catalog of NASA ICESat-2 (ATL24) granules across multiple verified Filecoin Storage Providers.
2. **Interactive Spatial Web Explorer** — Build and host a modern, responsive web application (Next.js, TailwindCSS, Mapbox/MapLibre, wagmi/viem) for geospatial dataset discovery and visual bounding-box selection.
3. **High-Throughput Spatial Retrieval Path** — Provide automated client tooling and HTTP retrieval endpoints to fetch localized spatial chunks and CAR manifests directly from decentralized storage nodes.
4. **End-to-End Pipeline Verification** — Demonstrate full round-trip workflows: raw HDF5 ingestion -> spatial CAR packing -> FVM onchain anchoring -> web explorer query -> sub-second chunk retrieval and photon point extraction.
5. **Open-Science Case Study & Documentation** — Publish a detailed technical case study, benchmarking report, and developer quickstart guide for researchers and data curators.

### Non-Goals

- Onboarding the entire multi-petabyte ICESat-2 archive in this single milestone (focus is on delivering and validating 1–5+ TB of high-value reference granules).
- Building bespoke GIS desktop software (all outputs conform to standard STAC and open spatial formats for direct use in QGIS, GDAL, and Python GIS tools).
- Proprietary closed-source hosting (all frontend, indexing, and retrieval utilities are fully open source under dual MIT/Apache-2.0).

---

## 3. Architecture

### 3.1 Component & Data Flow

```
┌────────────────────────────────────────────────────────┐
│               DeSDI Spatial Web Explorer               │
│          (Next.js / Mapbox GL JS / wagmi / viem)       │
└───────┬────────────────────────────────────────┬───────┘
        │ 1. Query Bounding Box                  │ 3. Fetch Spatial Chunks
        │    [MinX, MinY, MaxX, MaxY]            │    (Piece CID + Offset)
┌───────▼─────────────┐                 ┌────────▼──────────────┐
│ FVM Spatial Registry│                 │ Filecoin Storage Node │
│ (Milestone 2)       │                 │ / Retrieval Gateway   │
└───────┬─────────────┘                 └────────┬──────────────┘
        │ 2. Return Matching Piece CIDs          │ 4. Stream Spatial CAR
        │    & Hilbert Chunk Manifests           │    Shard (Photon Data)
        └────────────────────────────────────────►
                                        ┌────────▼──────────────┐
                                        │ Browser / Local GIS   │
                                        │ (Render Photons)      │
                                        └───────────────────────┘
```

### 3.2 Directory Layout

```
monorepo-root/
├── tools/
│   ├── frontend/                 # Web Explorer Application
│   │   ├── package.json
│   │   ├── src/
│   │   │   ├── app/              # Next.js App Router (Explorer UI)
│   │   │   ├── components/       # MapViewer, BBoxSelector, ChunkInspector
│   │   │   └── hooks/            # FVM wagmi/viem contract reader hooks
│   │   └── public/
│   └── retrieval/                # Fast Spatial Retrieval Client
│       ├── desdi_fetch.py        # Python SDK for streaming CAR chunks
│       └── cli/                  # CLI command: `desdi fetch --bbox ...`
├── docs/
│   ├── case_study/               # ICESat-2 ATL24 Open Science Report
│   └── quickstart.md             # Developer Quickstart & API Guide
└── infra/
    └── cdktf/                    # Declarative IaC for Web Explorer Hosting
```

---

## 4. Live Data Onboarding & Deal Orchestration

### 4.1 Target Dataset: NASA ICESat-2 ATL24

* **Product Description:** ATLAS/ICESat-2 Ocean Surface Photon and Atmospheric Data (ATL24), derived from photon returns classified by author Jeff Perry's algorithms.
* **Volume:** 1–5+ TB staged, sharded, and committed to active deals on Filecoin Mainnet.
* **Storage Redundancy:** Replicated across a minimum of 3 distinct, geographically distributed verified Storage Providers.
* **Metadata Registration:** Every sharded CAR piece is anchored in the FVM Spatial Registry with associated STAC metadata, bounding boxes, and temporal intervals.

### 4.2 Retrieval Pipeline Mechanics

1. **Spatial Filtering:** The researcher selects a region of interest (e.g., a polar ice shelf or coastal boundary).
2. **Registry Lookup:** The query calculates intersecting Hilbert indexes and retrieves matching piece CIDs from the FVM smart contract.
3. **Targeted Fetch:** Instead of downloading the full 2 GB HDF5 granule, the retrieval engine requests only the specific CAR byte ranges or spatial shards covering the bounding box.
4. **Data Unpacking:** The client extracts photon points and returns structured tabular data (or Parquet/GeoJSON) in seconds.

---

## 5. Web Explorer Specification

* **Tech Stack:** Next.js (App Router), TypeScript, TailwindCSS, Mapbox GL JS (or MapLibre), `wagmi` / `viem`.
* **Core Capabilities:**
  * **Interactive Bounding Box Selection:** Free-hand and coordinate-input bounding box selection on a global 3D/2D projection.
  * **Onchain Query Resolution:** Direct RPC queries to the FVM Spatial Registry on Filecoin Mainnet and Calibration testnet.
  * **Granule & Shard Inspector:** Displays piece CIDs, storage provider IDs, storage deal state, photon count, and temporal bounds.
  * **Direct Download / Stream:** Instant download link for retrieved spatial CAR shards and GeoJSON summaries.

---

## 6. Granular Issue Decomposition

These implementation tasks cover the frontend explorer, data onboarding, and retrieval tooling:

| # | Issue Title | Dependencies | Est. |
|---|---|---|---|
| 37 | `data: stage and prepare 1 TB NASA ICESat-2 reference dataset` | M1 | 4 days |
| 38 | `data: execute Filecoin Mainnet storage deals across 3+ providers` | #37, M2 | 3 days |
| 39 | `data: register all onboarded pieces into FVM Spatial Registry` | #38, M2 | 2 days |
| 40 | `frontend: scaffold Next.js application with Tailwind and wagmi` | None | 1 day |
| 41 | `frontend: implement Mapbox/MapLibre interactive globe and bbox drawer` | #40 | 3 days |
| 42 | `frontend: integrate FVM Spatial Registry contract queries` | #40, M2 | 2 days |
| 43 | `frontend: build shard inspector and CID metadata viewer` | #41, #42 | 2 days |
| 44 | `retrieval: implement Python client for targeted CAR chunk fetching` | M1, #38 | 3 days |
| 45 | `retrieval: add 'desdi fetch' CLI command with bbox and CID options` | #44 | 2 days |
| 46 | `infra: configure declarative deployment (Vercel/AWS) for explorer` | #40 | 1 day |
| 47 | `docs: author NASA ICESat-2 case study and retrieval benchmark report` | All | 3 days |
| 48 | `docs: publish end-to-end quickstart guide and API documentation` | All | 2 days |

---

## 7. Risks & Mitigations

| Risk | Impact | Mitigation |
|---|---|---|
| Storage Provider Retrieval Latency | SP retrieval endpoints may have variable response times or egress bottlenecks. | Replicate across multiple verified SPs; implement multi-gateway fallback (Lasp, Saturn, public IPFS gateways); support client-side CAR chunk caching. |
| Large Dataset Staging Egress | High ingress/egress costs when fetching raw granules from NASA DAACs. | Utilize AWS Open Data sponsorship or direct NASA Earthdata direct S3 egress within the `us-west-2` region. |
| FVM RPC Query Throughput | Heavy query loads on public FVM RPC nodes during live map exploration. | Cache indexed spatial events on the frontend or utilize an open subgraph (Envio/The Graph) for high-frequency RPC read caching. |

---

## 8. Success Criteria & Verification

- [ ] **Verified Mainnet Footprint:** At least 1–5+ TB of NASA ICESat-2 (ATL24) data actively stored on Filecoin Mainnet under live, verified storage provider deals with public piece CIDs.
- [ ] **Hosted Web Explorer:** Web explorer deployed to a public URL demonstrating responsive bounding-box searches and interactive map rendering.
- [ ] **Sub-Second Discovery:** Bounding box queries against the FVM registry return corresponding piece CIDs and chunk indexes in under 1 second.
- [ ] **Verified End-to-End Fetch:** A user can select a spatial bounding box, fetch the localized CAR chunk from Filecoin, and inspect photon coordinates locally without downloading the parent HDF5 file.
- [ ] **Published Case Study & Quickstart:** Comprehensive case study documenting bandwidth savings, retrieval benchmarks, and quickstart documentation published in the repository.
