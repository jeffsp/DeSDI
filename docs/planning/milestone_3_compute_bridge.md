# PRD: Milestone 3 — End-to-End Pipeline & Compute-to-Data Verification

| Field | Value |
|---|---|
| **Status** | Draft |
| **Author** | Jeff Perry |
| **Created** | 2026-07-04 |
| **Milestone** | 3 of 3 |
| **Budget** | $15,000 |
| **Timeline** | Months 4–6 |

---

## 1. Overview

Milestone 3 completes the DeSDI vision by addressing the "data gravity" problem. Downloading petabytes of NASA HDF5 data to local machines for processing is inefficient and expensive. Instead, this milestone introduces the **Compute-to-Data Bridge**, utilizing Zero-Knowledge Proofs (ZKPs) and decentralized compute networks to push processing jobs directly to the storage providers hosting the data.

Additionally, this milestone delivers the **Public Web Dashboard**, providing a user-friendly graphical interface to query the FVM Spatial Registry (from Milestone 2), visualize datasets on a map, and trigger decentralized processing jobs.

---

## 2. Goals & Non-Goals

### Goals

1. **ZKP Compute Orchestration** — Integrate a zkVM (e.g., RISC Zero or SP1) to allow arbitrary Python/C++ analysis scripts to run over the decentralized HDF5 shards.
2. **Compute Verification** — Ensure that the remote compute node provides a cryptographic receipt proving the computation was executed correctly against the specific CID requested.
3. **Web Dashboard** — Build a modern, responsive web dashboard (React/Next.js) for dataset discovery, bounding box selection, and compute job triggering.
4. **End-to-End Demonstration** — Host sample NASA ICESat-2 datasets on the Filecoin network and demonstrate the full pipeline: parsing, indexing, and remote execution.

### Non-Goals

- Building a brand new decentralized compute network from scratch (we will leverage existing zkVMs and orchestrators).
- Complex multi-node parallel computing (MapReduce) — focus is on single-shard or batched-shard isolated execution.

---

## 3. Architecture

### 3.1 Component Overview

```
┌────────────────────────────────────────────────────────┐
│                  DeSDI Web Dashboard                   │
│          (React / Next.js / Mapbox / wagmi)            │
└───────┬────────────────────────────────────────┬───────┘
        │ 1. Query BBox                          │ 3. Submit Compute Job
┌───────▼─────────────┐                 ┌────────▼──────────────┐
│ FVM Spatial Registry│                 │ Compute Orchestrator  │
│ (Milestone 2)       │                 │ (zkVM Layer)          │
└───────┬─────────────┘                 └────────┬──────────────┘
        │ 2. Return CIDs                         │ 4. Fetch CID & Execute
        │                               ┌────────▼──────────────┐
        └───────────────────────────────► Filecoin Storage Node │
                                        │ (Hosts HDF5 Shards)   │
                                        └────────┬──────────────┘
                                                 │ 5. Return Result + ZK Receipt
                                        ┌────────▼──────────────┐
                                        │ Dashboard / User      │
                                        └───────────────────────┘
```

### 3.2 Directory Layout

```
monorepo-root/
├── tools/
│   └── frontend/                 # Web Dashboard
│       ├── package.json
│       ├── src/
│       │   ├── app/              # Next.js App Router
│       │   ├── components/       # Map, QueryPanel, JobTracker
│       │   └── hooks/            # Web3/FVM integration (wagmi/viem)
│       └── public/
├── compute/                      # ZKP Compute Bridge
│   ├── zk_guest/                 # Code compiled to zkVM architecture (RISC-V)
│   │   └── hdf5_analyzer.cpp     # Sample analysis script
│   └── host/
│       └── orchestrator.rs       # Job submission and verification service
└── infra/
    └── cdktf/                    # Infrastructure as code for web dashboard hosting
```

---

## 4. The ZKP Compute-to-Data Bridge

To verify remote execution, we utilize a Zero-Knowledge Virtual Machine (zkVM). 

1. **Guest Code**: A user writes an analysis script (e.g., calculating average photon height in a given area) compiled to the zkVM's architecture (typically RISC-V).
2. **Execution**: The storage provider runs this code inside the zkVM. The input to the code is the IPFS CID of the sharded data.
3. **Verification**: The zkVM generates a mathematical "receipt" (STARK/SNARK). 
4. **Trust**: The user receives the output (e.g., `average_height: 42.5m`) and the receipt. They verify the receipt instantly locally or via an FVM smart contract, mathematically proving the output is correct and derived *exactly* from the requested CID.

---

## 5. Web Dashboard Specification

**Tech Stack**: Next.js (App Router), TailwindCSS, Mapbox GL JS (or maplibre), `wagmi` / `viem` for FVM interaction.

**Core Features:**
- **Map View**: Interactive globe/map displaying bounding boxes of available datasets indexed on the FVM registry.
- **Wallet Connection**: Connect MetaMask/WalletConnect to interact with the Filecoin network.
- **Search Interface**: Filter by time range, dataset type (ATL03 vs ATL08), and draw bounding boxes on the map.
- **Compute Dashboard**: Select a region, choose an analysis script, dispatch the job, and view the verified ZKP results upon completion.

---

## 6. Granular Issue Decomposition

| # | Issue Title | Dependencies | Est. |
|---|---|---|---|
| 37 | `frontend: scaffold Next.js app with Tailwind and wagmi` | None | 20 min |
| 38 | `frontend: implement interactive Mapbox/Maplibre viewer` | #37 | 45 min |
| 39 | `frontend: integrate FVM Spatial Registry read functions` | #37, M2 | 30 min |
| 40 | `frontend: visualize returned bounding boxes and CIDs on map` | #38, #39 | 30 min |
| 41 | `compute: scaffold zkVM host and guest environment (e.g., RISC Zero)` | None | 30 min |
| 42 | `compute: write sample ZK analysis script (average height calc)` | #41 | 40 min |
| 43 | `compute: implement host service to fetch CID data and execute zkVM` | #41 | 45 min |
| 44 | `compute: implement local ZK receipt verification logic` | #43 | 25 min |
| 45 | `frontend: build job submission and result viewer UI` | #37 | 40 min |
| 46 | `infra: define AWS CDK / Vercel IaC for frontend hosting` | None | 20 min |
| 47 | `docs: complete end-to-end user guide and project conclusion` | All | 30 min |

---

## 7. Risks & Mitigations

| Risk | Impact | Mitigation |
|---|---|---|
| zkVM Performance | Generating ZK proofs for large data shards takes massive computational power and time. | Limit the size of shards processed inside the zkVM. Use modern GPU-accelerated provers if the orchestrator supports them. |
| Network Congestion | Fetching shards over IPFS into the compute environment can be slow. | Colocate the compute node directly alongside the Filecoin storage provider hardware (Compute-to-Data). |
| Frontend RPC Limits | High volumes of map queries might rate-limit the FVM RPC provider. | Implement a caching layer or subgraph (e.g., Envio/TheGraph) to index contract events for the frontend. |

---

## 8. Success Criteria

- [ ] End-to-end test processes an ATL03 file, registers it to the FVM Calibration testnet, and successfully executes a verified ZKP analysis script against it.
- [ ] Web dashboard is deployed and accessible via public URL.
- [ ] A user can draw a bounding box on the web dashboard, retrieve the intersecting datasets from the FVM contract, and view the CIDs.
- [ ] A valid ZK receipt is generated and successfully verified for the sample analysis script.
- [ ] The full open-source repository is finalized and published for the Filecoin DevGrant completion.
