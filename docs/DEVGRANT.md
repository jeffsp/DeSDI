# Open Grant Proposal: `DeSDI -- HDF5-to-IPLD for NASA Earthdata`

**Project Name:** DeSDI

**Proposal Category:** `Developer and data tooling`

**Individual or Entity Name:** Jeff Perry

**Proposer:** `jeffsp`

**Project Repo(s)** https://github.com/jeffsp/desdi

**(Optional) Filecoin ecosystem affiliations:** N/A

**(Optional) Technical Sponsor:** N/A

**Do you agree to open source all work you do on behalf of this RFP under the MIT/Apache-2 dual-license?:** Yes

# Project Summary

The **HDF5-to-IPLD Tool** bridges the gap between complex
multi-dimensional scientific computing formats and decentralized Web3
storage networks. Developed under the **Decentralized Spatial Data
Infrastructure (DeSDI) Initiative**, this open-source solution
provides a specialized ingestion pipeline to parse, shard, and
permanently index critical public-good datasets, such as petabytes of
NASA earth-observation data, onto the Filecoin network.

Traditional decentralized protocols route and locate data strictly via
cryptographic Content Identifiers (CIDs) rather than geographic or
temporal coordinates. This tool extracts spatial metadata natively
during the ingestion process and integrates it with a decentralized
spatial indexing layer built on the SpatioTemporal Asset Catalog
(STAC) standard. As a result, researchers can query, index, and locate
ATLAS Ice, Cloud, and land Elevation Satellite-2 (ICESat-2) laser
altimetry products and multi-dimensional HDF5 data cubes by physical
geography rather than abstract cryptographic hashes.

## Impact 

- **Addressing "Data Gravity":** NASA datasets like ICESat-2 are
  massive, stored in complex formats (.h5), and downloading them
  locally for processing is inefficient. Web3 storage relies on
  cryptographic hashes which are useless to scientists who need to
  query data geographically.
- **Benefits:** By translating spatial coordinates into deterministic
  IPFS CIDs, we unlock petabytes of high-value scientific data for
  Web3, making it discoverable geographically and computable in place.
- **Ecosystem Impact:** It paves the way for a decentralized
  Compute-to-Data pipeline on Filecoin, allowing scientists to run
  Zero-Knowledge (ZKP) analysis directly on decentralized storage
  nodes instead of downloading gigabytes of data to local
  workstations.

## Outcomes

- **Native Parser Engine (C++23):** A fast parsing library that reads
  `.h5` files natively and transforms them into spatially coherent
  peer-to-peer content blocks using a cubed-sphere Hilbert curve
  projection.
- **Immutable Spatial Registry:** A STAC-compliant smart contract
  registry deployed on the Filecoin Virtual Machine (FVM) that links
  geometric bounding boxes to stored CIDs.
- **Compute-to-Data Bridge:** A ZKP-backed compute orchestrator
  allowing remote processing jobs to execute directly against the
  stored `.h5` datasets on Filecoin storage nodes.
- **Web Dashboard:** A public web-based query interface dashboard for
  spatial dataset discovery and remote compute execution.

## Data Onboarding

- Month #1: 0 (Development phase)
- Month #3: 100 GB (Test dataset deployment)
- Month #6: 10 TB (Sample NASA ICESat-2 granules)
- Month #12: 100+ TB (Full pipeline capability at scale)

## Adoption, Reach, and Growth Strategies

The target audience includes Earth scientists, geospatial data analysts, and Web3 developers. We will engage them via open-source communities, NASA Space Apps challenges, and Filecoin hackathons. Initial onboarding will focus on researchers working with ICESat-2 data, demonstrating a 10x improvement in time-to-insight by eliminating the need to download the dataset locally.

## Development Roadmap

### Milestone 1: Core Ingestion & Native Parser Engine
**Functionality:** Development of the C++23 open-source parsing
library capable of reading `.h5` files natively, sharding them
spatially, and outputting deterministic IPFS CIDv1 identifiers
alongside a JSON manifest.
**Team:** 1 Developer (Jeff Perry)
**Funding:** $20,000
**Timeframe:** Months 0-2 (8 weeks)

### Milestone 2: FVM Spatial Registry & Indexing Smart Contracts
**Functionality:** Deployment of the STAC-compliant smart contract
registry on the Filecoin Virtual Machine (FVM). Implementation of
lookup functions linking geometric bounding boxes to stored CIDs, and
an autonomous persistence manager to renew storage deals.
**Team:** 1 Developer (Jeff Perry)
**Funding:** $15,000
**Timeframe:** Months 2-4 (8 weeks)

### Milestone 3: End-to-End Pipeline & Compute-to-Data Verification
**Functionality:** Integration of the ZKP-backed compute orchestrator
allowing remote processing jobs to execute directly against the stored
`.h5` datasets. Delivery of a public web-based query interface
dashboard.
**Team:** 1 Developer (Jeff Perry)
**Funding:** $15,000
**Timeframe:** Months 4-6 (8 weeks)

## Total Budget Requested

| Milestone # | Description | Deliverables | Completion Date | Funding |
|---|---|---|---|---|
| 1 | Core Ingestion | C++23 Native Parser & CLI | Month 2 | $20,000 |
| 2 | FVM Registry | STAC Smart Contracts on FVM | Month 4 | $15,000 |
| 3 | Compute Bridge | ZKP Orchestrator & Web UI | Month 6 | $15,000 |
| **Total** | | | | **$50,000** |

## Maintenance and Upgrade Plans

The repository will be maintained as a strict monorepo, enforcing code
quality via comprehensive CI/CD pipelines and declarative
infrastructure-as-code. Long-term plans include extending the parsing
capabilities to support `.laz` and Cloud Optimized GeoTIFF (COG)
formats. The FVM smart contracts will be designed to support an
ecosystem endowment, ensuring permanent, unmanaged uptime for critical
scientific data via automated Filecoin storage renewals.

# Team

## Team Members

- Jeff Perry (Lead Developer)
- DSP (Developer)

## Team Member LinkedIn Profiles

- [https://www.linkedin.com/in/jeffsperry/]

## Team Website

- https://github.com/jeffsp/desdi

## Relevant Experience

Jeff Perry is a researcher affiliated with the Center for Perceptual
Systems at the University of Texas at Austin, specializing in machine
learning, image science, natural scene statistics, and computer
vision. Crucially for this proposal, he has collaborated with NASA as
part of a science team to develop the ICESat-2 ATL24 data product
algorithm, which classifies photon returns from the ATLAS instrument
(now permanently archived and distributed publicly by the NSIDC, one
of NASA's Distributed Active Archive Centers (DAACs)).

He also has extensive experience in modern C++ systems engineering.
While this project ventures into Web3 and Filecoin, the core
architecture heavily relies on parsing complex scientific datasets and
high-performance offline processing—domains where his direct experience
authoring NASA ICESat-2 algorithms and his academic research provide a
unique and decisive advantage.

DSP is a frontend developer and Web3 engineer specializing in building
intuitive, design-forward interfaces for decentralized applications.
His experience bridges the gap between complex blockchain architecture
and seamless user experiences using modern web design.

## Team code repositories

- https://github.com/jeffsp/desdi

# Additional Information
- Discovered Open Grants through the Filecoin ecosystem channels.
- Best email: [jeffsp@gmail.com]
