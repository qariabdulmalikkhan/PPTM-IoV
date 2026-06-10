# PPTM-IoV: Privacy-Preserving Trust Management Framework for Internet of Vehicles

## Overview

This repository contains the implementation and experimental artifact of PPTM-IoV, a Privacy-Preserving Trust Management Framework for Internet of Vehicles (IoV).

The framework focuses on secure trust evaluation and malicious vehicle detection in vehicular networks while preserving privacy and maintaining efficient communication.

The implementation integrates:

* Trust-based vehicle evaluation
* Direct trust computation
* Reward and punishment mechanisms
* Malicious vehicle detection
* Road-side Unit (RSU) assisted trust management
* SUMO mobility traces
* NS-3 network simulation
* NetAnim visualization

The repository provides all files required to reproduce the experiments reported in the paper.

---

## Repository Structure

```text
PPTM-IoV/
│
├── README.md
│
├── mobility/
│   └── faisal.tcl
│
├── sumo/
│   ├── faisal.osm
│   ├── faisal.net.xml
│   ├── faisal.rou.xml
│   ├── faisal.sumocfg
│
├── ns3/
│   └── pptm-iov.cc
│
├── netanim/
│   └── pptm-iov.xml
│
├── results/
│   ├── latency.csv
│   ├── pdr.csv
│   ├── throughput.csv
│   ├── overhead.csv
│   └── trust_accuracy.csv

```

---

## Simulation Environment

The experiments were conducted using:

| Component | Version   |
| --------- | --------- |
| Ubuntu    | 22.04 LTS |
| NS-3      | 3.43      |
| NetAnim   | 3.109     |
| SUMO      | 1.21      |
| GCC       | 13.x      |

---

## Scenario Configuration

| Parameter         | Value       |
| ----------------- | ----------- |
| Vehicles          | 47          |
| RSUs              | 3           |
| Mobility Source   | SUMO        |
| Mobility Trace    | faisal.tcl  |
| Simulation Time   | 850 seconds |
| Trust Threshold   | 0.30        |
| Reward Factor     | 0.05        |
| Punishment Factor | 0.10        |

---

## Trust Management Model

Each vehicle maintains:

* Direct Trust
* Indirect Trust
* Total Trust

Trust is periodically updated using reward and punishment mechanisms.

Vehicles exhibiting malicious behavior experience trust degradation.

When the total trust falls below the trust threshold, the vehicle is identified as malicious.

---

## Experimental Metrics

The following performance metrics are evaluated:

* Average Latency
* Packet Delivery Ratio (PDR)
* Throughput
* Communication Overhead
* Trust Accuracy

All generated metrics are stored in the `results/` directory.

---

## Running the Simulation

### Build NS-3

```bash
./ns3 build
```

### Execute PPTM-IoV

```bash
./ns3 run scratch/pptm/pptm-iov
```

---

## NetAnim Visualization

The simulation automatically generates:

```text
pptm-iov.xml
```

Open NetAnim:

```bash
NetAnim
```

Then:

1. Click **File → Open**
2. Select `pptm-iov.xml`
3. Press **Play**

Visualization colors:

* Green = Normal Vehicle
* Red = Detected Malicious Vehicle
* Blue = RSU

---

## SUMO Mobility Generation

The vehicular mobility traces are generated using SUMO.

Typical workflow:

```text
OpenStreetMap
        ↓
      SUMO
        ↓
faisal.net.xml
        ↓
faisal.rou.xml
        ↓
faisal.sumocfg
        ↓
traceExporter.py
        ↓
faisal.tcl
        ↓
NS-3
```

---

## Reproducing the Experiment

1. Install Ubuntu 22.04
2. Install NS-3.43
3. Install NetAnim 3.109
4. Install SUMO 1.21
5. Copy `pptm-iov.cc` into the NS-3 scratch directory
6. Place `faisal.tcl` in the mobility directory
7. Build NS-3
8. Execute the simulation
9. Collect output CSV files from `results/`
10. Open `pptm-iov.xml` in NetAnim

---

## Implementation Status

* SUMO mobility integration
* RSU deployment
* Trust initialization
* Trust updating
* Reward mechanism
* Punishment mechanism
* Malicious vehicle detection
* NetAnim visualization
* Performance metric generation

---

## License

This repository is released for academic and research purposes.

---

## Contact

For questions regarding the implementation or experimental setup, please contact the principal author.
