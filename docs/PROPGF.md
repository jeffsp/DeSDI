# ProPGF Grant Proposal - `DeSDI: HDF5-to-IPLD for NASA Earthdata`

**Project Name:** DeSDI

**Area of Focus:** `Tooling & Dev Ecosystem`

**Individual or Entity Name:** Jeff Perry

**Proposer:** `jeffsp`

**Project Repo(s):** https://github.com/jeffsp/desdi

**(Optional) Filecoin ecosystem affiliations:** N/A

**(Optional) Technical Sponsor:** N/A

**Do you agree to open source all work you do on behalf of this RFP under the MIT/Apache-2 dual-license?:** Yes

# Project Summary

The **HDF5-to-IPLD Tool** bridges the gap between complex multi-dimensional scientific computing formats and decentralized Web3 storage networks. Developed under the **Decentralized Spatial Data Infrastructure (DeSDI) Initiative**, this open-source solution provides a specialized ingestion pipeline to parse, shard, and permanently index critical public-goods datasets, such as petabytes of NASA earth observation data, onto the Filecoin network.

Traditional decentralized protocols route and locate data strictly via cryptographic Content Identifiers (CIDs) rather than geographic or temporal coordinates. This tool extracts spatial metadata natively during the ingestion process and integrates it with a decentralized spatial indexing layer built on the SpatioTemporal Asset Catalog (STAC) standard. As a result, researchers can query, index, and locate ATLAS Ice, Cloud, and land Elevation Satellite-2 (ICESat-2) laser altimetry products and multi-dimensional HDF5 data cubes by physical geography rather than abstract cryptographic hashes.

## Strategic Network Alignment & Impact

* **Shift to Value & Paid Onchain Deal Volume:** Generic, unmaintained storage pinning is commoditized. Filecoin's network priority centers on high-volume, verified scientific datasets that drive sustained, paid onchain storage deals and recurring FVM deal renewals.
* **Verifiable Scientific Data Rails:** Centralized S3 buckets lack cryptographic content-addressable integrity across multi-decadal research horizons and are vulnerable to deprecation or sudden changes in hosting policies. DeSDI establishes an open-source, reproducible data pipeline for institutional researchers and public-goods scientific archives.
* **High-Throughput Spatial Retrieval:** By preserving spatial locality across Content Identifiers (CIDs) and Content Addressable aRchives (CAR files), researchers can query bounding boxes and fetch spatial subsets directly without downloading multi-terabyte raw blobs.

## Outcomes

* **Native Parser & Spatial CAR Engine (C++23):** A high-performance parsing library that ingests `.h5` files natively, maps multi-dimensional photon returns to spatial chunks using a cubed-sphere Hilbert curve projection, and packs them into deterministic CAR files with spatial index manifests.
* **FVM Deal Management & Spatial Registry:** Smart contracts deployed to the Filecoin Virtual Machine (FVM) that anchor spatial bounding boxes to piece CIDs, automate deal negotiations with verified Storage Providers, monitor Proof of Spacetime (PoSt), and manage automated deal renewals.
* **Public Hosted Explorer & Retrieval Endpoint:** An interactive, map-based query interface and API enabling sub-second spatial tile discovery and verified fetching of NASA ICESat-2 granules from decentralized storage nodes.
* **Institutional Pilot & Benchmark Dataset:** Live deployment and permanent onchain archival of 1–5+ TB of NASA ICESat-2 (ATL24) public datasets with comprehensive documentation and reproducibility quickstarts.

## Data Onboarding

* Month #1: 0 (Architecture, packaging specifications, and core parser development)
* Month #3: 100 GB (Test dataset deployment on Filecoin Calibration testnet)
* Month #6: 1–5+ TB (Verified onchain deals for sample NASA ICESat-2 granules on Filecoin Mainnet)
* Month #12: 50–100+ TB (Full production pipeline scaling to additional NASA earth observation products)

## Adoption, Reach, and Growth Strategies

The primary target audience comprises Earth observation scientists, polar cryosphere researchers, and geospatial data engineers. Direct initial adoption centers on the NASA ICESat-2 research community, where eliminating the need to download entire multi-gigabyte HDF5 granules delivers an immediate 10x improvement in time-to-insight. We will engage the open-science and Web3 communities via the NASA Space Apps Challenge, the Open Geospatial Consortium (OGC), and Filecoin community forums.

## Development Roadmap & Milestones

### Milestone 1: Data Pipeline & Spatial CAR Engine
* **Functionality:** High-performance C++23 open-source parsing library for `.h5` (HDF5) files. Native extraction of geospatial coordinates and photon attributes. Spatial chunking based on a cubed-sphere Hilbert curve projection. Deterministic generation of Content Addressable aRchives (CAR files) and spatial index manifests linking geographic bounding boxes to byte offsets and CIDs. CLI tooling for local dataset staging and sharding.
* **Team:** Jeff Perry (Lead Systems Engineer), DSP (Frontend/Web3 Developer)
* **Funding:** $55,000
* **Timeframe:** Months 1–2 (8 weeks)
* **Verification / Acceptance Criteria:**
  * Public GitHub repository tagged release under dual MIT/Apache-2.0 license.
  * Automated unit and integration test suite passing with >80% code coverage.
  * Published benchmark report demonstrating high-throughput packaging speed and spatial index generation on a reference HDF5 dataset (>100 GB).

### Milestone 2: Onchain Deal Management & FVM Layer
* **Functionality:** Smart contracts deployed on the Filecoin Virtual Machine (FVM) providing an immutable, STAC-compliant spatial registry. Implementation of spatial bounding box lookup functions mapping geographic areas to stored piece CIDs. Integration with Filecoin storage client onramps (e.g., Storacha / Basin / direct deal APIs) for multi-provider redundancy. Automated deal renewal orchestrator and proof-of-spacetime verification monitor.
* **Team:** Jeff Perry (Lead Systems Engineer), DSP (Frontend/Web3 Developer)
* **Funding:** $60,000
* **Timeframe:** Months 3–4 (8 weeks)
* **Verification / Acceptance Criteria:**
  * Deployed and verified smart contracts on Filecoin Calibration testnet and Mainnet with public contract addresses and ABI documentation.
  * CLI tool and SDK demonstrating end-to-end automated deal creation and deal-state tracking for packaged spatial CAR files.
  * Published test suite and integration walkthrough demonstrating automated renewal triggers and deal health checks.

### Milestone 3: Institutional NASA Pilot & Retrieval Showcase
* **Functionality:** Live onboarding and permanent storage of real-world open NASA ICESat-2 (ATL24) data products onto Filecoin mainnet storage providers. Development of an interactive, responsive web explorer and retrieval endpoint allowing researchers to specify geospatial bounding boxes, inspect metadata, and stream spatial chunks directly. Comprehensive documentation, developer quickstarts, and an open-science case study.
* **Team:** Jeff Perry (Lead Systems Engineer), DSP (Frontend/Web3 Developer)
* **Funding:** $55,000
* **Timeframe:** Months 5–6 (8 weeks)
* **Verification / Acceptance Criteria:**
  * Verified live storage deals on Filecoin Mainnet representing 1–5+ TB of actively stored NASA ICESat-2 data with public piece CIDs and provider IDs.
  * Publicly hosted web application demonstrating sub-second bounding box queries and verified chunk downloads from decentralized storage.
  * Published developer documentation, end-to-end tutorial, and open-science case study.

### Direct Costs: Infrastructure & Data Staging
* **Functionality:** Cloud staging compute instances for downloading, decompressing, and chunking massive NASA granules; Filecoin Mainnet storage deal collateral and provider deal fees; FVM smart contract deployment and transaction gas reserves.
* **Funding:** $15,000
* **Timeframe:** Months 1–6
* **Verification / Acceptance Criteria:**
  * Itemized expenditure report documenting staging compute hours, onchain deal collateral disbursements, and gas fees.

## Total Budget Requested

| Milestone # | Description | Deliverables | Completion Date | Funding |
| :--- | :--- | :--- | :--- | :--- |
| **M1** | Data Pipeline & Spatial CAR Engine | C++23 Parser, Spatial Chunker, CAR Packager, CLI | Month 2 | $55,000 |
| **M2** | Onchain Deal Management & FVM Layer | STAC Spatial Registry & Deal Orchestrator Contracts | Month 4 | $60,000 |
| **M3** | Institutional NASA Pilot & Retrieval Showcase | 1–5+ TB Live Onboarding, Web Explorer, Case Study | Month 6 | $55,000 |
| **Direct Costs** | Infrastructure & Onchain Gas | Staging Compute, Storage Collateral, Deal Fees | Months 1–6 | $15,000 |
| **Total** | | | | **$185,000** |

### Budget Justification & Team Allocation
* **Lead Systems Engineer (Jeff Perry):** 1.0 FTE over 6 months ($15,000/month = $90,000). Responsible for low-level C++23 parser development, Hilbert curve spatial chunking algorithms, CAR file packaging, and NASA ICESat-2 data pipeline integration.
* **Web3 & Frontend Engineer (DSP):** ~0.65 FTE over 6 months ($13,333/month = $80,000). Responsible for FVM smart contract implementation, deal client integrations, web explorer UI, Mapbox/GIS visualization, and retrieval APIs.
* **Infrastructure & Storage Operations:** $15,000. Dedicated to high-memory staging servers for multi-gigabyte HDF5 granule processing, Filecoin Mainnet deal collateral, and FVM transaction gas.

## Key Reviewer Questions Pre-empted

1. **"Why not just store this in AWS S3 or Google Cloud Open Data?"**
   * *Answer:* Centralized S3 buckets lack intrinsic cryptographic content-addressable integrity across multi-decadal research horizons. Centralized archives are susceptible to link rot, arbitrary egress pricing hikes, and institutional hosting budget cuts. Filecoin guarantees cryptographic data integrity and permanent availability through decentralized consensus and Proof of Spacetime (PoSt).
2. **"Who are the initial users and why will they adopt this?"**
   * *Answer:* Initial users are cryosphere and photonics researchers working with NASA ATLAS ICESat-2 data. Currently, researchers must download entire multi-gigabyte HDF5 granules just to access a fraction of localized photons. DeSDI's spatial indexing and retrieval pipeline delivers an order-of-magnitude reduction in data transfer and time-to-insight.
3. **"How does this project drive Filecoin network economics?"**
   * *Answer:* Scientific earth observation archives are massive, growing continuously by terabytes each month. Rather than ephemeral, one-off pinning, DeSDI establishes an active pipeline of paid onchain deals with automated FVM renewals and high-frequency spatial tile retrieval.

## Maintenance and Upgrade Plans

The repository is maintained as a strict monorepo enforcing code quality through automated CI/CD pipelines, modern C++23 standards, and infrastructure-as-code principles. Following the completion of Milestone 3, future roadmap phases will extend native parsing to Cloud-Optimized GeoTIFF (COG) and `.laz` point clouds, expand storage deals to additional NASA Distributed Active Archive Centers (DAACs), and support generalized SpatioTemporal Asset Catalog (STAC) API endpoints.

# Team

## Team Members

* Jeff Perry (Lead Systems Engineer & Project Lead)
* DSP (Web3 & Frontend Developer)

## Team Member LinkedIn Profiles

* https://www.linkedin.com/in/jeffsperry/

## Team Website

* https://github.com/jeffsp/desdi

## Relevant Experience

**Jeff Perry** is a researcher affiliated with the Center for Perceptual Systems at the University of Texas at Austin, specializing in machine learning, image science, natural scene statistics, and computer vision. Crucially for this proposal, he has collaborated directly with NASA as part of the science team to develop the ICESat-2 ATL24 data product algorithm, which classifies photon returns from the ATLAS instrument (permanently archived and distributed publicly by the NSIDC DAAC).

He brings decades of specialized experience in modern C++ systems engineering, high-performance computing, and scientific dataset parsing. While this initiative bridges into Web3 and Filecoin, the primary technical bottleneck is parsing complex multi-dimensional scientific datasets and structuring performant spatial indexes—domains where his direct experience authoring NASA ICESat-2 algorithms provides a decisive and unique advantage.

**DSP** is a frontend and Web3 developer specializing in building intuitive, design-forward user interfaces for decentralized applications. His background spans smart contract integration, Web3 deal flows, and modern GIS/mapping visualization libraries, bridging complex blockchain architecture into seamless researcher-facing tools.

## Team Code Repositories

* https://github.com/jeffsp/desdi

# Additional Information

* Best contact email: jeffsp@gmail.com
